// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/math/math.h"
#include "nau/rtti/ptr.h"
#include "nau/debugRenderer/debug_render_system.h"

#include <Jolt/Jolt.h>
#include <Jolt/Core/Core.h>
#include <Jolt/Physics/Body/BodyFilter.h>
#include <Jolt/Renderer/DebugRendererSimple.h>

namespace nau::physics::jolt
{
    class JoltBodyDrawFilterImpl : public JPH::BodyDrawFilter
    {
    public:
        bool ShouldDraw(const JPH::Body& body) const override;
    };

    class DebugRendererImp final : public JPH::DebugRendererSimple
    {
    public:

        /**
         * @brief Draws a debug line segment.
         * 
         * @param [in] from, to Segment tips positions.
         * @param [in] color    Segment color.
         */
        virtual void DrawLine(JPH::RVec3Arg from, JPH::RVec3Arg to, JPH::ColorArg color) override;

        /**
         * @brief Draws a debug triangle.
         * 
         * @param [in] inV1, inV2, inV3 Triangle vertices.
         * @param [in] inColor          Triangle outline color.
         * @param [in] inCastShadow     Indicates whether the triangle should cast a shadow.
         * 
         * @note **inCastShadow** is currently unused.
         */
        virtual void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3,
            JPH::ColorArg inColor, ECastShadow inCastShadow = ECastShadow::Off) override;

        virtual void DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString, 
            JPH::ColorArg inColor, float inHeight) override;

        /**
         * @brief Renders a debug sphere.
         * 
         * @param [in] transform    Sphere center transform.
         * @param [in] radius       Sphere radius.
         * @param [in] color        Sphere wireframe color.
         */
        virtual void DrawSphere(JPH::RMat44Arg transform, float radius, JPH::ColorArg color) override;

        /**
         * @brief Sets the debug renderer that will be responsible for physics debug drawing.
         * 
         * @param [in] renderer A pointer to the renderer to bind.
         */
        void setDebugRenderer(nau::DebugRenderSystem* renderer);

    private:

        /**
         * @brief Debug renderer that is responsible for physics debug drawing.
         */
        nau::DebugRenderSystem* m_renderer = nullptr;
    };
}