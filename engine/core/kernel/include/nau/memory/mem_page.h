// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

/**
 * @file mem_page.h
 * @brief Defines the MemPage class for managing memory pages with specific alignment.
 */

#pragma once

#include "nau/kernel/kernel_config.h"
#include <stdlib.h>

namespace nau
{
    /**
     * @brief Class for managing memory pages with specific alignment.
     * The MemPage class provides a mechanism to manage memory pages with specific alignment requirements. 
     * This is useful in low-level memory management scenarios where precise control over memory alignment is necessary.
     */
    class NAU_KERNEL_EXPORT MemPage final
    {
    public:
        /**
         * @brief Checks if the given address is within the memory page.
         * 
         * @param address The address to check.
         * @return True if the address is within the memory page, false otherwise.
         */
        [[nodiscard]] bool contains(void* address) const;
        /**
         * @brief Gets the size of the memory page.
         * 
         * @return The size of the memory page.
         */
        [[nodiscard]] size_t getSize() const;

        /**
         * @brief Gets the aligned size of the memory page.
         * 
         * @return The aligned size of the memory page.
         */
        [[nodiscard]] size_t getAlignedSize() const;

        /**
         * @brief Gets the next memory page in the linked list.
         * 
         * @return Pointer to the next memory page.
         */
        [[nodiscard]] MemPage* getNext() const;

        /**
         * @brief Gets the starting address of the memory page.
         * 
         * @return The starting address of the memory page.
         */
        [[nodiscard]] void* getAddress() const;

        /**
         * @brief Sets the next memory page in the linked list.
         * 
         * @param next Pointer to the next memory page.
         */
        void setNext(MemPage* next) { m_next = next; }

        /**
         * @brief Allocates a new memory page with the specified size and alignment.
         * 
         * @param size The size of the memory page.
         * @param alignment The alignment of the memory page.
         * @return Pointer to the newly allocated memory page.
         */
        [[nodiscard]] static MemPage* allocateMemPage(size_t size, size_t alignment = alignof(std::max_align_t));

        /**
         * @brief Frees a memory page.
         * 
         * @param page The memory page to free.
         */
        static void freeMemPage(MemPage* page);

    private:
        using BytePtr = char*;

        MemPage(size_t size, size_t alignment = 1);
        ~MemPage() = default;

        size_t m_size = 0;
        size_t m_alligned = 0;           
        MemPage* m_next = nullptr;
        void *m_address = nullptr;
    };
}


