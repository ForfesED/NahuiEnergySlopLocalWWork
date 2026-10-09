// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

#include "nau/diag/assertion.h"
#include "nau/utils/type_list/type_list.h"

namespace nau
{
    namespace nau_detail
    {

        template <typename>
        struct TupleFrom;

        template <typename... T>
        struct TupleFrom<TypeList<T...>>
        {
            using type = std::tuple<T...>;
        };

        template <typename T>

        struct UniformTupleHelper : std::false_type
        {
        };

        template <typename U, size_t Sz>
        struct UniformTupleHelper<std::array<U, Sz>> : std::true_type
        {
        };

        template <typename T, typename... U>
        requires(std::is_same_v<T, U> && ...)
        struct UniformTupleHelper<std::tuple<T, U...>> : std::true_type
        {
        };

    }  // namespace nau_detail

    /**
     */
    template <typename T>
    concept UniformTuple = nau_detail::UniformTupleHelper<T>::value;

    /**
     */
    struct TupleUtils
    {
        template <typename T>
        using Indexes = std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>>;

        template <typename T>
        using TupleFrom = typename nau_detail::TupleFrom<T>::type;

        template <typename T>
        static consteval auto getIndexes(const T)
        {
            return Indexes<T>{};
        }

        /*
         */
        template <typename T, typename Element>
        consteval static bool contains()
        {
            return TupleUtils::containsHelper<T, Element>(Indexes<T>{});
        }

        template <typename Tuple_, typename F>
        static void invokeAt(Tuple_& tuple, size_t index, F accessor)
        {
            constexpr size_t Size = std::tuple_size_v<Tuple_>;
            static_assert(Size > 0, "Can not deal with zero-size tuple.");
            NAU_ASSERT(index < Size);

            [[maybe_unused]]
            const bool invoked = invokeAtHelper(tuple, index, accessor, TupleUtils::Indexes<Tuple_>{});
            NAU_ASSERT(invoked);
        }

        template <typename Tup, typename Callable, typename... Args>
        static void forEach(Tup&& tup, Callable c, Args&&... args)
        {
            TupleUtils::forEachHelper(std::forward<Tup>(tup), c, Indexes<Tup>{}, std::forward<Args>(args)...);
        }

    private:
        template <typename T, size_t Index>
        using El = std::remove_cv_t<std::tuple_element_t<Index, T>>;

        template <typename T, typename Element, size_t... I>
        constexpr static bool containsHelper(std::index_sequence<I...>)
        {
            return (std::is_same_v<Element, El<T, I>> || ...);
        }

        template <typename Tuple, typename F, size_t... Indexes>
        static bool invokeAtHelper(Tuple& tuple, size_t index, F accessor, std::index_sequence<Indexes...>)
        {
            const auto TryInvokeElement = [&](auto currentElementIndex) -> bool
            {
                constexpr size_t CurrentI = decltype(currentElementIndex)::value;
                using Element = std::add_lvalue_reference_t<std::tuple_element_t<CurrentI, Tuple>>;
                static_assert(std::is_invocable_r_v<void, F, Element>, "Tuple accessor can not be invoked for specific element");

                if(CurrentI != index)
                {
                    return false;
                }

                decltype(auto) element = std::get<CurrentI>(tuple);
                accessor(element);
                return true;
            };

            return (TryInvokeElement(std::integral_constant<size_t, Indexes>{}) || ...);
        }

        template <typename Tup, typename Callable, size_t... Index, typename... Args>
        static inline void forEachHelper(Tup&& tup, Callable& callback, std::index_sequence<Index...>, Args&&... args)
        {
            (callback(std::get<Index>(std::forward<Tup>(tup)), std::forward<Args>(args)...), ...);
        }
    };

}  // namespace nau
