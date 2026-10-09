// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/memory/aligned_allocator.h"


namespace nau
{
    /**
     * @brief IAlignedAllocator with memory overrun detection
     */
    class NAU_KERNEL_EXPORT IAlignedAllocatorDebug : public IAlignedAllocator
    {
    public:

        IAlignedAllocatorDebug()
        {
            m_name.value() = "IAlignedAllocatorDebug";
        }

        virtual ~IAlignedAllocatorDebug()
        {
        }

        /**
         * @brief Allocates a block of aligned memory.
         *
         * @param size The size of memory to allocate.
         * @param alignment The alignment to use.
         * @return A pointer to the allocated memory.
         */
        [[nodiscard]] virtual void* allocateAligned(size_t size, size_t alignment) override;

        /**
         * @brief Deallocates a block of aligned memory.
         *
         * @param ptr The pointer to the memory to deallocate.
         */
        virtual void deallocateAligned(void* ptr) override;

        /**
         * @brief Check, if block is not damaged
         *
         * @param ptr The pointer to the memory block.
         * @return Block state
         */
        [[nodiscard]] virtual bool isValid(const void* ptr) const override;

    private:

        const static uint32_t Pattern = 0xDEADBEEF;

        static void fillPattern(void* aligned, AllocationInfo& info);
        static bool checkPattern(const void* aligned, AllocationInfo& info);
    };

} // namespace nau
