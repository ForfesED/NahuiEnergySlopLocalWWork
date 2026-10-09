// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
#pragma once

#include "nau/io/stream.h"
#include "nau/rtti/type_info.h"
#include "nau/assets/asset_container_builder.h"

namespace nau
{
    /**
     */
    struct TextureAssetContainerBuilder : public IAssetContainerBuilder
    {
        NAU_INTERFACE(nau::TextureAssetContainerBuilder, IAssetContainerBuilder)

        bool isAcceptable(nau::Ptr<> asset) const override;

        Result<> writeAssetToStream(io::IStreamWriter::Ptr stream, nau::Ptr<> asset) override;
    };

}  // namespace nau