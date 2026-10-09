// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/3d/dag_drv3d.h"
#include "nau/assets/asset_view.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/utils/functor.h"
#include "graphics_assets/static_meshes/static_mesh.h"

namespace nau
{
    /**
     */
    class NAU_GRAPHICSASSETS_EXPORT StaticMeshAssetView : public IAssetView
    {
        NAU_CLASS_(nau::StaticMeshAssetView, IAssetView)
    public:
        static async::Task<nau::Ptr<StaticMeshAssetView>> createFromAssetAccessor(nau::Ptr<> accessor);

        Sbuffer* getPositionsBuffer() const; // todo: NAU-1797 Remove, use getMesh
        Sbuffer* getNormalsBuffer() const; // todo: NAU-1797 Remove, use getMesh
        Sbuffer* getTangentsBuffer() const; // todo: NAU-1797 Remove, use getMesh
        Sbuffer* getTexcoordsBuffer() const; // todo: NAU-1797 Remove, use getMesh
        Sbuffer* getIndexBuffer() const; // todo: NAU-1797 Remove, use getMesh
        unsigned getIndexCount() const; // todo: NAU-1797 Remove, use getMesh
        unsigned getVertexCount() const; // todo: NAU-1797 Remove, use getMesh

        void enumerateMeshTriangles(Functor<void(
            const nau::math::vec3&, const nau::math::vec3&, const nau::math::vec3&)> sink) const;

        inline nau::StaticMesh::Ptr getMesh()
        {
            return m_mesh;
        };

    protected:
        nau::StaticMesh::Ptr m_mesh;
    };

}  // namespace nau
