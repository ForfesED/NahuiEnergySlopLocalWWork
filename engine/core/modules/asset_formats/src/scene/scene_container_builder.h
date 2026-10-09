// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/assets/asset_container_builder.h"
#include "nau/rtti/rtti_impl.h"

namespace nau
{
    /**
     */
    class SceneContainerBuilder final : public IAssetContainerBuilder,
                                        public IRttiObject
    {
        NAU_RTTI_CLASS(nau::SceneContainerBuilder, IAssetContainerBuilder, IRttiObject)

    private:
        bool isAcceptable(nau::Ptr<> asset) const override;
        Result<> writeAssetToStream(io::IStreamWriter::Ptr stream, nau::Ptr<> asset) override;
    };

}  // namespace nau
