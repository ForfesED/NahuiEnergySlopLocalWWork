// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#pragma once


#include <limits.h>
#include <nau/math/math.h>
#include <nau/math/dag_bounds2.h>

namespace nau::math
{

class IBBox2
{
public:
  IVector2 lim[2];

  IBBox2(const IVector2 &lim0, const IVector2 &lim1)
  {
    lim[0] = lim0;
    lim[1] = lim1;
  }
  IBBox2() { setEmpty(); }

  void setEmpty()
  {
    lim[0].setX(INT_MAX);
    lim[0].setY(INT_MAX);
    lim[1].setX(INT_MIN);
    lim[1].setY(INT_MIN);
  }

  bool isEmpty() const { return lim[0].getX() > lim[1].getX() || lim[0].getY() > lim[1].getY(); }

  bool isAreaEmpty() const { return lim[0].getX() >= lim[1].getX() || lim[0].getY() >= lim[1].getY(); }

  const IVector2 &operator[](int i) const { return lim[i]; }
  IVector2 &operator[](int i) { return lim[i]; }

  void add(const IVector2 &p) { add(p.getX(), p.getY()); }
  IBBox2 &operator+=(const IVector2 &p)
  {
    add(p);
    return *this;
  }
  IBBox2 &operator+=(const IBBox2 &b)
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
    return *this;
  }
  void add(int u, int v)
  {
    if (isEmpty())
    {
      lim[0].setX(u);
      lim[0].setY(v);
      lim[1].setX(u);
      lim[1].setY(v);
      return;
    }

    if (u < lim[0].getX())
      lim[0].setX(u);
    if (v < lim[0].getY())
      lim[0].setY(v);

    if (u > lim[1].getX())
      lim[1].setX(u);
    if (v > lim[1].getY())
      lim[1].setY(v);
  }

  void clip(IVector2& uv0, IVector2& uv1) const
  {
    if (uv0.getX() < lim[0].getX())
      uv0.setX(lim[0].getX());
    if (uv0.getY() < lim[0].getY())
      uv0.setY(lim[0].getY());
    if (uv1.getX() > lim[1].getX())
      uv1.setX(lim[1].getX());
    if (uv1.getY() > lim[1].getY())
      uv1.setY(lim[1].getY());
  }
  void clipBox(IBBox2 &b) const { clip(b[0], b[1]); }

  void inflate(int val)
  {
    lim[0] -= IVector2(val, val);
    lim[1] += IVector2(val, val);
  }

  bool operator&(const IVector2 &p) const
  {
    if (p.getX() < lim[0].getX())
      return false;
    if (p.getX() > lim[1].getX())
      return false;
    if (p.getY() < lim[0].getY())
      return false;
    if (p.getY() > lim[1].getY())
      return false;
    return true;
  }
  /// check intersection with box
  bool operator&(const IBBox2 &b) const
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
    return true;
  }

  bool operator==(const IBBox2 &b) const { return b[0] == lim[0] && b[1] == lim[1]; }

  bool operator!=(const IBBox2 &b) const { return b[0] != lim[0] || b[1] != lim[1]; }

  inline IVector2 width() const { return lim[1] - lim[0]; }

  inline int left() const { return lim[0].getX(); }
  inline int right() const { return lim[1].getX(); }
  inline int top() const { return lim[0].getY(); }
  inline int bottom() const { return lim[1].getY(); }
  inline const IVector2 &getMin() const { return lim[0]; }
  inline const IVector2 &getMax() const { return lim[1]; }
  inline IVector2 size() const { return lim[1] - lim[0]; }

  inline const IVector2 &leftTop() const { return lim[0]; }
  inline IVector2 rightTop() const { return IVector2(lim[1].getX(), lim[0].getY()); }
  inline IVector2 leftBottom() const { return IVector2(lim[0].getX(), lim[1].getY()); }
  inline const IVector2 &rightBottom() const { return lim[1]; }
};

inline IBBox2 ibbox2(const BBox2 &p)
{
  return IBBox2(IVector2((int)floorf(p[0].getX()), (int)floorf(p[0].getY())), IVector2((int)ceilf(p[1].getX()), (int)ceilf(p[1].getY())));
}
inline BBox2 bbox2(const IBBox2 &p) { return BBox2(Vector2((real)p[0].getX(), (real)p[0].getY()), Vector2((real)p[1].getX(), (real)p[1].getY())); }

// does not check if box is empty. if min of one box == max of other, returns false (boxes intersect, but not overlap)
__forceinline bool unsafe_overlap(const IBBox2 &a, const IBBox2 &b)
{
  if (b.lim[0].getX() >= a.lim[1].getX())
    return false;
  if (b.lim[1].getX() <= a.lim[0].getX())
    return false;
  if (b.lim[0].getY() >= a.lim[1].getY())
    return false;
  if (b.lim[1].getY() <= a.lim[0].getY())
    return false;
  return true;
}
// check if a is completely inside b
__forceinline bool is_box_inside_other(const IBBox2 &a, const IBBox2 &b)
{
  return a[0].getX() >= b[0].getX() && a[1].getX() <= b[1].getX() && a[0].getY() >= b[0].getY() && a[1].getY() <= b[1].getY();
}


inline unsigned squared_int(int i) { return i * i; }

// returns squared distance to integer point from box. if point is inside box it will be 0
inline unsigned sq_distance_ipoint_to_ibox2(const IVector2 &p, const IBBox2 &box) // TODO: check if it works when point inside box
{
  if (p.getX() < box[0].getX())
  {
    if (p.getY() < box[0].getY())
    {
        IVector2 temp = box[0] - p;
        Vector2 fTemp = {static_cast<float>(temp.getX()), static_cast<float>(temp.getY())};
        return lengthSqr(fTemp);
    }
    if (p.getY() > box[1].getY())
      return lengthSqr(Vector2(box[0].getX(), box[1].getY()) - Vector2(p.getX(), p.getY()));
    return squared_int(box[0].getX() - p.getX());
  }
  if (p.getX() > box[1].getX())
  {
    if (p.getY() < box[0].getY())
      return lengthSqr(Vector2(box[1].getX(), box[0].getY()) - Vector2(p.getX(), p.getY()));
    if (p.getY() > box[1].getY())
    {
        IVector2 temp = box[1] - p;
        Vector2 fTemp = {static_cast<float>(temp.getX()), static_cast<float>(temp.getY())};
        return lengthSqr(fTemp);
    }
    return squared_int(p.getX() - box[1].getX());
  }
  if (p.getY() < box[0].getY())
    return squared_int(box[0].getY() - p.getY());
  if (p.getY() > box[1].getY())
    return squared_int(p.getY() - box[1].getY());
  return 0; // inside
}

}
