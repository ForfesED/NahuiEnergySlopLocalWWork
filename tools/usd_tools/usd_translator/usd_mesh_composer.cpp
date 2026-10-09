// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "usd_mesh_composer.h"
#include "usd_proxy/usd_proxy.h"
#include <pxr/base/vt/types.h>
#include <pxr/usd/usdSkel/bindingAPI.h>

using namespace PXR_NS;

namespace UsdTranslator
{
    template<class T>
    inline void adaptValue(PXR_NS::VtArray<T>& dest, const PXR_NS::VtArray<int>& faceVertexCounts, const PXR_NS::VtArray<T>& src) // for uniform primvars
    {
        auto uniformIndex = 0;
        auto destIt = dest.begin();
        for (auto numVertex : faceVertexCounts) // todo optimize parallel for
        {
            for (auto i = 0; i < numVertex; ++i)
            {
                *destIt = src[uniformIndex];
                destIt++;
            }
            uniformIndex++;
        }
    }

    template<class T>
    inline void adaptValue(PXR_NS::VtArray<T>& dest, const PXR_NS::VtArray<T>& src, const PXR_NS::VtArray<int>& srcIndices) // for indexed faceVarying or vertex primvars
    {
        for (int i = 0; i < dest.size(); ++i) // todo optimize parallel for
        {
            dest[i] = src[srcIndices[i]];
        }
    }

    template<class T>
    inline void adaptValue(PXR_NS::VtArray<T>& dest, const PXR_NS::VtArray<T>& src, const PXR_NS::VtArray<int>& srcIndices, int elementSize, int requiredSize) // for indexed faceVarying or vertex primvars
    {
        const int elementCount = std::min(requiredSize, elementSize);
        int i = 0;
        for (int index : srcIndices) // todo optimize parallel for
        {
            for (int j = 0; j < elementCount; ++j, ++i)
            {
                dest[i] = src[index * elementSize + j];
            }
        }
    }


    MeshComposer::MeshComposer(PXR_NS::UsdPrim prim)
        : m_mesh(prim)
    {
        computeNumIndVert();
    }

    void MeshComposer::computeNumIndVert()
    {
        PXR_NS::VtValue faceVertexCounts;
        m_mesh.GetFaceVertexCountsAttr().Get(&faceVertexCounts);
        if (!faceVertexCounts.CanCast<PXR_NS::VtArray<int>>())
        {
            auto meshPath = m_mesh.GetPath().GetString();
            return;
        }

        for (auto num : faceVertexCounts.Get<PXR_NS::VtArray<int>>())
        {
            m_numIndices += (num - 2) * 3;
            m_numVertex += num;
        }
    }

    uint32_t MeshComposer::getNumIndices() const
    {
        return m_numIndices;
    }

    uint32_t MeshComposer::getNumVertices() const
    {
        return m_numVertex;
    }

    PXR_NS::UsdGeomMesh MeshComposer::getUsdMesh() const
    {
        return m_mesh;
    }

    PXR_NS::VtArray<uint16_t> MeshComposer::getIndices() const
    {
        PXR_NS::VtValue faceVertexCounts;
        m_mesh.GetFaceVertexCountsAttr().Get(&faceVertexCounts);
        if (!faceVertexCounts.CanCast<PXR_NS::VtArray<int>>())
            return{};

        PXR_NS::VtArray<uint16_t> indices;
        indices.reserve(getNumIndices());
        auto faceVaryingIndex = 0;
        for (auto numVertex : faceVertexCounts.Get<PXR_NS::VtArray<int>>()) // todo optimize parallel for
        {
            for (auto i = 0; i < 3; ++i)
            {
                indices.push_back(faceVaryingIndex + i);
            }
            for (auto i = 3; i < numVertex; ++i)
            {
                indices.push_back(faceVaryingIndex);
                indices.push_back(faceVaryingIndex + i - 1);
                indices.push_back(faceVaryingIndex + i);
            }
            faceVaryingIndex += numVertex;
        }

        return std::move(indices);
    }
   
