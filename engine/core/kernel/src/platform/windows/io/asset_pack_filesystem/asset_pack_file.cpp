// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "./asset_pack_file.h"

#include "./asset_pack_file_system.h"
#include "nau/io/memory_stream.h"

namespace nau::io
{
    AssetPackFile::AssetPackFile(AssetPackFileSystemImpl* fileSystem, size_t offset, size_t size) :
        m_offset(offset),
        m_size(size),
        m_fileSystemRef(nau::Ptr{fileSystem})
    {
        NAU_FATAL(fileSystem);
        NAU_FATAL(m_fileSystemRef);
    }

    bool AssetPackFile::supports(FileFeature feature) const
    {
        return false;
    }

    bool AssetPackFile::isOpened() const
    {
        return true;
    }

    IStreamBase::Ptr AssetPackFile::createStream([[maybe_unused]] std::optional<AccessModeFlag> accessMode)
    {
        auto fileSystem = m_fileSystemRef.lock();
        NAU_ASSERT(fileSystem);

        return rtti::createInstance<AssetPackStream>(fileSystem, m_offset, m_size);
    }

    size_t AssetPackFile::getSize() const
    {
        return m_size;
    }

    FsPath AssetPackFile::getPath() const
    {
        return m_vfsPath;
    }

    void AssetPackFile::setVfsPath(io::FsPath path)
    {
        m_vfsPath = std::move(path);
    }

    AccessModeFlag AssetPackFile::getAccessMode() const
    {
        return AccessMode::Read;
    }

    AssetPackStream::AssetPackStream(const nau::Ptr<AssetPackFileSystemImpl>& fileSystem, size_t offset, size_t size) :
        m_fileSystemRef(fileSystem),
        m_offset(offset),
        m_size(size)
    {
        NAU_FATAL(fileSystem);
        NAU_FATAL(m_fileSystemRef);

        fileSystem->notifyStreamCreated(m_offset, m_size);
    }

    AssetPackStream::~AssetPackStream()
    {
        if (auto fileSystem = m_fileSystemRef.lock())
        {
            fileSystem->notifyStreamRemoved(m_offset, m_size);
        }
    }

    size_t AssetPackStream::getPosition() const
    {
        return m_selfPosition;
    }

    size_t AssetPackStream::setPosition(OffsetOrigin origin, int64_t offset)
    {
        int64_t newPos = offset;
        const int64_t currentSize = static_cast<int64_t>(m_size);

        if (origin == OffsetOrigin::Current)
        {
            newPos = static_cast<int64_t>(m_selfPosition) + offset;
        }
        else if (origin == OffsetOrigin::End)
        {
            newPos = currentSize + offset;
        }
#ifdef NAU_ASSERT_ENABLED
        else
        {
            NAU_ASSERT(origin == OffsetOrigin::Begin);
        }
#endif

        if (newPos < 0)
        {
            newPos = 0;
        }
        else if (currentSize < newPos)
        {
            newPos = currentSize;
        }

        NAU_FATAL(newPos >= 0);
        NAU_FATAL(newPos <= currentSize);

        m_selfPosition = static_cast<size_t>(newPos);
        return m_selfPosition;
    }

    Result<size_t> AssetPackStream::read(std::byte* buffer, size_t size)
    {
        NAU_FATAL(m_selfPosition <= m_size);

        const size_t availableSize = m_size - m_selfPosition;
        const size_t actualReadCount = std::min(availableSize, size);

        if (actualReadCount == 0)
        {
            return 0;
        }

        size_t alreadyRead = 0;
        size_t readOffset = m_selfPosition + m_offset;
        size_t readCount = actualReadCount;
        size_t writeOffset = 0;

        auto fileSystem = m_fileSystemRef.lock();
        NAU_FATAL(fileSystem);

        while (alreadyRead < actualReadCount)
        {
            const auto& [ptr, availSize] = fileSystem->requestRead(readOffset, readCount);
            alreadyRead += availSize;

            size_t realRead = std::min(availSize, actualReadCount);
            memcpy(buffer + writeOffset, reinterpret_cast<std::byte*>(ptr), realRead);
            writeOffset += realRead;
            readOffset += realRead;
            m_selfPosition += realRead;
            readCount -= realRead;
        }

        return actualReadCount;
    }
}  // namespace nau::io
