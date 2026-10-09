// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include "nau/utils/type_list/type_list.h"

namespace nau::type_list_detail
{

    template <typename, typename...>
    struct Append;

    template <typename, typename...>
    struct AppendHead;

    template <typename... T, typename... El>
    struct Append<TypeList<T...>, El...>
    {
        using type = TypeList<T..., El...>;
    };

    template <typename... T, typename... El>
    struct AppendHead<TypeList<T...>, El...>
    {
        using type = TypeList<El..., T...>;
    };

}  // namespace nau::type_list_detail

namespace nau::type_list
{

    template <typename TL, typename T, typename... U>
    using Append = typename nau::type_list_detail::Append<TL, T, U...>::type;

    template <typename TL, typename T, typename... U>
    using AppendHead = typename nau::type_list_detail::AppendHead<TL, T, U...>::type;

}  // namespace nau::type_list
