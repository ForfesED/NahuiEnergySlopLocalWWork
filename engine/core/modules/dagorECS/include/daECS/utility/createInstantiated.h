// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <daECS/core/entityId.h>
#include <daECS/core/entityManager.h>

namespace ecs
{

class ComponentsInitializer;

EntityId createInstantiatedEntitySync(EntityManager &mgr, const char *name, ComponentsInitializer &&initializer);

} // namespace ecs
