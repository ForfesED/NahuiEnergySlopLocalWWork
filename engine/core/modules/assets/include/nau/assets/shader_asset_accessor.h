// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/assets/asset_accessor.h"
#include "nau/async/task.h"
#include "nau/memory/bytes_buffer.h"
#include "nau/utils/enum/enum_reflection.h"

#include "shader.h"

namespace nau
{
    /**
     */
    struct NAU_ABSTRACT_TYPE IShaderAssetAccessor : IAssetAccessor
    {
        NAU_INTERFACE(nau::IShaderAssetAccessor, IAssetAccessor)

        virtual Result<> fillShader(Shader& shader) const = 0;
    };
}  // namespace nau
