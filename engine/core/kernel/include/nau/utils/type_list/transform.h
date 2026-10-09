// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/utils/type_list/type_list.h"

namespace nau::type_list
{

    template <template <typename, auto...> class Mapper, typename... T>
    constexpr auto transform_t(TypeList<T...>)
    {
        return TypeList<typename Mapper<T>::type...>{};
    }

    template <template <typename, auto...> class Mapper, typename... T>
    constexpr auto transform(TypeList<T...>)
    {
        return TypeList<Mapper<T>...>{};
    }

    template <typename TL, template <typename, auto...> class Mapper>
    using Transform = decltype(transform<Mapper>(TL{}));

    template <typename TL, template <typename, auto...> class Mapper>
    using TransformT = decltype(transform_t<Mapper>(TL{}));

}  // namespace nau::type_list
