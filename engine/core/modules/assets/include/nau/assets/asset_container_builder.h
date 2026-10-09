// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
#pragma once

#include "nau/io/stream.h"
#include "nau/rtti/type_info.h"
#include "nau/assets/asset_accessor.h"

namespace nau
{
    /**
     */
    struct NAU_ABSTRACT_TYPE IAssetContainerBuilder
    {
        NAU_TYPEID(nau::IAssetContainerBuilder)

        virtual ~IAssetContainerBuilder() = default;

        virtual bool isAcceptable(nau::Ptr<> asset) const = 0;
        virtual Result<> writeAssetToStream(io::IStreamWriter::Ptr stream, nau::Ptr<> asset) = 0;
    };

}  // namespace nau