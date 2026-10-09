// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/app/core_window_manager.h"
#include "nau/asset_pack/asset_pack_builder.h"
#include "nau/io/asset_pack_file_system.h"
#include "nau/io/memory_stream.h"
#include "nau/io/virtual_file_system.h"
#include "nau/module/module_manager.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/string/string_conv.h"

namespace nau::sample
{
    std::filesystem::path lookupProjectRootPath(const std::filesystem::path& projectName)
    {
        namespace fs = std::filesystem;

        const fs::path projectRelPath = fs::path{"samples"} / projectName;
        const fs::path lookupPath = projectRelPath / "CMakeLists.txt";
        fs::path currentPath = fs::current_path();

        do
        {
            auto targetPath = currentPath / lookupPath;
            if (fs::exists(targetPath))
            {
                return fs::canonical(currentPath / projectRelPath);
            }

            currentPath = currentPath.parent_path();

        } while (currentPath.has_relative_path());

        return {};
    }

    void configureVirtualFileSystem(io::IVirtualFileSystem& vfs)
    {
        namespace fs = std::filesystem;

        const auto projectRootDir = lookupProjectRootPath("assetPackSample");
        NAU_ASSERT(!projectRootDir.empty() && fs::exists(projectRootDir));
        if (projectRootDir.empty() || !fs::exists(projectRootDir))
        {
            return;
        }

        const auto projectContentDir = projectRootDir / "content";

        auto contentFs = io::createNativeFileSystem(projectContentDir.string());
        vfs.mount("/content", std::move(contentFs)).ignore();
    }

    Result<io::IMemoryStream::Ptr> sampleBuildAssetPack()
    {
        PackBuildOptions options;
        options.contentType = "application/json";
        options.description = "texture pack";
        options.version = "0.1";

        eastl::vector<PackInputFileData> packData;

        for (int i = 0; i < 2; ++i)
        {
            PackInputFileData& texture = packData.emplace_back();
            std::string filePath = std::format("/textures/{}/default.jpg", i);
            texture.filePathInPack = filePath.c_str();
            texture.stream = [filePath]()
            {
                using namespace io;
                auto& fileSystem = getServiceProvider().get<IFileSystem>();
                auto file = fileSystem.openFile("content/" + filePath, AccessMode::Read, OpenFileMode::OpenExisting);
                return file->createStream();
            };
        }

        io::IMemoryStream::Ptr memoryStream = io::createMemoryStream();
        buildAssetPackage(packData, options, memoryStream).ignore();
        memoryStream->setPosition(io::OffsetOrigin::Begin, 0);

        return {memoryStream};
    }

    Result<> buildAssetPackFromDirectory(std::filesystem::path sourceDirPath, std::filesystem::path targetPackFilePath)
    {
        namespace fs = std::filesystem;

        eastl::vector<PackInputFileData> packContent;

        const auto makePackRelativePath = [&sourceDirPath](const fs::path& inputPath) -> eastl::string
        {
            [[maybe_unused]] std::error_code ec;
            auto fsRelPath = fs::relative(inputPath, sourceDirPath, ec).generic_string();
            std::replace(fsRelPath.begin(), fsRelPath.end(), '\\', '/');

            return eastl::string{"/"} + eastl::string{fsRelPath.data(), fsRelPath.size()};
        };

        for (const auto& dirEntry : fs::recursive_directory_iterator{sourceDirPath})
        {
            if (dirEntry.is_directory())
            {
                continue;
            }

            PackInputFileData& packFile = packContent.emplace_back();
            packFile.filePathInPack = makePackRelativePath(dirEntry.path());
            packFile.stream = [fsPath = dirEntry.path()]() -> io::IStreamReader::Ptr
            {
                return io::createNativeFileStream(fsPath.string().c_str(), io::AccessMode::Read, io::OpenFileMode::OpenExisting);
            };
        }

        if (fs::exists(targetPackFilePath))
        {
            NAU_LOG("Remove existing pack file: ()", targetPackFilePath.string());
            fs::remove(targetPackFilePath);
        }

        auto outputPackStream = io::createNativeFileStream(targetPackFilePath.string().c_str(), io::AccessMode::Write, io::OpenFileMode::CreateAlways);
        NAU_FATAL(outputPackStream);

        const PackBuildOptions packOptions = {

            .contentType = "application/json",
            .version = "0.1",
            .description = "sample pack"

        };

        return buildAssetPackage(packContent, packOptions, outputPackStream);
    }

