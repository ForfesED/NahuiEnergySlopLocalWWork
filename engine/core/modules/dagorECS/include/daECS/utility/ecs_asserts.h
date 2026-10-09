// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once
#include "nau/diag/assertion.h"
#include "nau/diag/logging.h"

#define logerr(...) NAU_LOG_ERROR({ "ecs" }, ##__VA_ARGS__)
#define logwarn(...) NAU_LOG_WARNING({"ecs"}, ##__VA_ARGS__)
#define logmessage(level, ...) NAU_LOG_MESSAGE(level)({"ecs"}, ##__VA_ARGS__)
#define ECS_VERBOSE_LOG(...) NAU_LOG_VERBOSE({"ecs"}, ##__VA_ARGS__)
#define ECS_LOG(...) NAU_LOG_INFO({"ecs"}, ##__VA_ARGS__)
#define DA_PROFILE_TAG(...)
#define TIME_PROFILE(...)
#define FRAMEMEM_REGION
#define TIME_PROFILE_DEV(...)
#define TIME_PROFILE_WAIT_DEV(...)
#define TIME_PROFILER_SHUTDOWN(...)
#if NAU_DEBUG
#define DAECS_VALIDATE_ARCHETYPE(archetypeId) \
  NAU_ASSERT((archetypeId) == INVALID_ARCHETYPE || (archetypeId) < archetypes.size(), "{}", archetypeId)
#define DAECS_EXT_FAST_ASSERT     NAU_FAST_ASSERT
#define DAECS_EXT_ASSERT          NAU_ASSERT
#define DAECS_EXT_ASSERTF         NAU_ASSERT
#define DAECS_EXT_ASSERT_RETURN   NAU_ASSERT_RETURN
#define DAECS_EXT_ASSERTF_RETURN  NAU_ASSERT_RETURN
#define DAECS_EXT_ASSERT_CONTINUE NAU_ASSERT_CONTINUE
#else
#define DAECS_VALIDATE_ARCHETYPE(archetypeId)
#define DAECS_EXT_FAST_ASSERT(...)
#define DAECS_EXT_ASSERT(...)
#define DAECS_EXT_ASSERTF(...)
#define DAECS_EXT_ASSERT_RETURN(...)
#define DAECS_EXT_ASSERTF_RETURN(...)
#define DAECS_EXT_ASSERT_CONTINUE(...)
#endif
