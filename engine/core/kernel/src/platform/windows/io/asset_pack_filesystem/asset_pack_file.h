// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/io/file_system.h"
#include "nau/io/stream.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/rtti/weak_ptr.h"

namespace nau::io
{
    class AssetPackFileSystemImpl;
    /**
     */
    class AssetPackFile final : public IFile,
                                public io_detail::IFileInternal
    {
        NAU_CLASS_(AssetPackFile, IFile, io_detail::IFileInternal)
    public:
        AssetPackFile(const AssetPackFile&) = delete;
        AssetPackFile(AssetPackFileSystemImpl* fileSystem, size_t offset, size_t size);

        virtual ~AssetPackFile() = default;

        bool supports(FileFeature) const final;

        bool isOpened() const final;

        IStreamBase::Ptr createStream(std::optional<AccessModeFlag>) final;

        AccessModeFlag getAccessMode() const override;

        size_t getSize() const override;

        FsPath getPath() const override;

        void setVfsPath(io::FsPath path) override;

    private:
        FsPath m_vfsPath;
        size_t m_offset = 0;
        size_t m_size = 0;
        WeakPtr<AssetPackFileSystemImpl> m_fileSystemRef;
    };

    /**
     */
    class AssetPackStream : public IStreamReader
    {
        NAU_CLASS_(AssetPackStream, IStreamReader)
    public:
        AssetPackStream(const nau::Ptr<AssetPackFileSystemImpl>& fileSystem, size_t offset, size_t size);
        ~AssetPackStream();

        size_t getPosition() const override;

        size_t setPosition(OffsetOrigin origin, int64_t offset) override;

        Result<size_t> read(std::byte* buffer, size_t size) override;

    private:
        size_t m_offset = 0;
        size_t m_size = 0;
        size_t m_selfPosition = 0;
        WeakPtr<AssetPackFileSystemImpl> m_fileSystemRef;
    };
}  // namespace nau::io
