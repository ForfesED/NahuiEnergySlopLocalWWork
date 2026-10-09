// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

namespace nau::shader_globals
{
    NAU_RENDER_EXPORT bool containsName(eastl::string_view name);

    NAU_RENDER_EXPORT void addVariable(eastl::string_view name, size_t size, const void* defaultValue = nullptr);

    NAU_RENDER_EXPORT void setVariable(eastl::string_view name, const void* value);
    NAU_RENDER_EXPORT void getVariable(eastl::string_view name, size_t* size, void** value);
} // namespace nau::shaderGlobals
