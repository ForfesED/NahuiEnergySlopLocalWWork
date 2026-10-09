// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once


#include <daECS/core/entityManager.h>

namespace ecs
{
struct NestedQueryRestorer
{
  int nestedQuery;

  NestedQueryRestorer(const ecs::EntityManager *mgr) : nestedQuery(!mgr->isConstrainedMTMode() ? mgr->getNestedQuery() : -1) {}
  void restore(ecs::EntityManager *mgr)
  {
    if (nestedQuery >= 0)
      mgr->setNestedQuery(nestedQuery);
  }
};

}; // namespace ecs
