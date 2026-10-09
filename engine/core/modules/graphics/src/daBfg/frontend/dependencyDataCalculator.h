// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "dabfg/frontend/internalRegistry.h"
#include "dabfg/frontend/nameResolver.h"
#include "dabfg/common/graphDumper.h"
#include <EASTL/fixed_vector.h>

#include "dabfg/frontend/dependencyData.h"


namespace dabfg
{

class DependencyDataCalculator
{
public:
  DependencyDataCalculator(InternalRegistry &reg, const NameResolver &nameRes) : registry{reg}, nameResolver{nameRes} {}

  DependencyData depData;

  void recalculate();

private:
  InternalRegistry &registry;
  const NameResolver &nameResolver;

  void recalculateResourceLifetimes();
  void resolveRenaming();
  void updateRenamedResourceProperties();
};

} // namespace dabfg
