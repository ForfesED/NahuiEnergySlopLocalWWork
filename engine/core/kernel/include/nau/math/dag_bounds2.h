// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#pragma once

#include <nau/math/math.h>

#define INLINE __forceinline

namespace nau::math
{

    class BBox2;
    INLINE float non_empty_boxes_not_intersect(const BBox2& a, const BBox2& b);

    using real = float;

    /// @addtogroup math
    /// @{

    // BBox2 - 2D bounding box //
    /**
      2D bounding box
      @sa BBox3 BSphere3 TMatrix TMatrix4 Point3 Vector2 Point4
    */
    class BBox2
    {
    public:
        Vector2 lim[2];
        INLINE BBox2()
        {
            setempty();
        }
        BBox2(const Vector2& a, real s)
        {
            makebox(a, s);
        }
        INLINE BBox2(const Vector2& left_top, const Vector2& right_bottom)
        {
            lim[0] = left_top;
            lim[1] = right_bottom;
        }
        INLINE BBox2(real left, real top, real right, real bottom)
        {
            lim[0] = Vector2(left, top);
            lim[1] = Vector2(right, bottom);
        }

        INLINE void setempty()
        {
            lim[0] = Vector2(FLT_MAX / 4, FLT_MAX / 4);
            lim[1] = Vector2(FLT_MIN / 4, FLT_MIN / 4);
        }
        INLINE bool isempty() const
        {
            return lim[0].getX() > lim[1].getX() || lim[0].getY() > lim[1].getY();
        }
        INLINE void makebox(const Vector2& p, real s)
        {
            Vector2 d(s / 2, s / 2);
            lim[0] = p - d;
            lim[1] = p + d;
        }
        INLINE Vector2 center() const
        {
            return (lim[0] + lim[1]) * 0.5;
        }
        INLINE Vector2 width() const
        {
            return lim[1] - lim[0];
        }

        INLINE const Vector2& operator[](int i) const
        {
            return lim[i];
        }
        INLINE Vector2& operator[](int i)
        {
            return lim[i];
        }
        INLINE operator const Vector2*() const
        {
            return lim;
        }
        INLINE operator Vector2*()
        {
            return lim;
        }

        INLINE float float_is_empty() const
        {
            return nau::math::fsel(lim[1].getX() - lim[0].getX(), 0.0f, 1.0f) + nau::math::fsel(lim[1].getY() - lim[0].getY(), 0.0f, 1.0f);
        }
        INLINE BBox2& operator+=(const Vector2& p)
        {
            lim[0].setX(nau::math::fsel(lim[0].getX() - p.getX(), p.getX(), lim[0].getX()));
            lim[1].setX(nau::math::fsel(p.getX() - lim[1].getX(), p.getX(), lim[1].getX()));
            lim[0].setY(nau::math::fsel(lim[0].getY() - p.getY(), p.getY(), lim[0].getY()));
            lim[1].setY(nau::math::fsel(p.getY() - lim[1].getY(), p.getY(), lim[1].getY()));
            return *this;
        }
        INLINE BBox2& operator+=(const BBox2& b)
        {
            if(b.isempty())
                return *this;
            lim[0].setX(nau::math::fsel(lim[0].getX() - b.lim[0].getX(), b.lim[0].getX(), lim[0].getX()));
            lim[1].setX(nau::math::fsel(b.lim[1].getX() - lim[1].getX(), b.lim[1].getX(), lim[1].getX()));
            lim[0].setY(nau::math::fsel(lim[0].getY() - b.lim[0].getY(), b.lim[0].getY(), lim[0].getY()));
            lim[1].setY(nau::math::fsel(b.lim[1].getY() - lim[1].getY(), b.lim[1].getY(), lim[1].getY()));
            return *this;
        }

