// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#pragma once

#include "nau/math/math.h"
#include "nau/math/dag_bounds3.h"

namespace nau::math
{

    struct NAU_KERNEL_EXPORT NauFrustum // TODO: move this code to nau::math::Frustum
    {
        enum Face
        {
            Right = 0,
            Left = 1,
            Top = 2,
            Bottom = 3,
            FarPlane = 4,
            NearPlane = 5
        };
        enum
        {
          RIGHT = 0,
          LEFT = 1,
          TOP = 2,
          BOTTOM = 3,
          FARPLANE = 4,
          NEARPLANE = 5
        };

        NauFrustum() : plane03W2(Vector4::zero()), plane03W(Vector4::zero()), camPlanes{ Vector4::zero() }
        {
        }

        NauFrustum(const nau::math::Matrix4& matrix) { construct(matrix); }

        void construct(const nau::math::Matrix4& matrix);


        inline int testSphereOrthoB(Vector3 c, Vector4 rad) const;
        inline int testSphereOrtho(Vector3 center, Vector4 rad) const;

        int testSphereB(Vector3 c, Vector4 rad) const;
        int testSphere(Vector3 c, Vector4 rad) const;

        inline int testSphere(const BSphere3& sphere) const { return testSphere(sphere.c, Vector4{ sphere.r }); }


        Vector4 camPlanes[6];
        Vector4 plane03X, plane03Y, plane03Z, plane03W2, plane03W, plane4W2, plane5W2;

        // generate 8 points
        void generateAllPointFrustm(Vector3* pntList) const;
        void calcFrustumBBox(BBox3& box) const;
    };

    //  test if a sphere is within the view frustum

    // in ortho frustum, each odd plane is faced backwards to prev even plane. save dot product
    inline int NauFrustum::testSphereOrthoB(Vector3 c, Vector4 rad) const
    {
        return testSphereB(c, rad);
    }

    inline int NauFrustum::testSphereOrtho(Vector3 c, Vector4 rad) const
    {
        return testSphere(c, rad);
    }


    // zfar_plane should be normalized and faced towards camera origin. camPlanes[4] in Frustum is of that kind
    // v_plane_dist(zfar_plane, cur_view_pos) - should be positive!
    NAU_KERNEL_EXPORT Vector4 shrink_zfar_plane(Vector4 zfar_plane, Vector4 cur_view_pos, Vector4 max_z_far_dist);

    // znear_plane should be normalized and faced backwards  camera origin. camPlanes[5] in Frustum is of that kind
    // max_z_near_dist - is positive
    // v_plane_dist(znear_plane, cur_view_pos) - should be negative!
    NAU_KERNEL_EXPORT Vector4 expand_znear_plane(Vector4 znear_plane, Vector4 cur_view_pos, Vector4 max_z_near_dist);

} // namespace nau::math

