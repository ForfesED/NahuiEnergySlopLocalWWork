// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include "nau/utils/type_list/type_list.h"

namespace nau::type_list
{
    template <typename U, typename... T>
    consteval bool contains(TypeList<T...>)
    {
        return (std::is_same_v<T, U> || ...);
    }

    template <typename... U, typename... T>
    consteval bool containsAll(TypeList<T...> l)
    {
        return (type_list::contains<U>(l) && ...);
    }

    template <typename TL, typename T>
    inline constexpr bool Contains = type_list::contains<T>(TL{});

}  // namespace nau::type_list
