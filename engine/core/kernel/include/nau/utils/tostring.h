// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/utils/type_utility.h"

#include <string>
#include <string_view>

namespace nau
{

    template <typename T>
    concept StringConvertible =
        requires(const T& value) {
            { toString(value) } -> std::convertible_to<std::string>;
        };

    template <typename T>
    concept StringParsable =
        requires(T& value) {
            parseString(value, std::string_view{});
        };

    // namespace rt_detail {

    // template<typename T>
    // decltype(ToString(ConstLValueRef<T>()), std::true_type{}) ToStringResultHelper(int);

    // template<typename>
    // std::false_type ToStringResultHelper(...);

    // template<typename T>
    // decltype(Parse(LValueRef<T>(), std::string_view{}), std::true_type{}) ParseResultHelper(int);

    // template<typename>
    // std::false_type ParseResultHelper(...);

    // } // namespace rt_detail

    // template<typename T>
    // inline constexpr bool HasToString = decltype(RtDetail::ToStringResultHelper<std::remove_const_t<T>>(int{}))::value;

    // template<typename T>
    // inline constexpr bool HasParse = decltype(RtDetail::ParseResultHelper<std::remove_const_t<T>>(int{}))::value;

    // template<typename T>
    // inline constexpr bool IsStringSerializable = HasToString<T> && HasParse<T>;

    // template<typename T>
    // concept StringRepresentable = requires(const T &value) {
    //	{ toString(value) } -> internal__::Convertible<std::string>;
    // };
    //
    //
    // template<typename T>
    // concept WStringRepresentable = requires(const T &value) {
    //	{ toWString(value) } -> internal__::Convertible<std::wstring>;
    // };
    //
    //
    //
    // template<typename T, typename C>
    // concept StringRepresentableT =
    //	(std::is_same_v<C, char> && StringRepresentable<T>) ||
    //	(std::is_same_v<C, wchar_t> && WStringRepresentable<T>)
    //	;
    //
    //
    //
    // template<typename C, typename T>
    // inline std::basic_string<C> toStringT(const T& value) requires StringRepresentableT<T,C> {
    //	if constexpr (std::is_same_v<C, char>) {
    //		return toString(value);
    //	}
    //	else {
    //		return toWString(value);
    //	}
    // }

}  // namespace nau

namespace std
{

    inline auto toString(std::string_view str)
    {
        return std::string{str};
    }

    template <typename... Traits>
    inline decltype(auto) toString(std::basic_string<char, Traits...>& str)
    {
        return (str);
    }

    template <typename... Traits>
    inline bool parseString(std::basic_string<char, Traits...>& target, std::string_view str)
    {
        target = str;
        return true;
    }

}  // namespace std
