// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#include <nau/core_defines.h>
#include <nau/osApiWrappers/dag_rwLock.h>

#if NAU_PLATFORM_WIN32 | _TARGET_XBOX
    #include <windows.h>
namespace nau::hal
{
    void os_rwlock_init(os_rwlock_t& lock)
    {
        InitializeSRWLock((SRWLOCK*)&lock);
    }
    void os_rwlock_destroy(os_rwlock_t&)
    {
    }  // no-op
    void os_rwlock_acquire_read_lock(os_rwlock_t& lock)
    {
        AcquireSRWLockShared((SRWLOCK*)&lock);
    }
    bool os_rwlock_try_acquire_read_lock(os_rwlock_t& lock)
    {
        return TryAcquireSRWLockShared((SRWLOCK*)&lock);
    }
    void os_rwlock_release_read_lock(os_rwlock_t& lock)
    {
        ReleaseSRWLockShared((SRWLOCK*)&lock);
    }
    void os_rwlock_acquire_write_lock(os_rwlock_t& lock)
    {
        AcquireSRWLockExclusive((SRWLOCK*)&lock);
    }
    bool os_rwlock_try_acquire_write_lock(os_rwlock_t& lock)
    {
        return TryAcquireSRWLockExclusive((SRWLOCK*)&lock);
    }
    void os_rwlock_release_write_lock(os_rwlock_t& lock)
    {
        ReleaseSRWLockExclusive((SRWLOCK*)&lock);
    }
}  // namespace nau::hal
#else  // pthread-based implementation assumed
    #include <errno.h>
    #include <util/dag_globDef.h>

void os_rwlock_init(os_rwlock_t& lock)
{
    int r = pthread_rwlock_init(&lock, /*attr*/ NULL);
    NAU_ASSERT(r == 0, "{:#x}", r);
    // G_UNUSED(r);
}
void os_rwlock_destroy(os_rwlock_t& lock)
{
    int r = pthread_rwlock_destroy(&lock);
    NAU_ASSERT(r == 0, "{:#x}", r);
    // G_UNUSED(r);
}
void os_rwlock_acquire_read_lock(os_rwlock_t& lock)
{
    int r = pthread_rwlock_rdlock(&lock);
    NAU_ASSERT(r == 0, "{:#x}", r);
    // G_UNUSED(r);
}
bool os_rwlock_try_acquire_read_lock(os_rwlock_t& lock)
{
    int r = pthread_rwlock_tryrdlock(&lock);
    NAU_ASSERT(r == 0 || r == EBUSY, "{:#x}", r);
    return r == 0;
}
void os_rwlock_release_read_lock(os_rwlock_t& lock)
{
    int r = pthread_rwlock_unlock(&lock);
    NAU_ASSERT(r == 0, "{:#x}", r);
    // G_UNUSED(r);
}
void os_rwlock_acquire_write_lock(os_rwlock_t& lock)
{
    int r = pthread_rwlock_wrlock(&lock);
    NAU_ASSERT(r == 0, "{:#x}", r);
    // G_UNUSED(r);
}
bool os_rwlock_try_acquire_write_lock(os_rwlock_t& lock)
{
    int r = pthread_rwlock_trywrlock(&lock);
    NAU_ASSERT(r == 0 || r == EBUSY, "{:#x}", r);
    return r == 0;
}
void os_rwlock_release_write_lock(os_rwlock_t& lock)
{
    int r = pthread_rwlock_unlock(&lock);
    NAU_ASSERT(r == 0, "{:#x}", r);
    // G_UNUSED(r);
}

#endif
