// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/threading/thread_safe_annotations.h


#pragma once

#include "nau/kernel/kernel_config.h"


#if defined(NAU_THREADS_ENABLED) && defined(__clang__) 
#define THREAD_ANNOTATION_ATTRIBUTE(x) __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE(x)
#endif

#define THREAD_ANNOTATION(x) THREAD_ANNOTATION_ATTRIBUTE(x)

#define THREAD_CAPABILITY(x) \
    THREAD_ANNOTATION_ATTRIBUTE(capability(x))

#define THREAD_SCOPED_CAPABILITY \
    THREAD_ANNOTATION_ATTRIBUTE(scoped_lockable)

#define THREAD_GUARDED_BY(x) \
    THREAD_ANNOTATION_ATTRIBUTE(guarded_by(x))

#define THREAD_PTR_GUARDED_BY(x) \
    THREAD_ANNOTATION_ATTRIBUTE(pt_guarded_by(x))

#define THREAD_ACQUIRE(...) \
    THREAD_ANNOTATION_ATTRIBUTE(acquire_capability(__VA_ARGS__))

#define THREAD_TRY_ACQUIRE(...) \
    THREAD_ANNOTATION_ATTRIBUTE(try_acquire_capability(__VA_ARGS__))

#define THREAD_RELEASE(...) \
    THREAD_ANNOTATION_ATTRIBUTE(release_capability(__VA_ARGS__))

#define THREAD_ACQUIRE_SHARED(...) \
    THREAD_ANNOTATION_ATTRIBUTE(acquire_shared_capability(__VA_ARGS__))

#define THREAD_TRY_ACQUIRE_SHARED(...) \
    THREAD_ANNOTATION_ATTRIBUTE(try_acquire_shared_capability(__VA_ARGS__))

#define THREAD_RELEASE_SHARED(...) \
    THREAD_ANNOTATION_ATTRIBUTE(release_shared_capability(__VA_ARGS__))

#define THREAD_NO_ANALYSIS \
    THREAD_ANNOTATION_ATTRIBUTE(no_thread_safety_analysis)

#define THREAD_REQUIRES(...) \
    THREAD_ANNOTATION_ATTRIBUTE(requires_capability(__VA_ARGS__))

#define THREAD_REQUIRES_SHARED(...) \
    THREAD_ANNOTATION_ATTRIBUTE(requires_shared_capability(__VA_ARGS__))

#define THREAD_LOCK_RETURNED(x) \
    THREAD_ANNOTATION_ATTRIBUTE(lock_returned(x))

#define THREAD_LOCKS_EXCLUDES(...) \
    THREAD_ANNOTATION_ATTRIBUTE(locks_excluded(__VA_ARGS__))
