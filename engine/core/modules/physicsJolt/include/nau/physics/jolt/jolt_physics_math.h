// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/physics/physics_defines.h"
#include "nau/math/dag_color.h"
#include "nau/math/math.h"

#include <Jolt/Jolt.h>
#include <Jolt/Core/Color.h>
#include <Jolt/Math/Real.h>
#include <Jolt/Math/Quat.h>

namespace nau::physics::jolt
{
    /**
     * @brief Translates Nau Engine vector3 to Jolt vector3.
     * 
     * @param [in] nauV Nau Engine vector.
     * @return          Jolt vector.
     */
    JPH::Vec3 vec3ToJolt(math::vec3 nauV);

    /**
     * @brief Translates Nau Engine quaternion to Jolt quaternion.
     *
     * @param [in] nauQ Nau Engine quaternion.
     * @return          Jolt quaternion.
     */
    JPH::Quat quatToJolt(math::quat nauQ);


    /**
     * @brief Translates Jolt vector4 to Nau Engine vector4.
     *
     * @param [in] joltVec  Jolt vector.
     * @return              Nau Engine vector.
     */
    math::vec4 joltVec4ToNauVec4(JPH::Vec4Arg joltVec);

    /**
     * @brief Translates Jolt vector3 to Nau Engine vector3.
     *
     * @param [in] joltVec  Jolt vector.
     * @return              Nau Engine vector.
     */
    math::vec3 joltVec3ToNauVec3(JPH::Vec3Arg joltVec);


    /**
     * @brief Translates Jolt matrix4x4 to Nau Engine matrix4x4.
     *
     * @param [in] joltMat  Jolt matrix.
     * @return              Nau Engine matrix.
     */
    math::mat4 joltMatToNauMat(JPH::Mat44Arg joltMat);

    /**
     * @brief Translates Jolt color to Nau Engine color.
     *
     * @param [in] joltColor    Jolt color.
     * @return                  Nau Engine color.
     */
    math::Color4 joltColorToNauColor4(JPH::Color joltColor);

} // namespace nau::physics::jolt

