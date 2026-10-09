// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/memory/stack_allocator.h"

namespace nau
{
    struct AllocatorsChain
    {
        AllocatorsChain* priv = nullptr;
        IStackAllocator* allocator = nullptr;

        AllocatorsChain() = default;
        ~AllocatorsChain() = default;
        AllocatorsChain(const AllocatorsChain&) = delete;
        AllocatorsChain(AllocatorsChain&&) = delete;
        AllocatorsChain& operator=(const AllocatorsChain&) = delete;
        AllocatorsChain& operator=(AllocatorsChain&&) = delete;

    private:
        friend class IStackAllocator;

        AllocatorsChain(bool base)
        {
            allocator = new StackAllocator<1024>();
        }

        void reset()
        {
            if (!priv)
            {
                delete allocator;
                allocator = new StackAllocator<1024>();
            }
        }
    };

    void IStackAllocator::setStackAllocator(IStackAllocator* allocator)
    {
        AllocatorsChain* newNode = new AllocatorsChain;
        newNode->allocator = allocator;
        newNode->priv = globalAllocator();

        globalAllocator() = newNode;
    }
    
    IStackAllocator* IStackAllocator::getStackAllocator()
    {
        return globalAllocator() ? globalAllocator()->allocator : nullptr;
    }

    void IStackAllocator::releaseStackAllocator()
    {
        auto allocator = globalAllocator();
        if (auto priv = allocator->priv)
        {
            delete allocator;
            globalAllocator() = priv;
        }
        else
            allocator->reset();
    }
    
    AllocatorsChain*& IStackAllocator::globalAllocator()
    {
        static thread_local AllocatorsChain* allocator = new AllocatorsChain(true);
        thread_local static RAIIFunction s_logic = RAIIFunction
        (
            nullptr,
            []
            {
                while (allocator)
                {
                    auto priv = allocator->priv;
                    delete allocator;
                    allocator = priv;
                }
            }
        );

        return allocator;
    }
}