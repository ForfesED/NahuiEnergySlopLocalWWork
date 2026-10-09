// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#pragma once
#include "nau/kernel/kernel_config.h"
#include "nau/math/math.h"

namespace nau::math
{

    // clang-format off
#define POLY0(x, c0) Vector4{c0}
#define POLY1(x, c0, c1) POLY0(x, c1) * x + Vector4{c0}
#define POLY2(x, c0, c1, c2) (POLY1(x, c1, c2)) * x + Vector4{c0}
#define POLY3(x, c0, c1, c2, c3) (POLY2(x, c1, c2, c3)) * x + Vector4{c0}
#define POLY4(x, c0, c1, c2, c3, c4) (POLY3(x, c1, c2, c3, c4)) * x + Vector4{c0}
#define POLY5(x, c0, c1, c2, c3, c4, c5) (POLY4(x, c1, c2, c3, c4, c5)) * x + Vector4{c0}
    // clang-format on

#define V_C_HALF Vector4(0.5f)
#define V_C_HALF_MINUS_EPS Vector4(0.5f - 1.192092896e-07f * 32)

#define EXP_DEF_PART                                                             \
    IVector4 ipart;                                                              \
    Vector4 fpart, expipart, expfpart;                                           \
    x = Vectormath::min(x, Vector4(129.00000f));                                 \
    x = Vectormath::max(x, Vector4(-126.99999f));                                \
    ipart = IVector4(Vectormath::SSE::vector4int::FromFloatRound(x - V_C_HALF)); \
    fpart = x - Vector4::fromVector4Int(ipart.get128());                         \
    expipart = Vector4::fromVector4Int(ShiftL((ipart + IVector4(127)).get128(), 23));

    /* minimax polynomial fit of 2**x, in range [-0.5, 0.5] */
    Vector4 NAU_FORCE_INLINE v_exp2_est_p5(Vector4 x)
    {
        EXP_DEF_PART
        expfpart = POLY5(fpart, 9.9999994e-1f, 6.9315308e-1f, 2.4015361e-1f, 5.5826318e-2f, 8.9893397e-3f, 1.8775767e-3f);

        return expipart * expfpart;
    }

    Vector4 NAU_FORCE_INLINE v_exp2_est_p4(Vector4 x)
    {
        EXP_DEF_PART
        expfpart = POLY4(fpart, 1.0000026f, 6.9300383e-1f, 2.4144275e-1f, 5.2011464e-2f, 1.3534167e-2f);
        return expipart * expfpart;
    }
    Vector4 NAU_FORCE_INLINE v_exp2_est_p3(Vector4 x)
    {
        EXP_DEF_PART
        expfpart = POLY3(fpart, 9.9992520e-1f, 6.9583356e-1f, 2.2606716e-1f, 7.8024521e-2f);
        return expipart * expfpart;
    }
    Vector4 NAU_FORCE_INLINE v_exp2_est_p2(Vector4 x)
    {
        EXP_DEF_PART
        expfpart = POLY2(fpart, 1.0017247f, 6.5763628e-1f, 3.3718944e-1f);
        return expipart * expfpart;
    }

