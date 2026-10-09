// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <inttypes.h>
//#include <util/dag_globDef.h>
//#include <math/dag_mathBase.h>

#include "driver.h"
#include "util.h"


struct Extent2D
{
  uint32_t width;
  uint32_t height;
};

inline bool operator==(Extent2D l, Extent2D r) { return l.width == r.width && l.height == r.height; }
inline bool operator!=(Extent2D l, Extent2D r) { return !(l == r); }

struct Extent3D
{
  uint32_t width;
  uint32_t height;
  uint32_t depth;

  explicit operator Extent2D() const { return {width, height}; }
};

inline Extent3D operator*(Extent3D l, Extent3D r) { return {l.width * r.width, l.height * r.height, l.depth * r.depth}; }

inline Extent3D operator/(Extent3D l, Extent3D r) { return {l.width / r.width, l.height / r.height, l.depth / r.depth}; }

inline bool operator==(Extent3D l, Extent3D r) { return l.depth == r.depth && static_cast<Extent2D>(l) == static_cast<Extent2D>(r); }

inline bool operator!=(Extent3D l, Extent3D r) { return !(l == r); }

inline Extent3D operator>>(Extent3D value, uint32_t shift)
{
  return {value.width >> shift, value.height >> shift, value.depth >> shift};
}


inline Extent3D max_extent(Extent3D a, Extent3D b) { return {Vectormath::max(a.width, b.width), Vectormath::max(a.height, b.height), Vectormath::max(a.depth, b.depth)}; }

inline Extent3D min_extent(Extent3D a, Extent3D b) { return {Vectormath::min(a.width, b.width), Vectormath::min(a.height, b.height), Vectormath::min(a.depth, b.depth)}; }

inline Extent3D mip_extent(Extent3D value, uint32_t mip) { return max_extent(value >> mip, {1, 1, 1}); }

struct Offset2D
{
  int32_t x;
  int32_t y;
};

inline bool operator==(Offset2D l, Offset2D r) { return l.x == r.x && l.y == r.y; }

inline bool operator!=(Offset2D l, Offset2D r) { return !(l == r); }

struct Offset3D
{
  int32_t x;
  int32_t y;
  int32_t z;

  explicit operator Offset2D() { return {x, y}; }
};

inline bool operator==(Offset3D l, Offset3D r) { return l.z == r.z && static_cast<Offset2D>(l) == static_cast<Offset2D>(r); }

inline bool operator!=(Offset3D l, Offset3D r) { return !(l == r); }

inline Extent3D operator+(Extent3D ext, Offset3D ofs) { return {ext.width + ofs.x, ext.height + ofs.y, ext.depth + ofs.z}; }

inline D3D12_RECT clamp_rect(D3D12_RECT rect, Extent2D ext)
{
  rect.left = Vectormath::clamp<decltype(rect.left)>(rect.left, 0, ext.width);
  rect.right = Vectormath::clamp<decltype(rect.right)>(rect.right, 0, ext.width);
  rect.top = Vectormath::clamp<decltype(rect.top)>(rect.top, 0, ext.height);
  rect.bottom = Vectormath::clamp<decltype(rect.bottom)>(rect.bottom, 0, ext.height);
  return rect;
}

inline const Offset3D &toOffset(const Extent3D &ext)
{
  // sanity checks
  static_assert(offsetof(Extent3D, width) == offsetof(Offset3D, x));
  static_assert(offsetof(Extent3D, height) == offsetof(Offset3D, y));
  static_assert(offsetof(Extent3D, depth) == offsetof(Offset3D, z));
  return reinterpret_cast<const Offset3D &>(ext);
}

inline Extent3D align_value(const Extent3D &value, const Extent3D &alignment)
{
  return //
    {align_value(value.width, alignment.width), align_value(value.height, alignment.height),
      align_value(value.depth, alignment.depth)};
}
