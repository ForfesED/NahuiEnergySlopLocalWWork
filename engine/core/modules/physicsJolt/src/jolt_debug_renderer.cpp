// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/physics/jolt/jolt_debug_renderer.h"
#include "nau/physics/jolt/jolt_physics_math.h"
#include "nau/debugRenderer/debug_render_system.h"
#include "nau/physics/jolt/jolt_physics_body.h"

namespace nau::physics::jolt
{
    bool JoltBodyDrawFilterImpl::ShouldDraw(const JPH::Body& body) const
    {
        const auto& jBody = *reinterpret_cast<JoltPhysicsBody*>(body.GetUserData());
        return jBody.debugDrawEnabled();
    };

    void DebugRendererImp::DrawLine(JPH::RVec3Arg from, JPH::RVec3Arg to, JPH::ColorArg color)
    {
        if (!m_renderer)
        {
            return;
        }

        const Vectormath::SSE::Point3 start { from.GetX(), from.GetY(), from.GetZ() };
        const Vectormath::SSE::Point3 stop { to.GetX(), to.GetY(), to.GetZ() };

        m_renderer->drawLine(start, stop, joltColorToNauColor4(color), 1.f);
    }

    void DebugRendererImp::DrawTriangle(JPH::RVec3Arg v1, JPH::RVec3Arg v2,
        JPH::RVec3Arg v3, JPH::ColorArg color, ECastShadow castShadow)
    {
        DrawLine(v1, v2, color);
        DrawLine(v2, v3, color);
        DrawLine(v3, v1, color);
    }

    void DebugRendererImp::DrawText3D(JPH::RVec3Arg inPosition,
        const std::string_view& inString, JPH::ColorArg inColor, float inHeight)
    { 
    }

    void DebugRendererImp::DrawSphere(JPH::RMat44Arg transform, float radius, JPH::ColorArg color)
    {
        if (m_renderer)
        {
            static const float drawDensity = 100.0;
            m_renderer->drawSphere(radius, joltColorToNauColor4(color), joltMatToNauMat(transform), drawDensity, 1.0);
        }
    }

    void DebugRendererImp::setDebugRenderer(nau::DebugRenderSystem* renderer)
    {
        m_renderer = renderer;
    }
}
