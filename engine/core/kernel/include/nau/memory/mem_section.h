// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

/**
 * @file mem_section.h
 * @brief Defines the MemSection class for managing a section of memory with multiple aligned pages.
 */
#pragma once

#include <set>
#include "nau/diag/assertion.h"
#include "nau/memory/mem_page.h"

namespace nau
{
    /**
     * @brief Class for managing a section of memory with multiple aligned pages.
     */
    class NAU_KERNEL_EXPORT MemSection final
    {
        friend class HeapAllocator;
    public:

        MemSection() = default;
        ~MemSection();


        MemSection(const MemSection&) = delete;
        MemSection& operator=(const MemSection&) = delete;
        MemSection(MemSection&&) = delete;
        MemSection& operator=(MemSection&&) = delete;

        /**
         * @brief Set the size of the memory pages if the default size (64 KB) is not suitable.
         * @param size The size of the memory pages in bytes.
         */
        void setPageSize(size_t size);

        /**
         * @brief Gets the size of the memory pages.
         * 
         * @return The size of the memory pages in bytes.
         */
        [[nodiscard]] size_t getPageSize() const;

        /**
         * @brief Allocates a block of memory with the specified size and alignment.
         * 
         * @param size The size of the block of memory to allocate.
         * @param alignment The alignment of the block of memory to allocate.
         * @return A pointer to the allocated block of memory.
         */
        [[nodiscard]] void* allocate(size_t size, size_t alignment = 4);

        /**
         * @brief Checks if the given pointer is within the memory section.
         * 
         * @param ptr The pointer to check.
         * @return True if the pointer is within the memory section, false otherwise.
         */
        [[nodiscard]] bool contains(void* ptr) const;

        /**
         * @brief Resets the memory section, making all previously allocated memory available for reuse.
         */
        void reset();

    private:
        using BytePtr = char*;

        MemPage* m_rootPage = nullptr;
        MemPage* m_currentPage = nullptr;
        void* m_free = nullptr;
        size_t m_pageSize = 64 * 1024; // typical L1 cache size in bytes.
        bool m_inWork = false;

        void freeMem();
    };

}