// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/memory/eastl_aliases.h"

#include <nau/memory/frame_allocator.h>
#include <nau/memory/stack_allocator.h>

namespace nau
{
    EastlVectorAllocator::EastlVectorAllocator(const char* name)
    {
    }
    EastlVectorAllocator::EastlVectorAllocator(const EastlVectorAllocator& src)
    {
    }
    EastlVectorAllocator::EastlVectorAllocator(const EastlVectorAllocator& src, const char* name)
    {
    }
    EastlVectorAllocator::~EastlVectorAllocator()
    {
    }

    EastlVectorAllocator& EastlVectorAllocator::operator=(const EastlVectorAllocator& src)
    {
        return *this;
    }

    void* EastlVectorAllocator::allocate(size_t n, int flags)
    {
        return allocator::instance().allocate(n);
    }
    void* EastlVectorAllocator::allocate(size_t n, size_t alignment, size_t offset, int flags)
    {
        return allocator::instance().allocate(n);
    }
    void EastlVectorAllocator::deallocate(void* p, size_t n)
    {
        allocator::instance().deallocate(p);
    }

    const char* EastlVectorAllocator::get_name() const
    {
        return allocator::instance().getName();
    }
    void EastlVectorAllocator::set_name(const char* name)
    {
        allocator::instance().setName(name);
    }

    EastlStackAllocator::EastlStackAllocator(const char* name)
    {
    }
    EastlStackAllocator::EastlStackAllocator(const EastlStackAllocator& src)
    {
    }
    EastlStackAllocator::EastlStackAllocator(const EastlStackAllocator& src, const char* name)
    {
    }
    EastlStackAllocator::~EastlStackAllocator()
    {
    }

    EastlStackAllocator& EastlStackAllocator::operator=(const EastlStackAllocator& src)
    {
        return *this;
    }

    void* EastlStackAllocator::allocate(size_t n, int flags)
    {
        return nau::IStackAllocator::getStackAllocator()->allocate(n);
    }
    void* EastlStackAllocator::allocate(size_t n, size_t alignment, size_t offset, int flags)
    {
        return nau::IStackAllocator::getStackAllocator()->allocate(n);
    }
    void EastlStackAllocator::deallocate(void* p, size_t n)
    {
        nau::IStackAllocator::getStackAllocator()->deallocate(p);
    }

    const char* EastlStackAllocator::get_name() const
    {
        return nau::IStackAllocator::getStackAllocator()->getName();
    }
    void EastlStackAllocator::set_name(const char* name)
    {
        nau::IStackAllocator::getStackAllocator()->setName(name);
    }

    EastlFrameAllocator::EastlFrameAllocator(const char* name)
    {
    }
    EastlFrameAllocator::EastlFrameAllocator(const EastlFrameAllocator& src)
    {
    }
    EastlFrameAllocator::EastlFrameAllocator(const EastlFrameAllocator& src, const char* name)
    {
    }
    EastlFrameAllocator::~EastlFrameAllocator()
    {
    }

    EastlFrameAllocator& EastlFrameAllocator::operator=(const EastlFrameAllocator& src)
    {
        return *this;
    }

    void* EastlFrameAllocator::allocate(size_t n, int flags)
    {
        return nau::IFrameAllocator::getFrameAllocator()->allocate(n);
    }
    void* EastlFrameAllocator::allocate(size_t n, size_t alignment, size_t offset, int flags)
    {
        return nau::IFrameAllocator::getFrameAllocator()->allocate(n);
    }
    void EastlFrameAllocator::deallocate(void* p, size_t n)
    {
        nau::IFrameAllocator::getFrameAllocator()->deallocate(p);
    }

    const char* EastlFrameAllocator::get_name() const
    {
        return nau::IFrameAllocator::getFrameAllocator()->getName();
    }
    void EastlFrameAllocator::set_name(const char* name)
    {
        nau::IFrameAllocator::getFrameAllocator()->setName(name);
    }

}  // namespace nau