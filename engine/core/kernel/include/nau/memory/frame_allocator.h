// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

/**
 * @file frame_allocator.h
 * @brief Definition of the IFrameAllocator and FrameAllocator classes.
 */
#pragma once

#include "nau/memory/mem_allocator.h"
#include "nau/memory/mem_section_ptr.h"
#include "nau/memory/aligned_allocator_debug.h"
#include "nau/threading/thread_local_value.h"
#include "nau/rtti/rtti_impl.h"

namespace nau
{

    /**
     * @brief Interface for a frame allocator. 
     * 
     * Provides an interface for frame-based memory allocation.
     */
    class NAU_KERNEL_EXPORT IFrameAllocator : public IAlignedAllocatorDebug
    {
    public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~IFrameAllocator() {}

        /**
         * @brief Prepares the allocator for a new frame.
         * 
         * @return true if preparation is successful.
         * @return false otherwise.
         */
        [[nodiscard]] virtual bool prepareFrame() = 0;

        /**
         * @brief Sets the global frame allocator.
         * 
         * @param allocator Pointer to the frame allocator.
         */
        static void setFrameAllocator(IFrameAllocator* allocator);

        /**
         * @brief Gets the global frame allocator.
         * 
         * @return IFrameAllocator* Pointer to the frame allocator.
         */
        [[nodiscard]] static IFrameAllocator* getFrameAllocator();

    private:
        /**
         * @brief Gets the reference to the global frame allocator pointer.
         * 
         * @return IFrameAllocator*& Reference to the frame allocator pointer.
         */
        [[nodiscard]] static IFrameAllocator*& globalAllocator();
    };


    /**
     * @brief FrameAllocator is a linear growth memory allocator for frame-based  memory management. 
     * It provides efficient allocation and deallocation of memory within the scope of a frame, 
     * making it suitable for scenarios where memory is allocated and freed in a predictable pattern.
     * Provides efficient memory allocation within a frame
     * 
     * Before using the FrameAllocator, you need to set it as the global frame allocator:
     * FrameAllocator frameAllocator;
     * IFrameAllocator::setFrameAllocator(&frameAllocator);
     */
    class NAU_KERNEL_EXPORT FrameAllocator final : public IFrameAllocator
    {
    public:
        /**
         * @brief Constructs a new FrameAllocator object.
         */
        FrameAllocator();
        /**
         * @brief Destroys the FrameAllocator object.
         */
        ~FrameAllocator();

        // Disable copy and move operations
        FrameAllocator(const FrameAllocator&) = delete;
        FrameAllocator& operator=(const FrameAllocator&) = delete;
        FrameAllocator(FrameAllocator&&) = delete;
        FrameAllocator& operator=(FrameAllocator&&) = delete;

        /**
         * @brief Prepares the allocator for a new frame. Not thread safe
         * Call the prepareFrame method at the beginning of each frame to reset the allocator for new allocations:
         * 
         * @return true if preparation is successful.
         * @return false otherwise.
         */
        [[nodiscard]] bool prepareFrame() override;

        /**
         * @brief Allocates memory of the given size. Thread safe
         * 
         * @param size Size of the memory to allocate.
         * @return void* Pointer to the allocated memory.
         */
        [[nodiscard]] void* allocate(size_t size) override;

        /**
         * @brief Reallocates memory to a new size.
         * 
         * @param ptr Pointer to the existing memory block.
         * @param size New size for the memory block.
         * @return void* Pointer to the reallocated memory block.
         */
        [[nodiscard]] void* reallocate(void* ptr, size_t size) override;

        /**
         * @brief Deallocates the memory block at the given pointer.
         * 
         * @param ptr Pointer to the memory block to deallocate.
         */
        void deallocate(void* ptr) override;

        /**
         * @brief Gets the size of the allocated memory block.
         * 
         * @param ptr Pointer to the memory block.
         * @return size_t Size of the memory block.
         */
        virtual size_t getSize(const void* ptr) const override;

    private:

        ThreadLocalValue<MemSectionPtr> m_memSection;
        ThreadLocalValue<int> m_numAllocs;
    };
}


/**
 * @brief Macro for creating a new frame-based allocation.  
 * It provides a convenient way to allocate memory using the global frame allocator.
 */
#define frame_new unique_new(*nau::IFrameAllocator::getFrameAllocator())