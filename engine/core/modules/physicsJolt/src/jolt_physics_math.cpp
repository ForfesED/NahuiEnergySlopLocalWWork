// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/physics/jolt/jolt_physics_math.h"

namespace nau::physics::jolt
{
    JPH::RVec3 vec3ToJolt(math::vec3 nauV)
    {
        return JPH::RVec3(nauV.getX(), nauV.getY(), nauV.getZ());
    }

    JPH::Quat quatToJolt(math::quat nauQ)
    {
        return JPH::Quat(nauQ.getX(), nauQ.getY(), nauQ.getZ(), nauQ.getW());
    }

    math::vec4 joltVec4ToNauVec4(JPH::Vec4Arg joltVec)
    {
        return math::vec4(joltVec.GetX(), joltVec.GetY(), joltVec.GetZ(), joltVec.GetW());
    }

    math::vec3 joltVec3ToNauVec3(JPH::Vec3Arg joltVec)
    {
        return math::vec3(joltVec.GetX(), joltVec.GetY(), joltVec.GetZ());
    }

    math::Color4 joltColorToNauColor4(JPH::Color joltColor)
    {
        return math::Color4(joltColor.r, joltColor.g, joltColor.b, joltColor.a);
    }

    math::mat4 joltMatToNauMat(const JPH::Mat44& joltMat)
    {
        math::mat4 result;
        result.setCol(0, joltVec4ToNauVec4(joltMat.GetColumn4(0)));
        result.setCol(1, joltVec4ToNauVec4(joltMat.GetColumn4(1)));
        result.setCol(2, joltVec4ToNauVec4(joltMat.GetColumn4(2)));
        result.setCol(3, joltVec4ToNauVec4(joltMat.GetColumn4(3)));
        return result;
    }
} // namespace nau::physics::jolt

