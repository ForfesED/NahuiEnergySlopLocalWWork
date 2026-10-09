// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/io/file_system.h"

namespace nau::io
{
    DirectoryIterator::DirectoryIterator(IFileSystem::Ptr fs, FsPath path) :
        m_fs(std::move(fs)),
        m_path(std::move(path))
    {
        NAU_ASSERT(m_fs);
        NAU_ASSERT(!m_path.isEmpty());
    }

    DirectoryIterator::~DirectoryIterator()
    {
        if(m_fs && m_iteratorState)
        {
            m_fs->closeDirIterator(m_iteratorState);
        }
    }

    DirectoryIterator::iterator DirectoryIterator::start()
    {
        NAU_ASSERT(!m_iteratorState);
        NAU_ASSERT(m_fs);

        if(m_fs && !m_iteratorState)
        {
            auto openDirResult = m_fs->openDirIterator(m_path);
            if(!openDirResult)
            {
                // report an error
                return {};
            }

            auto [state, firstEntry] = *openDirResult;
            if(state)
            {
                m_iteratorState = state;
                return iterator{*this, std::move(firstEntry)};
            }
        }

        return {};
    }

    FsEntry DirectoryIterator::increment()
    {
        NAU_ASSERT(m_fs);
        NAU_ASSERT(m_iteratorState);

        if(!m_iteratorState)
        {
            return {};
        }

        return m_fs->incrementDirIterator(m_iteratorState);
    }

    DirectoryIterator::iterator::iterator() = default;

    DirectoryIterator::iterator::iterator(iterator&& other) :
        m_parent(other.m_parent)
    {
    }

    DirectoryIterator::iterator::iterator(DirectoryIterator& parent, FsEntry&& firstEntry) :
        m_parent(&parent),
        m_fsEntry(std::move(firstEntry))
    {
    }

    bool DirectoryIterator::iterator::operator==([[maybe_unused]] const iterator& other) const
    {
        NAU_ASSERT(other.isEnd(), "Dir iterator must be compared only with end()");
        return isEnd() && other.isEnd();
    }

    DirectoryIterator::iterator& DirectoryIterator::iterator::operator++()
    {
        NAU_ASSERT(m_parent);

        if(m_fsEntry = m_parent->increment(); !m_fsEntry)
        {
            m_parent = nullptr;
        }

        return *this;
    }

    DirectoryIterator::iterator DirectoryIterator::iterator::operator++(int)
    {
        NAU_ASSERT(m_parent);

        iterator temp;
        temp.m_parent = m_parent;
        temp.m_fsEntry = std::move(m_fsEntry);

        this->operator++();
        return temp;
    }

    const FsEntry& DirectoryIterator::iterator::operator*() const&
    {
        NAU_ASSERT(m_fsEntry);

        return (m_fsEntry);
    }

    FsEntry&& DirectoryIterator::iterator::operator*() &&
    {
        NAU_ASSERT(m_fsEntry);

        return std::move(m_fsEntry);
    }

    const FsEntry* DirectoryIterator::iterator::operator->() const
    {
        return &m_fsEntry;
    }

    bool DirectoryIterator::iterator::isEnd() const
    {
        return m_parent == nullptr;
    }

}  // namespace nau::io
