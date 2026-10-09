// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once



NAU_RENDER_EXPORT void enable_float_exceptions(bool enable);
NAU_RENDER_EXPORT bool is_float_exceptions_enabled();

class FloatingPointExceptionsKeeper
{
public:
  FloatingPointExceptionsKeeper()
  {
    savedFE = is_float_exceptions_enabled();
    if (savedFE)
      enable_float_exceptions(false);
  }

  ~FloatingPointExceptionsKeeper()
  {
    if (savedFE)
      enable_float_exceptions(true);
  }

protected:
  bool savedFE;
};

