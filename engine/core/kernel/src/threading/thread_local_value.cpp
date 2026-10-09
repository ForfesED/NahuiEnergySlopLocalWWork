// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/threading/thread_local_value.h"
#include "nau/diag/assertion.h"

namespace nau
{

    RAIIFunction::RAIIFunction(std::function<void()> construct, std::function<void()> destruct)
        : m_destruct(destruct)
    {
        if (construct)
        {
            construct();
        }
    }
    RAIIFunction::~RAIIFunction()
    {
        if (m_destruct)
        {
            m_destruct();
        }
    }

    uint32_t liveThreadIndex()
    {
        static std::mutex s_sync;
        static std::stack<uint32_t> s_freeIndexPool;
        static uint32_t s_maxIndex = 0;

        thread_local static uint32_t s_threadIndex = std::numeric_limits<uint32_t>::max();
        thread_local static RAIIFunction s_logic = RAIIFunction
        (
            []
            {
                std::lock_guard lock(s_sync);
                s_threadIndex = s_freeIndexPool.empty() ? s_maxIndex++ : s_freeIndexPool.top();
                if (!s_freeIndexPool.empty())
                {
                    s_freeIndexPool.pop();
                }
            },
            []
            {
                std::lock_guard lock(s_sync);
                s_freeIndexPool.push(s_threadIndex);
            }
        );

        NAU_ASSERT(s_threadIndex != std::numeric_limits<uint32_t>::max());
        return s_threadIndex;
    }
}
