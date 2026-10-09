// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "render/daBfg/detail/resourceType.h"


namespace dabfg
{

using TypeErasedCall = void (*)(void *);

struct BlobDescription
{
  ResourceSubtypeTag typeTag;
  size_t size;
  size_t alignment;
  TypeErasedCall activate;
  TypeErasedCall deactivate;
};

struct BlobView
{
  void *data = nullptr;
  ResourceSubtypeTag typeTag = ResourceSubtypeTag::Invalid;
};

} // namespace dabfg
