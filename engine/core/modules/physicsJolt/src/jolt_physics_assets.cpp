// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/physics/jolt/jolt_physics_assets.h"

namespace nau::physics::jolt
{
    namespace
    {
        void fillMeshTopology(IMeshAssetAccessor& meshAccessor, JPH::VertexList& positions, JPH::IndexedTriangleList& triangles)
        {
            static_assert(sizeof(JPH::Float3) == sizeof(float[3]));

            const MeshDescription meshDesc = meshAccessor.getDescription();
            NAU_ASSERT(meshDesc.indexCount % 3 == 0);

            positions.resize(meshDesc.vertexCount);

            std::array<OutputVertAttribDescription, 1> vOutput;

            vOutput.front().semantic = "POSITION";
            vOutput.front().semanticIndex = 0;
            vOutput.front().elementFormat = ElementFormat::Float;
            vOutput.front().attributeType = AttributeType::Vec3;
            vOutput.front().outputBuffer = positions.data();
            vOutput.front().outputBufferSize = sizeof(JPH::Float3) * positions.size();
            vOutput.front().byteStride = 0;

            meshAccessor.copyVertAttribs(vOutput).ignore();

            triangles.resize(meshDesc.indexCount / 3);

            if (meshDesc.indexFormat == ElementFormat::Uint16)
            {
                std::vector<uint16_t> indices(meshDesc.indexCount);
                meshAccessor.copyIndices(indices.data(), sizeof(uint16_t) * indices.size(), ElementFormat::Uint16).ignore();

                for (size_t i = 0, triCount = triangles.size(); i < triCount; ++i)
                {
                    const uint32_t i0 = static_cast<uint32_t>(indices[i * 3 + 0]);
                    const uint32_t i1 = static_cast<uint32_t>(indices[i * 3 + 1]);
                    const uint32_t i2 = static_cast<uint32_t>(indices[i * 3 + 2]);
                    triangles[i] = JPH::IndexedTriangle{i0, i1, i2, 0};
                }
            }
            else if (meshDesc.indexFormat == ElementFormat::Uint32)
            {
                std::vector<uint32_t> indices(meshDesc.indexCount);
                meshAccessor.copyIndices(indices.data(), sizeof(uint32_t) * indices.size(), ElementFormat::Uint32).ignore();

                for (size_t i = 0, triCount = triangles.size(); i < triCount; ++i)
                {
                    const uint32_t i0 = indices[i * 3 + 0];
                    const uint32_t i1 = indices[i * 3 + 1];
                    const uint32_t i2 = indices[i * 3 + 2];
                    triangles[i] = JPH::IndexedTriangle{i0, i1, i2, 0};
                }
            }
            else
            {
                NAU_FAILURE("Unsupported Index Format");
            }
        }

    }  // namespace

    JoltConvexHullAssetView::JoltConvexHullAssetView(IMeshAssetAccessor& meshAccessor)
    {
        JPH::VertexList positions;
        JPH::IndexedTriangleList triangles;
        fillMeshTopology(meshAccessor, positions, triangles);

        JPH::Array<JPH::Vec3> chPoints(triangles.size() * 3);
        size_t idx = 0;
        for (const JPH::IndexedTriangle& tri : triangles)
        {
            const JPH::Float3& p0 = positions[tri.mIdx[0]];
            const JPH::Float3& p1 = positions[tri.mIdx[1]];
            const JPH::Float3& p2 = positions[tri.mIdx[2]];

            chPoints[idx++] = JPH::Vec3{p0.x, p0.y, p0.z};
            chPoints[idx++] = JPH::Vec3{p1.x, p1.y, p1.z};;
            chPoints[idx++] = JPH::Vec3{p2.x, p2.y, p2.z};;
        }

        m_convexHullShapeSettings.emplace(std::move(chPoints));
    }

    JPH::ConvexHullShapeSettings& JoltConvexHullAssetView::getShapeSettings()
    {
        NAU_FATAL(m_convexHullShapeSettings);
        return *m_convexHullShapeSettings;
    }

    JoltTriMeshAssetView::JoltTriMeshAssetView(IMeshAssetAccessor& meshAccessor)
    {
        JPH::VertexList positions;
        JPH::IndexedTriangleList triangles;
        fillMeshTopology(meshAccessor, positions, triangles);

        m_meshShapeSettings.emplace(std::move(positions), std::move(triangles));
    }

    JPH::MeshShapeSettings& JoltTriMeshAssetView::getShapeSettings()
    {
        NAU_FATAL(m_meshShapeSettings);
        return *m_meshShapeSettings;
    }
}  // namespace nau::physics::jolt