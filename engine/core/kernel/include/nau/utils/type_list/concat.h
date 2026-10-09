// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include "nau/utils/type_list/type_list.h"

namespace nau::type_list_detail
{

    template <typename... T1, typename... T2>
    constexpr auto concat2List(TypeList<T1...>, TypeList<T2...>)
    {
        return TypeList<T1..., T2...>{};
    }

}  // namespace nau::type_list_detail

namespace nau::type_list
{
    consteval TypeList<> concat()
    {
        return TypeList<>{};
    }

    template <typename H, typename... T>
    consteval auto concat(H h, T... t)
    {
        return nau::type_list_detail::concat2List(h, type_list::concat(t...));
    }

    template <typename... TL>
    using Concat = decltype(type_list::concat(TL{}...));

}  // namespace nau::type_list
