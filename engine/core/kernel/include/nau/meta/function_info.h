// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/meta/function_info.h


#pragma once

#include <type_traits>

#include "nau/utils/type_list.h"

namespace nau::meta_detail
{
    template <typename F>
    decltype(&F::operator(), std::true_type{}) checkIsFunctorHelper(int);

    template <typename F>
    std::false_type checkIsFunctorHelper(...);

    template <typename F>
    constexpr bool IsFunctor = decltype(checkIsFunctorHelper<F>(int{}))::value;

}  // namespace nau::meta_detail

namespace nau::meta
{

    template <typename F>
    inline constexpr bool IsMemberFunction = std::is_member_function_pointer_v<F>;

    template <typename F>
    inline constexpr bool IsFunctor = meta_detail::IsFunctor<F>;

    /*
     */
    template <bool IsConst, bool IsNoExcept, typename InstanceClass, typename Res, typename... P>
    struct CallableTypeInfo
    {
        using Class = InstanceClass;
        using Result = Res;
        using ParametersList = TypeList<P...>;

        inline constexpr static bool Const = IsConst;
        inline constexpr static bool NoExcept = IsNoExcept;
    };
}  // namespace nau::meta

namespace nau::meta_detail
{
    template <typename F>
    struct CallableTypeInfoT;

    // Functor
    template <typename R, typename... P>
    struct CallableTypeInfoT<R(P...)> : meta::CallableTypeInfo<false, false, std::nullptr_t, R, P...>
    {
    };

    template <typename R, typename... P>
    struct CallableTypeInfoT<R(P...) noexcept> : meta::CallableTypeInfo<false, true, std::nullptr_t, R, P...>
    {
    };

    template <typename R, typename... P>
    struct CallableTypeInfoT<R(P...) const> : meta::CallableTypeInfo<true, false, std::nullptr_t, R, P...>
    {
    };

    template <typename R, typename... P>
    struct CallableTypeInfoT<R(P...) const noexcept> : meta::CallableTypeInfo<true, true, std::nullptr_t, R, P...>
    {
    };

    // Function pointer
    template <typename R, typename... P>
    struct CallableTypeInfoT<R (*)(P...)> : meta::CallableTypeInfo<false, false, std::nullptr_t, R, P...>
    {
    };

    template <typename R, typename... P>
    struct CallableTypeInfoT<R (*)(P...) noexcept> : meta::CallableTypeInfo<false, true, std::nullptr_t, R, P...>
    {
    };

    // Member function pointer
    // non const instance
    template <typename Class, typename R, typename... P>
    struct CallableTypeInfoT<R (Class::*)(P...)> : meta::CallableTypeInfo<false, false, Class, R, P...>
    {
    };

    template <typename Class, typename R, typename... P>
    struct CallableTypeInfoT<R (Class::*)(P...) noexcept> : meta::CallableTypeInfo<false, true, Class, R, P...>
    {
    };

    template <typename Class, typename R, typename... P>
    struct CallableTypeInfoT<R (Class::*)(P...) const> : meta::CallableTypeInfo<true, false, Class, R, P...>
    {
    };

    template <typename Class, typename R, typename... P>
    struct CallableTypeInfoT<R (Class::*)(P...) const noexcept> : meta::CallableTypeInfo<true, true, Class, R, P...>
    {
    };

    // const instance
    template <typename Class, typename R, typename... P>
    struct CallableTypeInfoT<R (Class::*const)(P...)> : meta::CallableTypeInfo<false, false, Class, R, P...>
    {
    };

    template <typename Class, typename R, typename... P>
    struct CallableTypeInfoT<R (Class::*const)(P...) noexcept> : meta::CallableTypeInfo<false, true, Class, R, P...>
    {
    };

    template <typename Class, typename R, typename... P>
    struct CallableTypeInfoT<R (Class::*const)(P...) const> : meta::CallableTypeInfo<true, false, Class, R, P...>
    {
    };

    template <typename Class, typename R, typename... P>
    struct CallableTypeInfoT<R (Class::*const)(P...) const noexcept> : meta::CallableTypeInfo<true, true, Class, R, P...>
    {
    };

    template <typename F, bool = meta::IsFunctor<F>>
    struct GetCallableTypeInfo
    {
        using type = CallableTypeInfoT<decltype(&F::operator())>;
    };

    template <typename F>
    struct GetCallableTypeInfo<F, false>
    {
        using type = CallableTypeInfoT<F>;
    };

    template <typename T>
    decltype(typename GetCallableTypeInfo<T>::type{}, std::true_type{}) checkIsCallableHelper(int);

    template <typename T>
    std::false_type checkIsCallableHelper(...);

    template <typename T>
    inline constexpr bool IsCallable = decltype(checkIsCallableHelper<T>(int{}))::value;

}  // namespace nau::meta_detail

namespace nau::meta
{
    template <typename F>
    using GetCallableTypeInfo = typename meta_detail::GetCallableTypeInfo<std::decay_t<F>>::type;

    template <typename T>
    inline constexpr bool IsCallable = meta_detail::IsCallable<T>;

}  // namespace nau::meta
