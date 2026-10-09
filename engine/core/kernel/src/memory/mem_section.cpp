// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/memory/mem_section.h"

namespace nau
{
    MemSection::~MemSection()
    {
        freeMem();
    }
    
    void MemSection::freeMem()
    {
        auto it = m_rootPage;
        while (it)
        {
            auto next = it->getNext();
            MemPage::freeMemPage(it);
            it = next;
        }
        m_rootPage = nullptr;
    }

    void MemSection::setPageSize(size_t size)
    {
        m_pageSize = size;
    }
    
    size_t MemSection::getPageSize() const
    {
        return m_pageSize;
    }


    void* MemSection::allocate(size_t size, size_t alignment)
    {
        auto pageSize = std::max(size, m_pageSize);
        if (!m_rootPage)
        {
            m_currentPage = m_rootPage = MemPage::allocateMemPage(pageSize, alignment);
            m_free = m_currentPage->getAddress();
            NAU_ASSERT(m_free, "MemSection memory allocation failed");
        }
        else while (!m_currentPage->contains(BytePtr(m_free) + size))
        {
            if(m_currentPage->getNext())
            {
                m_currentPage = m_currentPage->getNext();
                m_free = m_currentPage->getAddress();
            }
            else
            {
                auto newPage = MemPage::allocateMemPage(pageSize, alignment);
                m_currentPage->setNext(newPage);
                m_currentPage = newPage;
                m_free = m_currentPage->getAddress();
                NAU_ASSERT(m_free, "MemSection memory allocation failed");
            }
        }

        auto p = m_free;
        m_free = static_cast<BytePtr>(m_free) + size;
        return p;
    }
    
    bool MemSection::contains(void* ptr) const
    {
        if (!ptr)
            return false;

        auto it = m_rootPage;
        while (it)
        {
            if (it->contains(ptr))
                return true;
            it = it->getNext();
        }
        return false;
    }

    void MemSection::reset()
    {
        m_currentPage = m_rootPage;
        m_free = m_currentPage->getAddress();    
    }

}