    Vector4 NAU_FORCE_INLINE v_exp2(Vector4 x)
    {
        IVector4 ipart;
        Vector4 fpart, expipart, expfpart;
        x = Vectormath::min(x, Vector4(129.00000f));
        x = Vectormath::max(x, Vector4(-126.99999f));
        ipart = IVector4(FromFloatRound(x - V_C_HALF_MINUS_EPS));
        fpart = x - Vector4::fromVector4Int(ipart.get128());
        expipart = Vector4::fromVector4Int(ShiftL((ipart + IVector4(127)).get128(), 23));
        expfpart = POLY5(fpart, 9.9999994e-1f, 6.9315308e-1f, 2.4015361e-1f, 5.5826318e-2f, 8.9893397e-3f, 1.8775767e-3f);
        return select((expipart * expfpart), expipart, (FloatInVec(fpart.get128()) > FloatInVec(Vector4(0).get128())));  // ensure that exp2(int) = 2^int
    }

#undef EXP_DEF_PART
#define V_C_ONE \
    Vector4     \
    {           \
        1.0f    \
    }
#define LOG_DEF_PART                                                                                                      \
    static const IVector4 exp = IVector4(0x7F800000);                                                                     \
    static const IVector4 mant = IVector4(0x007FFFFF);                                                                    \
    IVector4 i = IVector4(FromFloatRound(x));                                                                             \
    Vector4 e = Vector4::fromVector4Int((IVector4(ShiftRu(And(i.get128(), exp.get128()), 23)) - IVector4(127)).get128()); \
    Vector4 m = orPerElem(Vector4::fromVector4Int((And(i.get128(), mant.get128()))), V_C_ONE);                            \
    Vector4 p;

    Vector4 NAU_FORCE_INLINE v_log2_est_p5(Vector4 x)
    {
        LOG_DEF_PART
        p = POLY5(m, 3.1157899f, -3.3241990f, 2.5988452f, -1.2315303f, 3.1821337e-1f, -3.4436006e-2f);
        /* This effectively increases the polynomial degree by one, but ensures that log2(1) == 0*/
        return p * (m - V_C_ONE) + e;
    }

    Vector4 NAU_FORCE_INLINE v_log2_est_p4(Vector4 x)
    {
        LOG_DEF_PART
        p = POLY4(m, 2.8882704548164776201f, -2.52074962577807006663f,
                  1.48116647521213171641f, -0.465725644288844778798f, 0.0596515482674574969533f);
        return p * (m - V_C_ONE) + e;
    }
    Vector4 NAU_FORCE_INLINE v_log2_est_p3(Vector4 x)
    {
        LOG_DEF_PART
        p = POLY3(m, 2.61761038894603480148f, -1.75647175389045657003f, 0.688243882994381274313f, -0.107254423828329604454f);
        return p * (m - V_C_ONE) + e;
    }
    Vector4 NAU_FORCE_INLINE v_log2_est_p2(Vector4 x)
    {
        LOG_DEF_PART
        p = POLY2(m, 2.28330284476918490682f, -1.04913055217340124191f, 0.204446009836232697516f);
        return p * (m - V_C_ONE) + e;
    }

#undef LOG_DEF_PART

    Vector4 NAU_FORCE_INLINE v_exp2_est(Vector4 x)
    {
        return v_exp2_est_p4(x);
    }
    Vector4 NAU_FORCE_INLINE v_log2_est(Vector4 x)
    {
        return v_log2_est_p4(x);
    }

    Vector4 NAU_FORCE_INLINE v_pow_est(Vector4 x, Vector4 y)
    {
        return v_exp2_est_p4(v_log2_est_p5(x) * y);
    }
    // natural log
    Vector4 NAU_FORCE_INLINE v_log(Vector4 x)
    {
        return (v_log2_est_p5(x) * Vector4(0.6931471805599453f));
    }
    // natural exponent
    Vector4 NAU_FORCE_INLINE v_exp(Vector4 x)
    {
        return v_exp2((x * Vector4(1.4426950408889634073599f)));
    }  // log2(e)
    // safer pow. checks for y == 0
    Vector4 NAU_FORCE_INLINE v_pow(Vector4 x, Vector4 y)
    {
        Vector4 ret = v_exp2(v_log2_est_p5(x) * y);
        ret = select(ret, V_C_ONE, y == Vector4(0.f));
        return ret;
    }

#undef POLY0
#undef POLY1
#undef POLY2
#undef POLY3
#undef POLY4
#undef POLY5

    NAU_FORCE_INLINE float safediv(float a, float b)
    {
        return b > MATH_SMALL_NUMBER ? a / b : (b < -MATH_SMALL_NUMBER ? a / b : 0.f);
    }
}  // namespace nau::math