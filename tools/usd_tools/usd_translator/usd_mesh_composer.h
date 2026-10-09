// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "usd_translator/usd_translator_api.h"
#include "usd_translator/usd_prim_adapter.h"

#include <nau/scene/scene.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usdGeom/primvarsAPI.h>

namespace UsdTranslator
{
    class USD_TRANSLATOR_API MeshComposer
    {
    public:
        MeshComposer(PXR_NS::UsdPrim prim);

        //OptimizedMesh optimizedMesh(std::vector<std::string> considerPrimvars); // todo

        uint32_t getNumIndices() const;
        uint32_t getNumVertices() const; 
        PXR_NS::VtArray<uint16_t> getIndices() const;
        PXR_NS::VtArray<PXR_NS::GfVec3f> getPositions() const;
        PXR_NS::VtArray<PXR_NS::GfVec3f> getNormals() const;
        PXR_NS::VtArray<PXR_NS::GfVec4f> getTangents() const;
        PXR_NS::VtArray<PXR_NS::GfVec2f> getUVs() const;
        PXR_NS::VtIntArray getJoints() const;
        PXR_NS::VtFloatArray getWeights() const;

        PXR_NS::UsdGeomMesh getUsdMesh() const;

    private:
        void computeNumIndVert();

        PXR_NS::UsdGeomMesh m_mesh;
        uint32_t m_numIndices = 0;
        uint32_t m_numVertex = 0;
    };

}