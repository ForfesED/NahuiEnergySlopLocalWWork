// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <nau/core_defines.h>
#include <nau/kernel/kernel_config.h>
#include <stdint.h>
// #include <osApiWrappers/dag_lockProfiler.h>
// #include <nau/perfMon/dag_daProfilerToken.h>

#if _TARGET_SIMD_SSE
    #include <emmintrin.h>  // _mm_pause
#endif

namespace nau::hal
{

#define SPINS_BEFORE_SLEEP 8192

    //! causes sleep for given time in msec (for Win32 uses Sleep)
    NAU_KERNEL_EXPORT void sleep_msec(int time_msec);

    inline void sleep_msec_ex(int ms)
    {
        sleep_msec(ms);
    }

    //! causes sleep for given time in usec (for Win32 uses Sleep, which has 1 msec precision, using rounding!)
    // this is not sleep_usec_precise by any means
    NAU_KERNEL_EXPORT void sleep_usec(uint64_t time_usec);

//! signals to the processor that the thread is doing nothing. Not relevant to OS thread scheduling.
#if _TARGET_SIMD_SSE
    #define cpu_yield _mm_pause
#elif defined(__arm__) || defined(__aarch64__)
    #define cpu_yield() __asm__ __volatile__("yield")
#else
    #define cpu_yield() ((void)0)
#endif

//! returns true when current thread is main thread
#if _TARGET_STATIC_LIB
    extern thread_local bool tls_is_main_thread;
    inline bool is_main_thread()
    {
        return tls_is_main_thread;
    }
#else
    NAU_KERNEL_EXPORT bool is_main_thread();
#endif

    NAU_KERNEL_EXPORT void init_main_thread_id();

    //! returns ID main thread
    NAU_KERNEL_EXPORT int64_t get_main_thread_id();
    //! returns ID current thread
    NAU_KERNEL_EXPORT int64_t get_current_thread_id();

    //! terminates process (for Win32 uses TerminateProcess)
    NAU_KERNEL_EXPORT void terminate_process(int code);

    //! convert IP address (as in struct in_addr) to string (in xxx.yyy.zzz.www format)
    //! uses inet_ntoa (winsock2)
    //! Uses static buffer for conversion! not thread-safe, can't be used twice in same printf
    NAU_KERNEL_EXPORT const char* ip_to_string(unsigned int ip);

    //! convert string (in xxx.yyy.zzz.www format) to number for struct in_addr
    //! uses inet_addr (winsock2)
    NAU_KERNEL_EXPORT unsigned int string_to_ip(const char* str);

    //! returns process unique ID (different for 2 simultaneously running processes)
    NAU_KERNEL_EXPORT int get_process_uid();

    //! structure to represent date/time
    struct DagorDateTime
    {
        unsigned short year;       // exact number (different range on different platforms)
        unsigned short month;      // [1..12]
        unsigned short day;        // [1..31]
        unsigned short hour;       // [0..23]
        unsigned short minute;     // [0..59]
        unsigned short second;     // [0..59], but on some platforms could be also 60 (for leap seconds)
        unsigned int microsecond;  // [0..999999], but also could be always 0 on some platforms
    };

    NAU_KERNEL_EXPORT int get_local_time(DagorDateTime* outTime);  // return zero on success

    enum TargetPlatform : uint8_t
    {
        // Change carefully, it's already stored in database
        TP_UNKNOWN = 0,
        TP_WIN32 = 1,
        TP_WIN64 = 2,
        TP_IOS = 3,
        TP_ANDROID = 4,
        TP_MACOSX = 5,
        TP_PS3 = 6,  // dropped
        TP_PS4 = 7,
        TP_XBOX360 = 8,  // dropped
        TP_LINUX64 = 9,
        TP_LINUX32 = 10,  // dropped
        TP_XBOXONE = 11,
        TP_XBOX_SCARLETT = 12,
        TP_TVOS = 13,
        TP_NSWITCH = 14,
        TP_PS5 = 15,
        TP_TOTAL
    };

    constexpr TargetPlatform get_platform_id()
    {
        TargetPlatform platform =
#if NAU_PLATFORM_WIN32
    #if NAU_64BIT
            TP_WIN64
    #else
            TP_WIN32
    #endif
#elif _TARGET_IOS
            TP_IOS
#elif _TARGET_ANDROID
            TP_ANDROID
#elif _TARGET_PC_MACOSX
            TP_MACOSX
#elif _TARGET_C1

#elif _TARGET_PC_LINUX
            TP_LINUX64
#elif _TARGET_XBOXONE
            TP_XBOXONE
#elif _TARGET_SCARLETT
            TP_XBOX_SCARLETT
#elif _TARGET_TVOS
            TP_TVOS
#elif _TARGET_C3

#elif _TARGET_C2

#else
    #error "Undefined platform type"
            TP_UNKNOWN
#endif
            ;
        return platform;
    }

    NAU_KERNEL_EXPORT const char* get_platform_string_by_id(TargetPlatform id);

    NAU_KERNEL_EXPORT TargetPlatform get_platform_id_by_string(const char* name);

    //! returns string that uniquely identifies platform (that target was build for)
    NAU_KERNEL_EXPORT const char* get_platform_string_id();

    //! flash window in the taskbar if inactive // Removed, shouldn't be here
    // NAU_KERNEL_EXPORT void flash_window(void *wnd_handle = NULL);

#if NAU_PLATFORM_WIN32
    //! replacement of GetVersionEx that is working on all versions from Windows XP to Windows 10 build 1511.
    typedef struct _OSVERSIONINFOEXW OSVERSIONINFOEXW;
    NAU_KERNEL_EXPORT bool get_version_ex(OSVERSIONINFOEXW* osversioninfo);
#endif

    // Returns true if the current process is being debugged (either
    // running under the debugger or has a debugger attached post facto).
    NAU_KERNEL_EXPORT bool is_debugger_present();

    // Returns real Windows version when run in compatibility mode for older Windows version.
    NAU_KERNEL_EXPORT bool detect_os_compatibility_mode(char* os_real_name = NULL, size_t os_real_name_size = 0);

    enum class ConsoleModel
    {
        UNKNOWN = 0,
        PS4,
        PS4_PRO,
        XBOXONE,
        XBOXONE_S,
        XBOXONE_X,
        XBOX_LOCKHART,
        XBOX_ANACONDA,
        PS5,
        NINTENDO_SWITCH,
        TOTAL
    };

    NAU_KERNEL_EXPORT ConsoleModel get_console_model();
    NAU_KERNEL_EXPORT const char* get_console_model_revision(ConsoleModel model);

    template <typename F>
    __forceinline void spin_wait_no_profile(F keep_waiting_cb)
    {
        int spinsLeftBeforeSleep = SPINS_BEFORE_SLEEP;
        while(keep_waiting_cb())
        {
            if(--spinsLeftBeforeSleep > 0)
                cpu_yield();
            else if(spinsLeftBeforeSleep > -SPINS_BEFORE_SLEEP / 8)
                sleep_usec(0);
            else
            {
                sleep_usec(1000);
                spinsLeftBeforeSleep = 0;
            }
        }
    }

    template <typename F>
    inline void spin_wait(F keep_waiting_cb, uint32_t token = /*TODO: da_profiler::DescSleep*/ 0, uint32_t threshold_us = 1)
    {
        // lock_start_t reft = dagor_lock_profiler_start();
        spin_wait_no_profile(keep_waiting_cb);
        // dagor_lock_profiler_stop(reft, token, threshold_us);
    }

}  // namespace nau::hal