    void sampleReadAssetPackNative(io::IMemoryStream::Ptr packageStream)
    {
        NAU_ASSERT(packageStream);
        io::AssetPackIndexData assetPackData = *readAssetPackage(packageStream);

        NAU_LOG("Asset pack version: {}", assetPackData.version);
        NAU_LOG("Asset pack description: {}\n", assetPackData.description);
        for (const io::AssetPackFileEntry& content : assetPackData.content)
        {
            NAU_LOG("Content file path: {}", content.filePath);
            NAU_LOG("Content client size: {}", content.clientSize);
            NAU_LOG("Content compression type: {}", content.contentCompression);

            io::BlobData contentData = content.blobData;
            NAU_LOG("Content offset in pack: {}", contentData.offset);
            packageStream->setPosition(io::OffsetOrigin::Begin, contentData.offset);
            NAU_LOG("Content size: {}", contentData.size);
            io::IMemoryStream::Ptr contentStream = io::createMemoryStream(contentData.size);
            NAU_LOG("Content stream size: {}\n", *io::copyFromStream(*contentStream, contentData.size, *packageStream));
        }
    }

    void sampleReadAssetPack()
    {
        std::function<void(io::DirectoryIterator&)> walk;
        walk = [&walk](io::DirectoryIterator& iterator)
        {
            for (const io::FsEntry& entry : iterator)
            {
                auto& fileSystem = getServiceProvider().get<io::IFileSystem>();
                if (entry.kind == io::FsEntryKind::File)
                {
                    io::IFile::Ptr file = fileSystem.openFile(entry.path, io::AccessMode::Read, io::OpenFileMode::OpenExisting);
                    io::IStreamReader::Ptr stream = file->createStream();
                    BytesBuffer buffer(entry.size);
                    NAU_VERIFY(*stream->read(buffer.data(), entry.size) == entry.size);
                }
                else
                {
                    io::DirectoryIterator next(&fileSystem, entry.path);
                    walk(next);
                }
            }
        };

        auto& fileSystem = getServiceProvider().get<io::IFileSystem>();
        io::DirectoryIterator iterator(&fileSystem, "packs/textures");
        walk(iterator);
    }

}  // namespace nau::sample

int main(int argc, char** argv)
{
    namespace fs = std::filesystem;
    using namespace nau;
    using namespace nau::async;
    using namespace std::chrono_literals;

    auto app = nau::createApplication([]
    {
        loadModulesList(NAU_MODULES_LIST).ignore();

        constexpr bool ReadSample = true;

        if (ReadSample)
        {
            sample::configureVirtualFileSystem(getServiceProvider().get<io::IVirtualFileSystem>());
            sample::sampleReadAssetPackNative(*sample::sampleBuildAssetPack());
            sample::sampleReadAssetPack();
        }
        else if (const auto sceneBaseProjectRoot = sample::lookupProjectRootPath("sceneBase"); fs::exists(sceneBaseProjectRoot))
        {
            // Writing pack sample.
            // - lookup sceneBase project's root directory.
            // - collect all files from [sceneBase]/content directory
            // - build [sceneBase]/content.packs/content.nau_pack content package
            const fs::path sceneBaseContentDir = sceneBaseProjectRoot / "content";
            const fs::path sceneBasePacksDir = sceneBaseProjectRoot / "content.packs";
            const fs::path targetPackPath = sceneBasePacksDir / "content.nau_pack";

            if (!fs::exists(sceneBasePacksDir))
            {
                fs::create_directory(sceneBasePacksDir);
            }

            sample::buildAssetPackFromDirectory(sceneBaseContentDir, targetPackPath);

        }
        return ResultSuccess;
    });

    app->startupOnCurrentThread();
    app->stop();

    while (app->step())
    {
        std::this_thread::sleep_for(5ms);
    }

    return 0;
}
