// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#define DAGOR_ALLOW_FAST_UNSAFE_TIMERS 0 // define it to 1 locally when going to profile fine-grained code on proper Intel CPU

#include <stdint.h>


#ifdef __cplusplus
extern "C"
{

  // initialize timers
  NAU_RENDER_EXPORT void measure_cpu_freq(bool force_lowres_timer = false);
#endif

  // returns ticks frequency, ticks * 1000000 / ref_ticks_frequency - usec
  NAU_RENDER_EXPORT int64_t ref_ticks_frequency(void);

  // returns reference time label (in ticks!)
  NAU_RENDER_EXPORT int64_t ref_time_ticks(void);
  // returns reference time label relative to base reference time label
  NAU_RENDER_EXPORT int64_t rel_ref_time_ticks(int64_t ref, int time_usec);
  // convert ref time delta to nsecs
  NAU_RENDER_EXPORT int64_t ref_time_delta_to_nsec(int64_t ref);
  // convert ref time delta to usecs
  NAU_RENDER_EXPORT int64_t ref_time_delta_to_usec(int64_t ref);
  // returns time passed since reference time label, in nanoseconds
  NAU_RENDER_EXPORT int64_t get_time_nsec(int64_t ref);
  // returns time passed since reference time label, in microseconds
  NAU_RENDER_EXPORT int get_time_usec(int64_t ref);
  // returns time passed since initialization time in milliseconds
  NAU_RENDER_EXPORT int get_time_msec(void);

// aliases for legacy code
#define ref_time_ticks_qpc ref_time_ticks
#define get_time_usec_qpc  get_time_usec
#define get_time_msec_qpc  get_time_msec

#if _TARGET_PC_LINUX
  // converts milliseconds passed since initialization to wallclock time
  NAU_RENDER_EXPORT int64_t time_msec_to_localtime(int64_t t);
#endif

#ifdef __cplusplus
}
#endif