    PXR_NS::VtArray<PXR_NS::GfVec3f> MeshComposer::getPositions() const
    {
        if (!m_mesh.GetPointsAttr().IsValid())
            return {};
        PXR_NS::VtValue vertexData;
        m_mesh.GetPointsAttr().Get(&vertexData);
        if(!vertexData.CanCast<PXR_NS::VtArray<PXR_NS::GfVec3f>>())
            return {};

        PXR_NS::VtValue vertexIndices;
        if (m_mesh.GetFaceVertexIndicesAttr().IsValid())
        {
            m_mesh.GetFaceVertexIndicesAttr().Get(&vertexIndices);
            if (!vertexIndices.CanCast<PXR_NS::VtArray<int>>())
                return {};

            PXR_NS::VtArray<PXR_NS::GfVec3f> dest;
            dest.resize(*getIndices().rbegin() + 1);
            adaptValue(dest, vertexData.Get<PXR_NS::VtArray<PXR_NS::GfVec3f>>(), vertexIndices.Get<PXR_NS::VtArray<int>>());
            return std::move(dest);
        }

        return vertexData.Get<PXR_NS::VtArray<PXR_NS::GfVec3f>>();
    }

    PXR_NS::VtArray<PXR_NS::GfVec3f> MeshComposer::getNormals() const
    {
        if (!m_mesh.GetNormalsAttr().IsValid())
            return {};

        PXR_NS::VtValue vertexData;
        m_mesh.GetNormalsAttr().Get(&vertexData);
        if (!vertexData.CanCast<PXR_NS::VtArray<PXR_NS::GfVec3f>>())
            return {};

        return vertexData.Get<PXR_NS::VtArray<PXR_NS::GfVec3f>>();
    }

    PXR_NS::VtArray<PXR_NS::GfVec4f> MeshComposer::getTangents() const
    {
        PXR_NS::VtArray<uint16_t> indices = getIndices();
        PXR_NS::VtArray<PXR_NS::GfVec3f> normals = getNormals();
        PXR_NS::VtArray<PXR_NS::GfVec3f> positions = getPositions();
        PXR_NS::VtArray<PXR_NS::GfVec2f> uvs = getUVs();

        if (indices.empty() || positions.empty() || uvs.empty())
        {
            return {};
        }

        PXR_NS::VtArray<PXR_NS::GfVec4f> tangents;
        tangents.resize(normals.size());

        for (int i = 0; i < indices.size(); i += 3)
        {
            uint16_t index0 = indices[i + 0];
            uint16_t index1 = indices[i + 1];
            uint16_t index2 = indices[i + 2];

            const PXR_NS::GfVec3f& v0 = positions[index0];
            const PXR_NS::GfVec3f& v1 = positions[index1];
            const PXR_NS::GfVec3f& v2 = positions[index2];

            const PXR_NS::GfVec2f& uv0 = uvs[index0];
            const PXR_NS::GfVec2f& uv1 = uvs[index1];
            const PXR_NS::GfVec2f& uv2 = uvs[index2];

            const PXR_NS::GfVec3f dv1 = v1 - v0;
            const PXR_NS::GfVec3f dv2 = v2 - v0;

            const PXR_NS::GfVec2f duv1 = uv1 - uv0;
            const PXR_NS::GfVec2f duv2 = uv2 - uv0;

            float r = 1.0f / (duv1[0] * duv2[1] - duv1[1] * duv2[0]);
            PXR_NS::GfVec3f tangent = (dv1 * duv2[1] - dv2 * duv1[1]) * r;

            PXR_NS::GfVec3f tangent0 = tangent - PXR_NS::GfDot(tangent, normals[index0]) * normals[index0];
            PXR_NS::GfVec3f tangent1 = tangent - PXR_NS::GfDot(tangent, normals[index1]) * normals[index1];
            PXR_NS::GfVec3f tangent2 = tangent - PXR_NS::GfDot(tangent, normals[index2]) * normals[index2];

            tangent0.Normalize();
            tangent1.Normalize();
            tangent2.Normalize();

            tangents[index0] = PXR_NS::GfVec4f(tangent0[0], tangent0[1], tangent0[2], 1.0f);
            tangents[index1] = PXR_NS::GfVec4f(tangent1[0], tangent1[1], tangent1[2], 1.0f);
            tangents[index2] = PXR_NS::GfVec4f(tangent2[0], tangent2[1], tangent2[2], 1.0f);
        }

        return tangents;
    }

