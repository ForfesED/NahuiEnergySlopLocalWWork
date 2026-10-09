// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "typesAndLimits.h"
#include <daECS/core/ecsHash.h>

namespace ecs
{
// load time component list - to allow faster get
struct NAU_DAGORECS_EXPORT LTComponentList
{
  LTComponentList(const HashedConstString name_, const ecs::component_t type_, const char *file = "", const char *fn = "", int line_ = 0) :
    name(name_.hash),
    type(type_)
#if NAU_DEBUG
    ,
    line(line_)
#endif
  {
    next = tail;
    tail = this;
    (void)(file);
    (void)(line_);
    (void)(fn);
#if NAU_DEBUG
    nameStr = name_.str;
    fileStr = file;
    fnStr = fn;
#endif
  }
  __forceinline component_index_t getCidx() const { return info.cidx; }
  __forceinline FastGetInfo getInfo() const { return info; }
  __forceinline component_t getName() const { return name; }
  __forceinline const char *getNameStr() const
  {
#if NAU_DEBUG
    return nameStr;
#else
    return NULL;
#endif
  }

protected:
  LTComponentList(const LTComponentList &) = delete;
  LTComponentList &operator=(const LTComponentList &) = delete;
  const component_t name = 0;
  FastGetInfo info;
  const component_type_t type = 0;
#if NAU_DEBUG
  const char *nameStr = 0;
  const char *fileStr = 0;
  const char *fnStr = 0;
  const int line = 0;
#endif
  LTComponentList *next = 0;
  static LTComponentList *tail;
  friend struct DataComponents;
  friend class EntityManager;
};

}; // namespace ecs