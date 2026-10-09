// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/utils/type_list/type_list.h"

namespace nau::meta::type_list_internal
{

    /// <summary>
    ///
    /// </summary>
    template <typename TL, template <typename> class Predicate, typename Result = TypeList<>>
    struct Filter;

    template <template <typename> class Predicate, typename Result>
    struct Filter<TypeList<>, Predicate, Result>
    {
        using type = Result;
    };

    template <typename Current, typename... Tail, template <typename> class Predicate, typename... Result>
    struct Filter<TypeList<Current, Tail...>, Predicate, TypeList<Result...>>
    {
        using type = std::conditional_t<Predicate<Current>::value,
                                        typename Filter<TypeList<Tail...>, Predicate, TypeList<Result..., Current>>::type,
                                        typename Filter<TypeList<Tail...>, Predicate, TypeList<Result...>>::type>;
    };

}  // namespace nau::meta::type_list_internal
