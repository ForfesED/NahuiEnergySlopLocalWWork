// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "EASTL/list.h"
#include "EASTL/map.h"
#include "EASTL/set.h"
#include "EASTL/unordered_map.h"
#include "EASTL/unordered_set.h"
#include "EASTL/vector.h"
#include "nau/memory/fixed_blocks.h"
#include "nau/memory/string_allocator.h"

namespace nau
{

    class NAU_KERNEL_EXPORT EastlStackAllocator
    {
    public:
        EastlStackAllocator(const char* name = "NAU");
        EastlStackAllocator(const EastlStackAllocator& src);
        EastlStackAllocator(const EastlStackAllocator& src, const char* name);

        ~EastlStackAllocator();

        EastlStackAllocator& operator=(const EastlStackAllocator& src);

        void* allocate(size_t n, int flags = 0);
        void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0);
        void deallocate(void* p, size_t n);

        const char* get_name() const;
        void set_name(const char* name);

    protected:
    };

    class NAU_KERNEL_EXPORT EastlFrameAllocator
    {
    public:
        EastlFrameAllocator(const char* name = "NAU");
        EastlFrameAllocator(const EastlFrameAllocator& src);
        EastlFrameAllocator(const EastlFrameAllocator& src, const char* name);

        ~EastlFrameAllocator();

        EastlFrameAllocator& operator=(const EastlFrameAllocator& src);

        void* allocate(size_t n, int flags = 0);
        void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0);
        void deallocate(void* p, size_t n);

        const char* get_name() const;
        void set_name(const char* name);

    protected:
    };

    class NAU_KERNEL_EXPORT EastlVectorAllocator
    {
        using allocator = ArrayAllocator<1024 * 1024>;

    public:
        EastlVectorAllocator(const char* name = "NAU");
        EastlVectorAllocator(const EastlVectorAllocator& src);
        EastlVectorAllocator(const EastlVectorAllocator& src, const char* name);

        ~EastlVectorAllocator();

        EastlVectorAllocator& operator=(const EastlVectorAllocator& src);

        void* allocate(size_t n, int flags = 0);
        void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0);
        void deallocate(void* p, size_t n);

        const char* get_name() const;
        void set_name(const char* name);

    protected:
    };

    template <size_t BlockSize>
    class EastlBlockAllocator
    {
        using Allocator = FixedBlocksAllocator<BlockSize>;

    public:
        EastlBlockAllocator(const char* name = "NAU")
        {
        }
        EastlBlockAllocator(const EastlBlockAllocator& src)
        {
        }
        EastlBlockAllocator(const EastlBlockAllocator& src, const char* name)
        {
        }
        ~EastlBlockAllocator()
        {
        }

        EastlBlockAllocator& operator=(const EastlBlockAllocator& src)
        {
            return *this;
        }

        void* allocate(size_t n, [[maybe_unused]] int flags = 0)
        {
            return Allocator::instance().allocate(n);
        }
        void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0)
        {
            return Allocator::instance().allocate(n);
        }
        void deallocate(void* p, size_t n)
        {
            Allocator::instance().deallocate(p);
        }

        const char* get_name() const
        {
            return nullptr;
        }
        void set_name(const char* name)
        {
        }

    protected:
    };

    template <class T>
    class EastlBlockAllocatorTyped : public EastlBlockAllocator<sizeof(T)>
    {
        using Base = EastlBlockAllocator<sizeof(T)>;

    public:
        using Base::Base;

        EastlBlockAllocatorTyped& operator=([[maybe_unused]] const EastlBlockAllocatorTyped& src)
        {
            return *this;
        }
    };

    template <typename T>
    using Vector = eastl::vector<T, EastlVectorAllocator>;
    template <typename T>
    using StackVector = eastl::vector<T, EastlStackAllocator>;
    template <typename T>
    using FrameVector = eastl::vector<T, EastlFrameAllocator>;

    template <typename Key, typename T, typename Compare = eastl::less<Key>>
    using Map = eastl::map<Key, T, Compare, EastlBlockAllocatorTyped<typename eastl::map<Key, T, Compare>::node_type>>;
    template <typename Key, typename T, typename Compare = eastl::less<Key>>
    using StackMap = eastl::map<Key, T, Compare, EastlStackAllocator>;
    template <typename Key, typename T, typename Compare = eastl::less<Key>>
    using FrameMap = eastl::map<Key, T, Compare, EastlFrameAllocator>;

    template <typename T>
    using List = eastl::list<T, EastlBlockAllocatorTyped<typename eastl::list<T>::node_type>>;

}  // namespace nau
