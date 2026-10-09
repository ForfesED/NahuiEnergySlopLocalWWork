// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/kernel/kernel_config.h"
#include "nau/math/math.h"

namespace nau::render
{
    typedef unsigned int uint;
    typedef nau::math::Matrix4 float4x4;

    struct HlslUint2
    {
        uint x, y;
        NAU_FORCE_INLINE HlslUint2()
        {
        }  // -V730
        NAU_FORCE_INLINE HlslUint2(uint ax, uint ay) :
            x(ax),
            y(ay)
        {
        }
        NAU_FORCE_INLINE uint& operator[](int i)
        {
            return (&x)[i];
        }
        NAU_FORCE_INLINE const uint& operator[](int i) const
        {
            return (&x)[i];
        }
    };

    struct HlslUint3
    {
        uint x, y, z;
        NAU_FORCE_INLINE HlslUint3()
        {
        }  // -V730
        NAU_FORCE_INLINE HlslUint3(uint ax, uint ay, uint az) :
            x(ax),
            y(ay),
            z(az)
        {
        }
        NAU_FORCE_INLINE uint& operator[](int i)
        {
            return (&x)[i];
        }
        NAU_FORCE_INLINE const uint& operator[](int i) const
        {
            return (&x)[i];
        }
    };

    struct HlslUint4
    {
        uint x, y, z, w;
        NAU_FORCE_INLINE HlslUint4()
        {
        }  // -V730
        NAU_FORCE_INLINE HlslUint4(uint ax, uint ay, uint az, uint aw) :
            x(ax),
            y(ay),
            z(az),
            w(aw)
        {
        }
        NAU_FORCE_INLINE uint& operator[](int i)
        {
            return (&x)[i];
        }
        NAU_FORCE_INLINE const uint& operator[](int i) const
        {
            return (&x)[i];
        }
    };

    typedef HlslUint2 uint2;
    typedef HlslUint3 uint3;
    typedef HlslUint4 uint4;

    struct HlslInt2
    {
        int x, y;
        NAU_FORCE_INLINE HlslInt2()
        {
        }  // -V730
        NAU_FORCE_INLINE HlslInt2(int ax, int ay) :
            x(ax),
            y(ay)
        {
        }
        NAU_FORCE_INLINE int& operator[](int i)
        {
            return (&x)[i];
        }
    };

    struct HlslInt3
    {
        int x, y, z;
        NAU_FORCE_INLINE HlslInt3()
        {
        }  // -V730
        NAU_FORCE_INLINE HlslInt3(int ax, int ay, int az) :
            x(ax),
            y(ay),
            z(az)
        {
        }
        NAU_FORCE_INLINE int& operator[](int i)
        {
            return (&x)[i];
        }
    };

    struct HlslInt4
    {
        int x, y, z, w;
        NAU_FORCE_INLINE HlslInt4()
        {
        }  // -V730
        NAU_FORCE_INLINE HlslInt4(int ax, int ay, int az, int aw) :
            x(ax),
            y(ay),
            z(az),
            w(aw)
        {
        }
        NAU_FORCE_INLINE int& operator[](int i)
        {
            return (&x)[i];
        }
    };

    typedef HlslInt2 int2;
    typedef HlslInt3 int3;
    typedef HlslInt4 int4;

    struct Half2
    {
        nau::math::half x, y;

        NAU_FORCE_INLINE Half2()
        {
        }
        NAU_FORCE_INLINE Half2(float ax, float ay)
        {
            set(0, ax);
            set(1, ay);
        }
        NAU_FORCE_INLINE Half2(const Half2& a) :
            x(a.x),
            y(a.y)
        {
        }
        NAU_FORCE_INLINE Half2(const nau::math::Point2& a) :
            Half2(a.getX(), a.getY())
        {
        }

        NAU_FORCE_INLINE float get(int i) const
        {
            return float((&x)[i]);
        }
        NAU_FORCE_INLINE void set(int i, float v)
        {
            (&x)[i] = nau::math::half(v);
        }

        NAU_FORCE_INLINE nau::math::half operator[](int i) const
        {
            return (&x)[i];
        }
        operator nau::math::Point2() const
        {
            return nau::math::Point2(x, y);
        }
    };

    struct Half3
    {
        nau::math::half x, y, z;

        NAU_FORCE_INLINE Half3()
        {
        }
        NAU_FORCE_INLINE Half3(float ax, float ay, float az)
        {
            set(0, ax);
            set(1, ay);
            set(2, az);
        }
        NAU_FORCE_INLINE Half3(const Half3& a) :
            x(a.x),
            y(a.y),
            z(a.z)
        {
        }
        NAU_FORCE_INLINE Half3(const nau::math::Point3& a) :
            Half3(a.getX(), a.getY(), a.getZ())
        {
        }

        NAU_FORCE_INLINE float get(int i) const
        {
            return float((&x)[i]);
        }
        NAU_FORCE_INLINE void set(int i, float v)
        {
            (&x)[i] = nau::math::half(v);
        }

