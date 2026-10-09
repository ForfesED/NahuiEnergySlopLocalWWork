// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

/**
 * @file string_allocator.h
 * @brief This file contains the implementation of a StringAllocator class which 
 * is used to allocate and deallocate memory for strings of varying sizes.
 */
#pragma once

#include "nau/memory/array_allocator.h"

namespace nau
{
    class NAU_KERNEL_EXPORT StringAllocator
    {
        using ArrayAllocator = ArrayAllocator<512>;
    public:
        /**
         * @brief Returns the singleton instance of the StringAllocator class.
         */
        static StringAllocator& instance();
        
        /**
         * @brief Allocates memory for a string of the specified size.
         * @param size The size of the string to allocate memory for.
         * @return A pointer to the allocated memory.
         */
        [[nodiscard]] char* allocate(unsigned int size);

        /**
         * @brief Deallocates memory for a string of the specified length.
         * @param str A pointer to the memory to deallocate.
         * @param len The length of the string.
         */
        void deallocate(const char* str, size_t len);
    };

}