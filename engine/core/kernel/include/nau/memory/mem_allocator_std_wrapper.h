// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once
#include "nau/memory/mem_allocator.h"

namespace nau
{
    /**
     * @brief Standard allocator wrapper for the NAU memory allocator.
     * Using with STL Containers:
     * nau::IMemAllocator::Ptr customAllocator = // get your allocator;
     * std::vector<int, nau::MemAllocatorStdWrapper<int>> myVector(customAllocator);
     * @tparam T The type of the objects to allocate.
     */
    template <typename T>
    class MemAllocatorStdWrapper
    {
    public:
        using value_type = T;
        using propagate_on_container_move_assignment = std::true_type;

        /**
         * @brief Constructs the wrapper with the given allocator.
         *
         * @param alloc The allocator instance.
         */
        MemAllocatorStdWrapper(IMemAllocator::Ptr alloc) noexcept :
            m_allocator(std::move(alloc))
        {
            NAU_ASSERT(m_allocator);
        }

        // move is forbidden: because original (other) allocator should always refer to valid allocator (will use it to free its own state)
        MemAllocatorStdWrapper(const MemAllocatorStdWrapper<T>& other) noexcept :
            m_allocator(other.m_allocator)
        {
            NAU_ASSERT(m_allocator);
        }

        template <typename U,
                  std::enable_if_t<!std::is_same_v<U, T>, int> = 0>
        MemAllocatorStdWrapper(const MemAllocatorStdWrapper<U>& other) noexcept :
            m_allocator(other.m_allocator)
        {
            NAU_ASSERT(m_allocator);
        }

        MemAllocatorStdWrapper<T>& operator=([[maybe_unused]] const MemAllocatorStdWrapper<T>& other) noexcept
        {
            NAU_ASSERT(m_allocator.get() == other.m_allocator.get());
            return *this;
        }

        template <typename U,
                  std::enable_if_t<!std::is_same_v<U, T>, int> = 0>
        MemAllocatorStdWrapper<T>& operator=([[maybe_unused]] const MemAllocatorStdWrapper<U>& other) noexcept
        {
            NAU_ASSERT(m_allocator.get() == other.m_allocator.Get());
            return *this;
        }

        /**
         * @brief Allocates memory for the given number of objects.
         *
         * @param n The number of objects to allocate.
         * @return Pointer to the allocated memory.
         */
        [[nodiscard]]
        constexpr T* allocate(size_t n)
        {
            void* const ptr = m_allocator->allocate(sizeof(T) * n);
            return static_cast<T*>(ptr);
        }

        /**
         * @brief Deallocates memory for the given number of objects.
         *
         * @param p Pointer to the memory to deallocate.
         * @param n The number of objects to deallocate.
         */
        void deallocate(T* p, [[maybe_unused]] std::size_t n)
        {
            m_allocator->deallocate(static_cast<void*>(p));
        }

        template <typename U>
        bool operator==(const MemAllocatorStdWrapper<U>& other) const
        {
            return m_allocator.get() == other.get();
        }

        const IMemAllocator::Ptr m_allocator;
    };

}  // namespace nau

