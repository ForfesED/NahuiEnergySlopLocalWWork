// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/memory/singleton_memop.h

/**
 * @file singleton_memop.h
 * @brief Provides a mechanism to enforce singleton allocation and deallocation for a class.
 */

#pragma once
#include "nau/diag/assertion.h"

namespace nau::rt_detail
{
    /**
     * @brief Singleton memory operations helper.
     * 
     * This template struct provides customized new and delete operators to enforce singleton allocation
     * for the specified class type. It ensures that only one instance of the class can be allocated.
     *
     * @tparam Class The class type for which singleton memory operations are to be provided.
     */
    template <typename Class>
    struct SingletonMemOp
    {
        /**
         * @brief Allocates memory for a singleton instance.
         * 
         * This method overrides the global new operator to allocate memory for a singleton instance.
         * It ensures that the size of the allocated memory is not greater than the size of the storage.
         *
         * @param size The size of the memory to be allocated.
         * @return A pointer to the allocated memory.
         */
        static void* operator_new([[maybe_unused]] size_t size) noexcept
        {
            decltype(auto) state = getSingletonState();
            NAU_ASSERT(size <= sizeof(Storage));
            NAU_ASSERT(!state.allocated, "Singleton already allocated");
            state.allocated = true;
            return &state.storage;
        }

        /**
         * @brief Deallocates memory for a singleton instance.
         * 
         * This method overrides the global delete operator to deallocate memory for a singleton instance.
         * It ensures that the memory being deallocated was previously allocated by this class.
         *
         * @param ptr The pointer to the memory to be deallocated.
         * @param size The size of the memory to be deallocated (ignored).
         */
        static void operator_delete(void* ptr, size_t) noexcept
        {
            decltype(auto) state = getSingletonState();
            NAU_ASSERT(state.allocated);
            state.allocated = false;
        }

    private:
        using Storage = std::aligned_storage_t<sizeof(Class), alignof(Class)>;

        static auto& getSingletonState()
        {
            static struct
            {
                Storage storage;
                bool allocated = false;
            } state = {};
            return (state);
        }
    };

}  // namespace nau::rt_detail


/**
 * @brief Macro to declare singleton memory operations for a class.
 * 
 * This macro defines custom new and delete operators for the specified class to enforce singleton allocation.
 * Use the NAU_DECLARE_SINGLETON_MEMOP macro within your class definition to enable singleton memory operations.
 * class MySingletonClass
 * {
 *      NAU_DECLARE_SINGLETON_MEMOP(MySingletonClass)
 * private:
 *      MySingletonClass() = default;
 *      ~MySingletonClass() = default;
 * public:
 *      static MySingletonClass& getInstance()
 *      {
 *          static MySingletonClass instance;
 *          return instance;
 *      }
 * };
 * @param ClassName The name of the class for which singleton memory operations are to be declared.
 */
#define NAU_DECLARE_SINGLETON_MEMOP(ClassName)                                                   \
public:                                                                                          \
    static void* operator new (size_t size)                                                      \
    {                                                                                            \
        static_assert(!std::is_abstract_v<ClassName>, "Using singleton memop on abstract type"); \
        return ::nau::rt_detail::SingletonMemOp<ClassName>::operator_new(size);                  \
    }                                                                                            \
                                                                                                 \
    static void operator delete (void* ptr, size_t size)                                         \
    {                                                                                            \
        ::nau::rt_detail::SingletonMemOp<ClassName>::operator_delete(ptr, size);                 \
    }
