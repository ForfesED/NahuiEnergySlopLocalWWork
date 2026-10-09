// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/string/string.h"

class D3dResource
{
public:
  virtual void destroy() = 0;
  virtual int restype() const = 0;
  virtual int ressize() const = 0;
  const char8_t* getResName() const { return statName.c_str(); }
  void setResName(const char8_t *name) { statName = name ? name : u8""; } // TODO: add assert on name
  // WARNING: This might allocate. Avoid calling it every frame.
  virtual void setResApiName(const char * /*name*/) const {}

protected:
  virtual ~D3dResource(){};

private:
  nau::string statName;
};

enum
{
  RES3D_TEX,
  RES3D_CUBETEX,
  RES3D_VOLTEX,
  RES3D_ARRTEX,
  RES3D_CUBEARRTEX,
  RES3D_SBUF
};

inline void destroy_d3dres(D3dResource *res) { return res ? res->destroy() : (void)0; }
template <class T>
inline void del_d3dres(T *&p)
{
  destroy_d3dres(p);
  p = nullptr;
}
