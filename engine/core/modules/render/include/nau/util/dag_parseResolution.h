// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <string.h>

#if defined(__cplusplus) && !defined(__GNUC__)
template <typename T, size_t N>
char (&_countof__helper_(T (&array)[N]))[N];
#define countof(x) (sizeof(_countof__helper_(x)))
#else
#define countof(x) (sizeof(x) / sizeof((x)[0]))
#endif
#define c_countof(x) (sizeof(x) / sizeof((x)[0]))

inline bool get_resolution_from_str(const char *s, int &width, int &height)
{
  const char *del = " x";
  char buf[32];
  strncpy(buf, s, countof(buf));
  buf[countof(buf) - 1] = 0;

  char *p = strtok(buf, del);
  if (p == NULL)
    return false;
  width = atoi(p);

  p = strtok(NULL, del);
  if (p == NULL)
    return false;
  height = atoi(p);

  return true;
}
