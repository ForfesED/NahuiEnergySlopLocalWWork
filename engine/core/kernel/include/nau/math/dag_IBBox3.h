// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#pragma once

//#include "dag_IVector3.h"
#include <limits.h>
#include <nau/math/math.h>
#include <nau/math/dag_bounds3.h>

namespace nau::math
{
class IBBox3
{
public:
  IVector3 lim[2];

  IBBox3() { setEmpty(); }
  IBBox3(const IVector3 &lim0, const IVector3 &lim1)
  {
    lim[0] = lim0;
    lim[1] = lim1;
  }

  void setEmpty()
  {
    lim[0] = {INT_MAX,INT_MAX,INT_MAX};
    lim[1] = {INT_MAX,INT_MAX,INT_MAX};
  }

  bool isEmpty() const { return lim[0].getX() > lim[1].getX() || lim[0].getY() > lim[1].getY() || lim[0].getZ() > lim[1].getZ(); }

  bool isVolumeEmpty() const { return lim[0].getX() >= lim[1].getX() || lim[0].getY() >= lim[1].getY() || lim[0].getZ() >= lim[1].getZ(); }

  const IVector3 &operator[](int i) const { return lim[i]; }
  IVector3 &operator[](int i) { return lim[i]; }

  void add(const IVector3 &p) { add(p.getX(), p.getY(), p.getZ()); }
  IBBox3 &operator+=(const IVector3 &p)
  {
    add(p);
    return *this;
  }
  IBBox3 &operator+=(const IBBox3 &b)
  {
    if (b.isEmpty())
      return *this;
    if (b.lim[0].getX() < lim[0].getX())
      lim[0].setX(b.lim[0].getX());
    if (b.lim[1].getX() > lim[1].getX())
      lim[1].setX(b.lim[1].getX());
    if (b.lim[0].getY() < lim[0].getY())
      lim[0].setY(b.lim[0].getY());
    if (b.lim[1].getY() > lim[1].getY())
      lim[1].setY(b.lim[1].getY());
    if (b.lim[0].getZ() < lim[0].getZ())
      lim[0].setZ(b.lim[0].getZ());
    if (b.lim[1].getZ() > lim[1].getZ())
      lim[1].setZ(b.lim[1].getZ());
    return *this;
  }
  void add(int u, int v, int w)
  {
    if (isEmpty())
    {
      lim[0].setX(u);
      lim[0].setY(v);
      lim[0].setZ(w);
      lim[1].setX(u);
      lim[1].setY(v);
      lim[1].setZ(w);
      return;
    }

    if (u < lim[0].getX())
      lim[0].setX(u);
    if (v < lim[0].getY())
      lim[0].setY(v);
    if (w < lim[0].getZ())
      lim[0].setZ(w);

    if (u > lim[1].getX())
      lim[1].setX(u);
    if (v > lim[1].getY())
      lim[1].setY(v);
    if (w > lim[1].getZ())
      lim[1].setZ(w);
  }

  void clip(IVector3& u, IVector3& v) const
  {
    if (u.getX() < lim[0].getX())
      u.setX(lim[0].getX());
    if (u.getY() < lim[0].getY())
      u.setY(lim[0].getY());
    if (u.getZ() < lim[0].getZ())
      u.setZ(lim[0].getZ());
    if (v.getX() > lim[1].getX())
      v.setX(lim[1].getX());
    if (v.getY() > lim[1].getY())
      v.setY(lim[1].getY());
    if (v.getZ() > lim[1].getZ())
      v.setZ(lim[1].getZ());
  }

  void clipBox(IBBox3 &b) const { clip(b[0], b[1]); }

  void inflate(int val)
  {
    lim[0] -= IVector3(val, val, val);
    lim[1] += IVector3(val, val, val);
  }

  bool operator&(const IVector3 &p) const
  {
    if (p.getX() < lim[0].getX())
      return false;
    if (p.getX() > lim[1].getX())
      return false;
    if (p.getY() < lim[0].getY())
      return false;
    if (p.getY() > lim[1].getY())
      return false;
    if (p.getZ() < lim[0].getZ())
      return false;
    if (p.getZ() > lim[1].getZ())
      return false;
    return true;
  }
  /// check intersection with box
  bool operator&(const IBBox3 &b) const
  {
    if (b.isEmpty())
      return false;
    if (b.lim[0].getX() > lim[1].getX())
      return false;
    if (b.lim[1].getX() < lim[0].getX())
      return false;
    if (b.lim[0].getY() > lim[1].getY())
      return false;
    if (b.lim[1].getY() < lim[0].getY())
      return false;
    if (b.lim[0].getZ() > lim[1].getZ())
      return false;
    if (b.lim[1].getZ() < lim[0].getZ())
      return false;
    return true;
  }

  bool operator==(const IBBox3 &b) const { return b[0] == lim[0] && b[1] == lim[1]; }

  bool operator!=(const IBBox3 &b) const { return b[0] != lim[0] || b[1] != lim[1]; }

  inline IVector3 width() const { return lim[1] - lim[0]; }
};


inline IBBox3 ibbox3(const BBox3 &p)
{
  return IBBox3(IVector3((int)floorf(p[0].getX()), (int)floorf(p[0].getY()), (int)floorf(p[0].getZ())),
    IVector3((int)ceilf(p[1].getX()), (int)ceilf(p[1].getY()), (int)ceilf(p[1].getZ())));
}
inline BBox3 bbox3(const IBBox3 &p)
{
  return BBox3(Vector3((float)p[0].getX(), (float)p[0].getY(), (float)p[0].getZ()), Vector3((float)p[1].getX(), (float)p[1].getY(), (float)p[1].getZ()));
}

}