        INLINE bool operator&(const Vector2& p) const
        {
            if(p.getX() < lim[0].getX())
                return 0;
            if(p.getX() > lim[1].getX())
                return 0;
            if(p.getY() < lim[0].getY())
                return 0;
            if(p.getY() > lim[1].getY())
                return 0;
            return 1;
        }
        INLINE bool operator&(const BBox2& b) const
        {
            if(b.isempty())
                return 0;
            if(b.lim[0].getX() > lim[1].getX())
                return 0;
            if(b.lim[1].getX() < lim[0].getX())
                return 0;
            if(b.lim[0].getY() > lim[1].getY())
                return 0;
            if(b.lim[1].getY() < lim[0].getY())
                return 0;
            return 1;
        }

        void inflate(float val)
        {
            lim[0] -= Vector2(val, val);
            lim[1] += Vector2(val, val);
        }

        INLINE void scale(float val)
        {
            const Vector2 c = center();
            lim[0] = (lim[0] - c) * val + c;
            lim[1] = (lim[1] - c) * val + c;
        }

        INLINE real left() const
        {
            return lim[0].getX();
        }
        INLINE real right() const
        {
            return lim[1].getX();
        }
        INLINE real top() const
        {
            return lim[0].getY();
        }
        INLINE real bottom() const
        {
            return lim[1].getY();
        }
        INLINE const Vector2& getMin() const
        {
            return lim[0];
        }
        INLINE const Vector2& getMax() const
        {
            return lim[1];
        }
        INLINE Vector2 size() const
        {
            return lim[1] - lim[0];
        }

        INLINE const Vector2& leftTop() const
        {
            return lim[0];
        }
        INLINE Vector2 rightTop() const
        {
            return Vector2(lim[1].getX(), lim[0].getY());
        }
        INLINE Vector2 leftBottom() const
        {
            return Vector2(lim[0].getX(), lim[1].getY());
        }
        INLINE const Vector2& rightBottom() const
        {
            return lim[1];
        }

        template <class T>
        static BBox2 xz(const T& a)
        {
            return BBox2(Vector2(a.lim[0].getX(), a.lim[0].getZ()), Vector2(a.lim[1].getX(), a.lim[1].getZ()));
        }
        template <class T>
        static BBox2 yz(const T& a)
        {
            return BBox2(Vector2(a.lim[0].getY(), a.lim[0].getZ()), Vector2(a.lim[1].getY(), a.lim[1].getZ()));
        }
        template <class T>
        static BBox2 xy(const T& a)
        {
            return BBox2(Vector2(a.lim[0].getX(), a.lim[0].getY()), Vector2(a.lim[1].getX(), a.lim[1].getY()));
        }
    };

    INLINE float non_empty_boxes_not_intersect(const BBox2& a, const BBox2& b)
    {
        return nau::math::fsel(a.lim[1].getX() - b.lim[0].getX(), 0.0f, 1.0f) + nau::math::fsel(b.lim[1].getX() - a.lim[0].getX(), 0.0f, 1.0f) +
               nau::math::fsel(a.lim[1].getY() - b.lim[0].getY(), 0.0f, 1.0f) + nau::math::fsel(b.lim[1].getY() - a.lim[0].getY(), 0.0f, 1.0f);
    }

    INLINE float float_non_empty_boxes_not_inclusive(const BBox2& inner, const BBox2& outer)
    {
        return nau::math::fsel(inner.lim[0].getX() - outer.lim[0].getX(), 0.0f, 1.0f) + nau::math::fsel(outer.lim[1].getX() - inner.lim[1].getX(), 0.0f, 1.0f) +
               nau::math::fsel(inner.lim[0].getY() - outer.lim[0].getY(), 0.0f, 1.0f) + nau::math::fsel(outer.lim[1].getY() - inner.lim[1].getY(), 0.0f, 1.0f);
    };

    INLINE bool non_empty_boxes_inclusive(const BBox2& inner, const BBox2& outer)
    {
        return float_non_empty_boxes_not_inclusive(inner, outer) < 1.0f;
    };

#undef INLINE

    /// @}

}  // namespace nau::math


