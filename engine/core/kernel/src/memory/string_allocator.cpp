// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/memory/string_allocator.h"
#include "nau/memory/fixed_blocks.h"

namespace nau
{
    StringAllocator& StringAllocator::instance()
    {
        static StringAllocator inst;
        return inst;
    }

    [[nodiscard]] char* StringAllocator::allocate(unsigned int size)
    {
        if (size <= 16) return static_cast<char*>(FixedBlocksAllocator<16>::instance().allocate(size));
        else if (size <= 32) return static_cast<char*>(FixedBlocksAllocator<32>::instance().allocate(size));
        else if (size <= 64) return static_cast<char*>(FixedBlocksAllocator<64>::instance().allocate(size));
        else if (size <= 128) return static_cast<char*>(FixedBlocksAllocator<128>::instance().allocate(size));
        else if (size <= 256) return static_cast<char*>(FixedBlocksAllocator<256>::instance().allocate(size));
        else return static_cast<char*>(ArrayAllocator::instance().allocate(size));
    }

    void StringAllocator::deallocate(const char* str, size_t len)
    {
        if (len <= 16) FixedBlocksAllocator<16>::instance().deallocate(reinterpret_cast<void*>(const_cast<char*>(str)));
        else if (len <= 32) FixedBlocksAllocator<32>::instance().deallocate(reinterpret_cast<void*>(const_cast<char*>(str)));
        else if (len <= 64) FixedBlocksAllocator<64>::instance().deallocate(reinterpret_cast<void*>(const_cast<char*>(str)));
        else if (len <= 128) FixedBlocksAllocator<128>::instance().deallocate(reinterpret_cast<void*>(const_cast<char*>(str)));
        else if (len <= 256) FixedBlocksAllocator<256>::instance().deallocate(reinterpret_cast<void*>(const_cast<char*>(str)));
        else ArrayAllocator::instance().deallocate(reinterpret_cast<void*>(const_cast<char*>(str)));
    }
}