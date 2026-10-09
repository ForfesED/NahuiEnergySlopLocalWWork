// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "gltf/gltf_file.h"

#include "nau/diag/logging.h"
#include "nau/serialization/json.h"
#include "nau/serialization/runtime_value_builder.h"

namespace nau
{
    Result<> GltfFile::loadFromJsonStream(const io::IStreamReader::Ptr& stream, GltfFile& gltfFile)
    {
        auto parseResult = serialization::jsonParse(stream->as<io::IStreamReader&>());
        NauCheckResult(parseResult);

        auto value = nau::makeValueRef(gltfFile);
        auto res = RuntimeValue::assign(value, *parseResult);
        if(!res)
        {
            NAU_LOG_ERROR("Fail to assign GLTF value: ({})", res.getError()->getMessage());
            return res;
        }

        return nau::ResultSuccess;
    }

}  // namespace nau
