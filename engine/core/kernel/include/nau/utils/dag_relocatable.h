// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#pragma once


#include <EASTL/type_traits.h>

namespace dag
{
  template <typename T, typename = void>
  struct is_type_relocatable : public eastl::false_type {};

  template <typename T>
  struct is_type_init_constructing : public eastl::true_type {};
}

#define DAG_DECLARE_RELOCATABLE(C) template<> struct dag::is_type_relocatable<C> : public eastl::true_type {}
