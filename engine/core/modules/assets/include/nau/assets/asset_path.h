// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once
#include <EASTL/functional.h>
#include <EASTL/string.h>
#include <EASTL/string_view.h>

#include <compare>
#include <string>
#include <string_view>

#include "nau/utils/result.h"

namespace nau
{
    /**
     */
    class NAU_COREASSETS_EXPORT AssetPath
    {
    public:
        static bool isValid(eastl::string_view fullAssetPath);

        AssetPath();
        AssetPath(const AssetPath&);
        AssetPath(AssetPath&&);
        AssetPath(eastl::string_view fullAssetPath);
        AssetPath(eastl::string_view scheme, eastl::string_view assetContainerPath, eastl::string_view assetInnerPath = {});

        AssetPath& operator=(const AssetPath&);
        AssetPath& operator=(AssetPath&&);

        eastl::string_view getScheme() const;
        eastl::string_view getContainerPath() const;
        eastl::string_view getSchemeAndContainerPath() const;
        eastl::string_view getAssetInnerPath() const;

        AssetPath& setScheme(eastl::string_view);
        AssetPath& setContainerPath(eastl::string_view);
        AssetPath& setAssetInnerPath(eastl::string_view);

        bool isEmpty() const;
        bool hasScheme(eastl::string_view scheme) const;
        eastl::string toString() const;
        Result<AssetPath> resolve() const;

        explicit operator bool() const;
        std::strong_ordering operator<=>(const AssetPath& other) const;
        bool operator==(const AssetPath& other) const;
        bool operator==(eastl::string_view right) const;

    private:
        size_t getHashCode() const
        {
            return eastl::hash<eastl::string>()(m_assetFullPath);
        }

        eastl::string m_assetFullPath;

        NAU_COREASSETS_EXPORT friend Result<> parse(std::string_view str, AssetPath& assetPath);
        NAU_COREASSETS_EXPORT friend std::string toString(const AssetPath& assetPath);

        template <typename>
        friend struct ::std::hash;

        template <typename>
        friend struct ::eastl::hash;
    };
}  // namespace nau

template <>
struct std::hash<::nau::AssetPath>
{
    [[nodiscard]]
    size_t operator()(const ::nau::AssetPath& val) const
    {
        return val.getHashCode();
    }
};

template <>
struct eastl::hash<::nau::AssetPath>
{
    [[nodiscard]]
    size_t operator()(const ::nau::AssetPath& val) const
    {
        return val.getHashCode();
    }
};