        NAU_FORCE_INLINE nau::math::half operator[](int i) const
        {
            return (&x)[i];
        }
        operator nau::math::Point3() const
        {
            return nau::math::Point3(x, y, z);
        }
    };

    struct Half4
    {
        nau::math::half x, y, z, w;

        NAU_FORCE_INLINE Half4()
        {
        }
        NAU_FORCE_INLINE Half4(float ax, float ay, float az, float aw)
        {
            set(0, ax);
            set(1, ay);
            set(2, az);
            set(3, aw);
        }
        NAU_FORCE_INLINE Half4(const Half4& a) :
            x(a.x),
            y(a.y),
            z(a.z),
            w(a.w)
        {
        }
        NAU_FORCE_INLINE Half4(const nau::math::Vector4& a) :
            Half4(a.getX(), a.getY(), a.getZ(), a.getW())
        {
        }

        NAU_FORCE_INLINE float get(int i) const
        {
            return float((&x)[i]);
        }
        NAU_FORCE_INLINE void set(int i, float v)
        {
            (&x)[i] = nau::math::half(v);
        }

        NAU_FORCE_INLINE nau::math::half operator[](int i) const
        {
            return (&x)[i];
        }
        operator nau::math::Vector4() const
        {
            return nau::math::Vector4(x, y, z, w);
        }
    };

    typedef nau::math::half half;
    typedef Half2 half2;
    typedef Half3 half3;
    typedef Half4 half4;

    typedef nau::math::float2 float2;
    typedef nau::math::float3 float3;
    typedef nau::math::float4 float4;

    template <typename FloatType>
    FloatType clamp(FloatType t, const FloatType min_val, const FloatType max_val);

    template <>
    NAU_FORCE_INLINE float clamp(float t, const float min_val, const float max_val)
    {
        return std::min(std::max(t, min_val), max_val);
    }

    template <>
    NAU_FORCE_INLINE float2 clamp(float2 t, const float2 min_val, const float2 max_val)
    {
        return min(max(t, min_val), max_val);
    }

    template <>
    NAU_FORCE_INLINE float3 clamp(float3 t, const float3 min_val, const float3 max_val)
    {
        return min(max(t, min_val), max_val);
    }

    template <>
    NAU_FORCE_INLINE float4 clamp(float4 t, const float4 min_val, const float4 max_val)
    {
        return min(max(t, min_val), max_val);
    }

    template <>
    NAU_FORCE_INLINE uint32_t clamp(uint32_t t, const uint32_t min_val, const uint32_t max_val)
    {
        return std::min(std::max(t, min_val), max_val);
    }

    template <>
    NAU_FORCE_INLINE int clamp(int t, const int min_val, const int max_val)
    {
        return std::min(std::max(t, min_val), max_val);
    }

    template <class T>
    inline T sign(T x)
    {
        return (x * 1 < 0) ? -1 : ((x > 0) ? 1 : 0);  // *1 - to avoid pointers
    };

    NAU_FORCE_INLINE float dot(const float3& a, const float3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    NAU_FORCE_INLINE float dot(const float4& a, const float4& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }
    NAU_FORCE_INLINE float dot(const float3& a, const float* b)
    {
        return dot(a, *(float3*)b);
    }
    NAU_FORCE_INLINE float dot(const float4& a, const float* b)
    {
        return dot(a, *(float4*)b);
    }

    NAU_FORCE_INLINE float3 reflect(const float3& i, const float3& n)
    {
        return i - 2.f * n * dot(i, n);
    }
    NAU_FORCE_INLINE float3 sign(const float3& a)
    {
        return float3(sign(a.x), sign(a.y), sign(a.z));
    }

    NAU_FORCE_INLINE float2 exp(const float2& a)
    {
        return float2(expf(a.x), expf(a.y));
    }
    NAU_FORCE_INLINE float3 exp(const float3& a)
    {
        return float3(expf(a.x), expf(a.y), expf(a.z));
    }
    NAU_FORCE_INLINE float4 exp(const float4& a)
    {
        return float4(expf(a.x), expf(a.y), expf(a.z), expf(a.w));
    }
    NAU_FORCE_INLINE float2 saturate(const float2& a)
    {
        return min(max(a, float2(0, 0)), float2(1, 1));
    }
    NAU_FORCE_INLINE float3 saturate(const float3& a)
    {
        return min(max(a, float3(0, 0, 0)), float3(1, 1, 1));
    }
    NAU_FORCE_INLINE float4 saturate(const float4& a)
    {
        return min(max(a, float4(0, 0, 0, 0)), float4(1, 1, 1, 1));
    }
    inline float smoothstep(float edge0, float edge1, float x)
    {
        // Scale, bias and saturate x to 0..1 range
        x = clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
        // Evaluate polynomial
        return x * x * (3.f - 2.f * x);
    }
#undef NAU_FORCE_INLINE

}  // namespace nau::render