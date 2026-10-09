// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include "nau/shaders/shader_globals.h"

#include <EASTL/unordered_map.h>

namespace nau::shader_globals
{
    namespace detail
    {
        eastl::unordered_map<eastl::string, eastl::vector<std::byte>> g_shaderDataTable;

        std::shared_mutex g_mutex;
    } // namespace detail

    using namespace detail;

    bool containsName(eastl::string_view name)
    {
        shared_lock_(g_mutex);
        return detail::g_shaderDataTable.contains(name);
    }

    void addVariable(eastl::string_view name, size_t size, const void* defaultValue /* = nullptr */)
    {
        NAU_ASSERT(size);

        lock_(g_mutex);

        g_shaderDataTable[name.data()] = {};
        g_shaderDataTable[name.data()].resize(size);

        if (defaultValue != nullptr)
        {
            memcpy(g_shaderDataTable[name.data()].data(), defaultValue, size);
        }
    }

    void setVariable(eastl::string_view name, const void* value)
    {
        NAU_FATAL(g_shaderDataTable.contains(name), "Global shader variable not found: {}", name.data());

        NAU_ASSERT(value);

        auto& buf = g_shaderDataTable[name.data()];
        memcpy(buf.data(), value, buf.size());
    }

    void getVariable(eastl::string_view name, size_t* size, void** value)
    {
        NAU_FATAL(g_shaderDataTable.contains(name), "Global shader variable not found: {}", name.data());

        NAU_ASSERT(size);
        NAU_ASSERT(value);

        auto& buf = g_shaderDataTable[name.data()];
        *size = buf.size();
        *value = buf.data();
    }
} // namespace nau::shaderGlobals
