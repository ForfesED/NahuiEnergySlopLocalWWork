// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include <tuple>
#include <type_traits>

namespace nau
{

    /**
     *
     */
    template <typename... T>
    struct TypeList
    {
        static constexpr size_t Size = sizeof...(T);
    };
}  // namespace nau

namespace nau::type_list_detail
{

    template <typename>
    struct TypeListOf;

    template <typename... T>
    struct TypeListOf<std::tuple<T...>>
    {
        using type = TypeList<T...>;
    };

    template <typename T>
    struct IsTypeList : std::false_type
    {
    };

    template <typename... U>
    struct IsTypeList<TypeList<U...>> : std::true_type
    {
    };

}  // namespace nau::type_list_detail

namespace nau::type_list
{

    template <typename U>
    constexpr int findIndex(TypeList<>, int = 0)
    {
        return -1;
    }

    template <typename U, typename H, typename... T>
    constexpr int findIndex(TypeList<H, T...>, int index = 0)
    {
        if constexpr(std::is_same_v<H, U>)
        {
            return index;
        }
        else
        {
            return findIndex<U>(TypeList<T...>{}, index + 1);
        }
    }

    template <typename T>
    using TypeListOf = typename type_list_detail::TypeListOf<T>::type;

    template <typename TL, typename T>
    inline constexpr int ElementIndex = type_list::findIndex<T>(TL{});

    template <typename T>
    inline constexpr bool IsTypeList = type_list_detail::IsTypeList<T>::value;

}  // namespace nau::type_list
