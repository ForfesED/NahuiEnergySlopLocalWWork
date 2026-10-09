// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include <malloc.h>

#include "nau/memory/mem_allocator.h"
#include "nau/threading/thread_local_value.h"

namespace nau
{
    /**
     * @brief IMemAllocator partial implemantation for align allocations
     * Using build in Win API aligned allocators
     */
    class IAlignedAllocatorWindows : public IMemAllocator
    {
    public:

        virtual ~IAlignedAllocatorWindows()
        {
        }

        /**
         * @brief Allocates a block of aligned memory.
         *
         * @param size The size of memory to allocate.
         * @param alignment The alignment to use.
         * @return A pointer to the allocated memory.
         */
        [[nodiscard]] virtual void* allocateAligned(size_t size, size_t alignment) override
        {
            return _aligned_malloc(size, alignment);
        }

        /**
         * @brief Reallocates a block of aligned memory.
         *
         * @param ptr The pointer to the previously allocated memory.
         * @param size The new size of memory to allocate.
         * @return A pointer to the reallocated memory.
         */
        [[nodiscard]] virtual void* reallocateAligned(void* ptr, size_t size, size_t alignment) override
        {
            return _aligned_realloc(ptr, size, alignment);
        }

        /**
         * @brief Deallocates a block of aligned memory.
         *
         * @param ptr The pointer to the memory to deallocate.
         */
        virtual void deallocateAligned(void* ptr) override
        {
            _aligned_free(ptr);
        }

        /**
         * @brief Gets the size of a previously allocated block of memory.
         *
         * @param ptr The pointer to the memory block.
         * @return The size of the memory block.
         */
        [[nodiscard]] virtual size_t getSizeAligned(const void* ptr, size_t alignment) const override
        {
            return _aligned_msize(const_cast<void*>(ptr), alignment, 0);
        }

        /**
         * @brief Check, is block allocated by allocateAligned
         *
         * @param ptr The pointer to the memory block.
         * @return Is block allocated as aligned
         */
        [[nodiscard]] virtual bool isAligned(const void* ptr) const override
        {
            return true;
        }

        /**
         * @brief Check, if block is not damaged
         *
         * @param ptr The pointer to the memory block.
         * @return Block state
         */
        [[nodiscard]] virtual bool isValid(const void* ptr) const override
        {
            return true;
        }

        /**
         * @brief Get allocator name
         *
         * @return A pointer to the char* name string.
         */
        virtual const char* getName() const override
        {
            static const char* name = "IAlignedAllocatorWindows";
            return name;
        }

        /**
         * @brief Get allocator name
         *
         * @param name The pointer to the char* name string.
         */
        virtual void setName(const char* name) override
        {
        }
    };

} // namespace nau
