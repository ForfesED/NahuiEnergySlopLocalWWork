// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include "nau/utils/type_list/type_list.h"

namespace nau::type_list_detail
{

    template <typename... R>
    constexpr TypeList<R...> distinct(TypeList<>, TypeList<R...> r)
    {
        return r;
    }

    template <typename H, typename... T, typename... R>
    constexpr auto distinct(TypeList<H, T...> t, TypeList<R...> r)
    {
        if constexpr(type_list::findIndex<H>(r) < 0)
        {
            return distinct(TypeList<T...>{}, TypeList<R..., H>{});
        }
        else
        {
            return distinct(TypeList<T...>{}, r);
        }
    }

}  // namespace nau::type_list_detail

namespace nau::type_list
{

    template <typename... T>
    constexpr auto distinct(TypeList<T...> t)
    {
        return nau::type_list_detail::distinct(t, TypeList<>{});
    }

    template <typename TL>
    using Distinct = decltype(type_list::distinct(TL{}));

}  // namespace nau::type_list
