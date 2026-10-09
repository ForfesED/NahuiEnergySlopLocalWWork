// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/diag/source_info.h


#pragma once

#include <EASTL/string_view.h>

#include <optional>

#include "nau/kernel/kernel_config.h"
#include "nau/utils/preprocessor.h"

namespace nau::diag
{

    /**
     */
    struct NAU_KERNEL_EXPORT SourceInfo
    {
        eastl::string_view moduleName;
        eastl::string_view functionName;
        eastl::string_view filePath;
        std::optional<unsigned> line;

        SourceInfo() = default;
        SourceInfo(const SourceInfo&) = default;

        SourceInfo(eastl::string_view module, eastl::string_view func, eastl::string_view file, std::optional<unsigned> ln = std::nullopt) :
            moduleName(module),
            functionName(func),
            filePath(file),
            line(ln)
        {
        }

        SourceInfo(eastl::string_view func, eastl::string_view file, std::optional<unsigned> ln = std::nullopt) :
            SourceInfo{{}, func, file, ln}
        {
        }

        SourceInfo& operator=(const SourceInfo&) = default;

        explicit operator bool() const
        {
            return !functionName.empty() || !filePath.empty();
        }
    };

}  // namespace nau::diag

#define NAU_INLINED_SOURCE_INFO                                                                             \
    ::nau::diag::SourceInfo                                                                                 \
    {                                                                                                       \
        ::eastl::string_view{__FUNCTION__}, ::eastl::string_view{__FILE__}, static_cast<unsigned>(__LINE__) \
    }
