// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/threading/lock_guard.h


#pragma once

#include <mutex>
#include <shared_mutex>

#include "nau/threading/thread_safe_annotations.h"
#include "nau/utils/preprocessor.h"

namespace nau::threading
{

    template <typename T>
    class THREAD_SCOPED_CAPABILITY LockGuard : protected std::lock_guard<T>
    {
    public:
        explicit LockGuard(T& mutex) THREAD_ACQUIRE(mutex) :
            std::lock_guard<T>(mutex)
        {
        }

        explicit LockGuard(T& mutex, std::adopt_lock_t) THREAD_REQUIRES(mutex) :
            std::lock_guard<T>(mutex, std::adopt_lock)
        {
        }

        ~LockGuard() THREAD_RELEASE() = default;
    };

}  // namespace nau::threading

// clang-format off
#define lock_(Mutex) \
    ::nau::threading::LockGuard ANONYMOUS_VAR(lock_mutex_) {Mutex}

#define shared_lock_(Mutex) \
    ::std::shared_lock ANONYMOUS_VAR(lock_mutex_) {Mutex}

// clang-format on