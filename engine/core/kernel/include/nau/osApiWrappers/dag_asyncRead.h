// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <nau/kernel/kernel_config.h>
#include <stdarg.h>
namespace nau::hal
{
    //
    // additional routines for async reading of files
    // all routines call Win32 directly, so there is no prebuffering or caching
    //

#ifdef __cplusplus
    extern "C"
    {
#endif

        // opens real file for reading
        NAU_KERNEL_EXPORT void* dfa_open_for_read(const char* fpath, bool non_cached);
        // closes real file handle
        NAU_KERNEL_EXPORT void dfa_close(void* handle);

        // returns associated file sector size (uses path of file)
        NAU_KERNEL_EXPORT unsigned dfa_chunk_size(const char* fname);

        // returns associated file size
        NAU_KERNEL_EXPORT int dfa_file_length(void* handle);

        // allocates handle for async read operation
        NAU_KERNEL_EXPORT int dfa_alloc_asyncdata();
        // deallocates handle, allocated with dfa_alloc_asyncdata;
        // must not be called before async read completion
        NAU_KERNEL_EXPORT void dfa_free_asyncdata(int data_handle);

        // places request to read asynchronously data from real file; returns false on failure
        NAU_KERNEL_EXPORT bool dfa_read_async(void* handle, int asyncdata_handle, int offset, void* buf, int len);
        // checks for async read completion
        NAU_KERNEL_EXPORT bool dfa_check_complete(int asyncdata_handle, int* read_len);

#ifdef __cplusplus
    }
#endif
}  // namespace nau::hal