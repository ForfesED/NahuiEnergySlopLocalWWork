// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include <filesystem>

#include "nau/io/virtual_file_system.h"

namespace nau
{
    void configureVirtualFileSystem(io::IVirtualFileSystem& vfs)
    {
        namespace fs = std::filesystem;

        const auto projectContentDir = EXPR_Block->fs::path
        {
            const fs::path contentRelativePath{L"samples/networkTransport/content"};
            fs::path currentPath = fs::current_path();

            do
            {
                auto targetPath = currentPath / contentRelativePath;
                if (fs::exists(targetPath))
                {
                    return fs::canonical(targetPath);
                }

                currentPath = currentPath.parent_path();

            } while (currentPath.has_relative_path());

            return {};
        };

        auto contentFs = io::createNativeFileSystem(projectContentDir.string());
        vfs.mount("/content", std::move(contentFs)).ignore();
    }
}  // namespace nau
