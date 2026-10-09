// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <utility>

#include "nau/utils/type_list/type_list.h"

namespace nau::type_list_detail
{
    template <typename T, auto>
    struct GetT
    {
        using type = T;
    };

    template <typename, typename>
    struct FillHelper;

    template <typename T, size_t... I>
    struct FillHelper<T, std::index_sequence<I...>>
    {
        using type = TypeList<typename GetT<T, I>::type...>;
    };

    template <typename T>
    struct FillHelper<T, std::index_sequence<>>
    {
        using type = TypeList<>;
    };
}  // namespace nau::type_list_detail

namespace nau::type_list
{
    template <typename T, size_t Size>
    using Fill = typename ::nau::type_list_detail::FillHelper<T, std::make_index_sequence<Size>>::type;
}  // namespace nau::type_list
