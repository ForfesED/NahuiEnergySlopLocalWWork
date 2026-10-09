
// ================================================================================================
// -*- C++ -*-
// File: vectormath/vectormath.hpp
// Author: Guilherme R. Lampert
// Created on: 30/12/16
// Brief: This header exposes the Sony Vectormath library types and functions into the global scope.
// ================================================================================================

#ifndef VECTORMATH_HPP
#define VECTORMATH_HPP


#include "vectormath_settings.hpp"

//For similar() functions
#define MATH_SMALL_NUMBER 0.000001f


#if (VECTORMATH_CPU_HAS_SSE1_OR_BETTER && !VECTORMATH_FORCE_SCALAR_MODE) // SSE
    #include "sse/vectormath.hpp"
namespace Vectormath
{
    using namespace Vectormath::SSE;
}
#elif (VECTORMATH_CPU_HAS_NEON && !VECTORMATH_FORCE_SCALAR_MODE) // NEON
	#include "neon/vectormath.hpp"
namespace Vectormath
{
    using namespace Vectormath::Neon;
}
#else // !SSE
    #include "scalar/vectormath.hpp"
namespace Vectormath
{
    using namespace Vectormath::Scalar;
}
#endif // Vectormath mode selection

//========================================= #TheForgeMathExtensionsBegin ================================================
//========================================= #TheForgeAnimationMathExtensionsBegin =======================================
#include "soa/soa.hpp"
namespace Vectormath
{
    using namespace Vectormath::Soa;
}
//========================================= #TheForgeAnimationMathExtensionsEnd =======================================
//========================================= #TheForgeMathExtensionsEnd ================================================

#include "vec2d.hpp"  // - Extended 2D vector and point classes; not aligned and always in scalar floats mode.
#include "common.hpp" // - Miscellaneous helper functions.

using namespace Vectormath::Comparators;

#endif // VECTORMATH_HPP
