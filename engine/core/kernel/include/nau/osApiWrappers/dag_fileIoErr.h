// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/kernel/kernel_config.h"

namespace nau::hal
{

    //! if installed, is called on each successful file open operation
    //! (can be used to bind fname and file_handle to resolve filename in other callbacks)
    extern NAU_KERNEL_EXPORT void (*dag_on_file_open)(const char* fname, void* file_handle, int flags);
    //! if installed, is called on each file close operation
    extern NAU_KERNEL_EXPORT void (*dag_on_file_close)(void* file_handle);

    //! if installed, is called on each file after it was deleted
    extern NAU_KERNEL_EXPORT void (*dag_on_file_was_erased)(const char* fname);

    //! if installed, is called on each failure to open file
    extern NAU_KERNEL_EXPORT void (*dag_on_file_not_found)(const char* fname);

    //! dagor cricital error callcack called on read file error;
    //! if installed, must return false to report IO fail or true to re-try reading
    extern NAU_KERNEL_EXPORT bool (*dag_on_read_beyond_eof_cb)(void* file_handle, int ofs, int len, int read);

    //! dagor cricital error callcack called on read file error;
    //! if installed, must return false to report IO fail or true to re-try reading
    extern NAU_KERNEL_EXPORT bool (*dag_on_read_error_cb)(void* file_handle, int ofs, int len);

    //! dagor cricital error callcack called on write file error;
    //! if installed, must return false to report IO fail or true to re-try writing
    extern NAU_KERNEL_EXPORT bool (*dag_on_write_error_cb)(void* file_handle, int ofs, int len);

    //! if installed, is called before attempt to open file
    //! if it returns false, file is handled as non-existing
    extern NAU_KERNEL_EXPORT bool (*dag_on_file_pre_open)(const char* fname);

    //! called on unpacking errors in zlib
    extern NAU_KERNEL_EXPORT void (*dag_on_zlib_error_cb)(const char* fname, int error);

    //! can be called when assets failed to load and the game can't run after it
    //! when installed should show system message box on android and ios and quit app after
    extern NAU_KERNEL_EXPORT void (*dag_on_assets_fatal_cb)(const char* asset_name);
}  // namespace nau::hal