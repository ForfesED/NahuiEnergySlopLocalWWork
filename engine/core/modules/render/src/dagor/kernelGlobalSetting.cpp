// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include "nau/perfMon/dag_cpuFreq.h"
#include "nau/startup/dag_globalSettings.h"

#if __GNUC__
#include <pthread.h>
#endif
#include "nau/diag/logging.h"
#include <stdio.h>
#include <string.h>

#include "nau/dataBlock/dag_dataBlock.h"

bool dgs_execute_quiet = false;
void (*dgs_post_shutdown_handler)() = NULL;
void (*dgs_pre_shutdown_handler)() = NULL;
bool (*dgs_fatal_handler)(const char *msg, const char *call_stack, const char *file, int line) = NULL;
void (*dgs_shutdown)() = NULL;
void (*dgs_fatal_report)(const char *msg, const char *call_stack) = NULL;
int (*dgs_fill_fatal_context)(char *buff, int sz, bool terse) = NULL;
void (*dgs_report_fatal_error)(const char *title, const char *msg, const char *call_stack) = NULL;
void (*loading_progress_point_cb)() = NULL;
void (*dgs_on_swap_callback)() = NULL;
void (*dgs_on_dagor_cycle_start)() = NULL;
void (*dgs_on_promoted_log_tag)(int tag, const char *fmt, const void *arg, int anum) = NULL;
void (*dgs_on_thread_enter_cb)(const char *) = nullptr;
void (*dgs_on_thread_exit_cb)() = nullptr;

static const nau::DataBlock *default_get_settings() {
    static const nau::DataBlock defaultSettings;
    return &defaultSettings;
}
const nau::DataBlock *(*dgs_get_settings)() = &default_get_settings;

static const nau::DataBlock *default_get_game_params() { return NULL; }
const nau::DataBlock *(*dgs_get_game_params)() = &default_get_game_params;

bool dagor_demo_mode = false;

WindowMode dgs_window_mode = WindowMode::FULLSCREEN_EXCLUSIVE;
int dagor_frame_no_int = 0;

bool dgs_app_active = true;
unsigned int dgs_last_suspend_at = 0;
unsigned int dgs_last_resume_at = 0;

#if _TARGET_PC | _TARGET_IOS | _TARGET_TVOS | _TARGET_ANDROID
static bool launchedAsDemo = false;
static int idleStartT = 0, demoIdleTimeout = 0;

bool dagor_is_demo_mode() { return launchedAsDemo; }
void dagor_demo_reset_idle_timer() { idleStartT = get_time_msec(); }
void dagor_demo_idle_timer_set_IS(bool) {}
bool dagor_demo_check_idle_timeout()
{
  if (!launchedAsDemo || !demoIdleTimeout || get_time_msec() < idleStartT + demoIdleTimeout)
    return false;
  return true;
}
void dagor_demo_final_quit(const char *) {}

void dagor_force_demo_mode(bool demo, int timeout_ms)
{
  measure_cpu_freq();
  launchedAsDemo = demo;
  demoIdleTimeout = timeout_ms;
  dagor_demo_reset_idle_timer();
  NAU_LOG_DEBUG("force {} mode, timeout={} ms, idleStartT={}", demo ? "DEMO" : "normal", timeout_ms, idleStartT);
}
#endif

int dgs_argc = 0;
char **dgs_argv = NULL;
bool dgs_sse_present = false;
char dgs_cpu_name[128] = "n/a";

bool dgs_trace_inpdev_line = false;


// build timestamp processing
//#include <osApiWrappers/dag_direct.h>
#include <stdio.h>
#include <string.h>

const char *dagor_get_build_stamp_str_ex(char *buf, size_t bufsz, const char *suffix, const char *dagor_exe_build_date,
  const char *dagor_exe_build_time)
{
  if (strcmp(dagor_exe_build_date, "*") == 0)
  {
    char stamp_fn[512];
    if (const char *fext = nullptr)//dd_get_fname_ext(dgs_argv[0]))
      snprintf(stamp_fn, sizeof(stamp_fn), "%.*s-STAMP", int(fext - dgs_argv[0]), dgs_argv[0]);
    else
      snprintf(stamp_fn, sizeof(stamp_fn), "%s-STAMP", dgs_argv[0]);
    if (FILE *fp = fopen(stamp_fn, "rt"))
    {
      if (fgets(stamp_fn, sizeof(stamp_fn), fp) == nullptr)
        stamp_fn[0] = '\0';
      stamp_fn[sizeof(stamp_fn) - 1] = '\0';
      if (char *p = strchr(stamp_fn, '\n'))
        *p = '\0';
      fclose(fp);
      snprintf(buf, bufsz, "BUILD TIMESTAMP:   %s%s", stamp_fn, suffix);
      return buf;
    }
  }
  snprintf(buf, bufsz, "BUILD TIMESTAMP:   %s %s%s", dagor_exe_build_date, dagor_exe_build_time, suffix);
  return buf;
}

#if _TARGET_STATIC_LIB
extern "C" const char *dagor_exe_build_date;
extern "C" const char *dagor_exe_build_time;

extern "C" const char *dagor_get_build_stamp_str(char *buf, size_t bufsz, const char *suffix)
{
  return dagor_get_build_stamp_str_ex(buf, bufsz, suffix, dagor_exe_build_date, dagor_exe_build_time);
}

#endif

