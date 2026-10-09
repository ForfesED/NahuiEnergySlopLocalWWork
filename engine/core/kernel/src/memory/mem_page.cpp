// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/memory/mem_page.h"
#include "nau/memory/mem_allocator.h"
#include "nau/diag/assertion.h"
#include <cstddef>

namespace nau
{
    [[nodiscard]]
    bool MemPage::contains(void* address) const
    {
        return (m_address <= address) && (address <= (static_cast<BytePtr>(m_address) + m_size));
    }

    [[nodiscard]] size_t MemPage::getSize() const { return m_size; }
    [[nodiscard]] size_t MemPage::getAlignedSize() const { return m_alligned; }
    [[nodiscard]] MemPage* MemPage::getNext() const { return m_next; }
    [[nodiscard]] void* MemPage::getAddress() const { return m_address; }
    
    MemPage::MemPage(size_t size, size_t alignment)
    {
        m_size = size;
        m_alligned = alignment;
        m_next = nullptr;
        auto mask = ~(m_alligned - 1);
        m_address = reinterpret_cast<void*>
        (
            reinterpret_cast<uintptr_t>
            (
                reinterpret_cast<BytePtr>(this) + sizeof(MemPage) + m_alligned - 1
            ) & mask
        );
    }

    [[nodiscard]] MemPage* MemPage::allocateMemPage(size_t size, size_t alignment)
    {
        NAU_ASSERT(isPowerOf2(alignment), "requested alignment is not a power of 2");

        alignment = (alignment < alignof(std::max_align_t)) ? alignof(std::max_align_t) : alignment;
        auto pageSize = sizeof(MemPage) + size + alignment - 1;
        auto page = new(malloc(pageSize)) MemPage(size, alignment);
        NAU_ASSERT(page, "MemPage page allocation failed");

        return page;
    }

    void MemPage::freeMemPage(MemPage* page)
    {
        page->~MemPage();
        free(page);
    }
}