    PXR_NS::VtArray<PXR_NS::GfVec2f> MeshComposer::getUVs() const
    {
        PXR_NS::UsdGeomPrimvarsAPI primvars(m_mesh.GetPrim());
        PXR_NS::TfToken uvPrimvarName = "UVMap"_tftoken;  // Blender case
        if (!primvars.HasPrimvar(uvPrimvarName))
            uvPrimvarName = "st"_tftoken;                 // Maya and standard usd case
        if (!primvars.HasPrimvar(uvPrimvarName))
            return {};

        auto primvar = primvars.GetPrimvar(uvPrimvarName);

        PXR_NS::VtValue vertexData;
        primvar.Get(&vertexData);
        if (!vertexData.CanCast<PXR_NS::VtArray<PXR_NS::GfVec2f>>())
            return {};

        auto interpol = primvar.GetInterpolation();
        if (interpol == "faceVarying"_tftoken)
        {
            if (primvar.IsIndexed())
            {
                PXR_NS::VtValue primvarIndices;
                primvar.GetIndicesAttr().Get(&primvarIndices);
                if (!primvarIndices.CanCast<PXR_NS::VtArray<int>>())
                    return {};

                PXR_NS::VtArray<PXR_NS::GfVec2f> dest;
                dest.resize(*getIndices().rbegin() + 1);
                adaptValue(dest, vertexData.Get<PXR_NS::VtArray<PXR_NS::GfVec2f>>(), primvarIndices.Get<PXR_NS::VtArray<int>>());
                return std::move(dest);
            }
            else
                return vertexData.Get<PXR_NS::VtArray<PXR_NS::GfVec2f>>();
           
        }
        else if (interpol == "vertex"_tftoken)
        {
            PXR_NS::VtValue primvarIndices;
            m_mesh.GetFaceVertexIndicesAttr().Get(&primvarIndices);
            if (!primvarIndices.CanCast<PXR_NS::VtArray<int>>())
                return {};

            PXR_NS::VtArray<PXR_NS::GfVec2f> dest;
            dest.resize(*getIndices().rbegin() + 1);
            adaptValue(dest, vertexData.Get<PXR_NS::VtArray<PXR_NS::GfVec2f>>(), primvarIndices.Get<PXR_NS::VtArray<int>>());
            return std::move(dest);
        }
        else
        {
            NAU_FAILURE("Unsupported Interpolation {}", interpol.GetText());
        }

        return {};
    }

    PXR_NS::VtIntArray MeshComposer::getJoints() const
    {
        pxr::UsdSkelBindingAPI bindingApi{ m_mesh };
        PXR_NS::UsdGeomPrimvar primvar = bindingApi.GetJointIndicesPrimvar();
        if (!primvar)
            return {};
        pxr::VtIntArray attributeData;
        if (!primvar.Get(&attributeData))
            return {};

        PXR_NS::VtValue vertexIndices;
        if (m_mesh.GetFaceVertexIndicesAttr().IsValid())
        {
            m_mesh.GetFaceVertexIndicesAttr().Get(&vertexIndices);
            if (!vertexIndices.CanCast<PXR_NS::VtIntArray>())
                return {};

            PXR_NS::VtIntArray dest;
            dest.resize((*getIndices().rbegin() + 1) * 4);
            adaptValue(dest, attributeData, vertexIndices.Get<PXR_NS::VtIntArray>(), primvar.GetElementSize(), 4);
            return dest;
        }

        return attributeData;
    }

    PXR_NS::VtFloatArray MeshComposer::getWeights() const
    {
        pxr::UsdSkelBindingAPI bindingApi{ m_mesh };
        PXR_NS::UsdGeomPrimvar primvar = bindingApi.GetJointWeightsPrimvar();
        if (!primvar)
            return {};
        pxr::VtFloatArray attributeData;
        if (!primvar.Get(&attributeData))
            return {};

        PXR_NS::VtValue vertexIndices;
        if (m_mesh.GetFaceVertexIndicesAttr().IsValid())
        {
            m_mesh.GetFaceVertexIndicesAttr().Get(&vertexIndices);
            if (!vertexIndices.CanCast<PXR_NS::VtIntArray>())
                return {};

            PXR_NS::VtFloatArray dest;
            dest.resize((*getIndices().rbegin() + 1) * 4);
            adaptValue(dest, attributeData, vertexIndices.Get<PXR_NS::VtIntArray>(), primvar.GetElementSize(), 4);
            return dest;
        }

        return attributeData;
    }

    

    

}