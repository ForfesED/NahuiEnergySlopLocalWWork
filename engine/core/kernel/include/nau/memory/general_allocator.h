// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/memory/array_allocator.h"

namespace nau
{
    /**
     * @brief A memory allocator that uses different memory management strategies based on the requested block size.
     *
     * The GeneralAllocator class provides a memory allocation and deallocation mechanism that
     * uses different memory management strategies depending on the size of the requested block.
     * It uses a combination of fixed-size block allocators and a dynamic array allocator to
     * efficiently manage memory allocations.
     */
    class NAU_KERNEL_EXPORT GeneralAllocator final : public IAlignedAllocatorDebug
    {
    public:
        /**
         * @brief Allocates a block of memory of the specified size.
         * @param size The size of the memory block to be allocated, in bytes.
         * @return A pointer to the allocated memory block.
         */
        [[nodiscard]] void* allocate(size_t size) override;
        /**
         * @brief Reallocates a block of memory to a new size.
         * @param ptr The pointer to the memory block to be reallocated.
         * @param size The new size of the memory block, in bytes.
         * @return A pointer to the reallocated memory block.
         */
        [[nodiscard]] void* reallocate(void* ptr, size_t size) override;
         /**
         * @brief Deallocates a block of memory.
         * @param ptr The pointer to the memory block to be deallocated.
         */
        void deallocate(void* ptr) override;

        /**
         * @brief Retrieves the size of the allocated memory block.
         * @param ptr The pointer to the memory block.
         * @return The size of the allocated memory block, in bytes.
         */
        size_t getSize(const void* ptr) const override;

    private:
        using ArrayAllocator = ArrayAllocator<2048>;
    };

}