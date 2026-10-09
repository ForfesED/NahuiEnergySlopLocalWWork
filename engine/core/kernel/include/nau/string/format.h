// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/service/format.h


#pragma once

#include "EASTL/string.h"
#include "fmt/chrono.h"
#include "fmt/color.h"
#include "fmt/xchar.h"

namespace nau::utils
{
    namespace details
    {

        template <typename T>
        requires(!std::is_constructible_v<eastl::string_view, T>) &&
                (!std::is_constructible_v<std::string_view, T>) &&
                (!std::is_constructible_v<eastl::u8string_view, T>) &&
                (!std::is_constructible_v<std::u8string_view, T>)
        static inline auto make_formatable_string_view(T&& arg)
        {
            return std::forward<T>(arg);
        }

        template <typename T>
        requires std::is_constructible_v<eastl::string_view, T>
        static inline eastl::string_view make_formatable_string_view(T&& arg)
        {
            return eastl::string_view{std::forward<T>(arg)};
        }

        template <typename T>
        requires(!std::is_constructible_v<eastl::string_view, T>) &&
                std::is_constructible_v<std::string_view, T>
        static inline eastl::string_view make_formatable_string_view(T&& arg)
        {
            auto strV = std::string_view{std::forward<T>(arg)};
            return eastl::string_view(strV.data(), strV.size());
        }

        template <typename T>
        requires std::is_constructible_v<eastl::u8string_view, T>
        static inline eastl::string_view make_formatable_string_view(T&& arg)
        {
            auto strV8 = eastl::u8string_view{std::forward<T>(arg)};
            return eastl::string_view(reinterpret_cast<const char*>(strV8.data()), strV8.size());
        }

        template <typename T>
        requires(!std::is_constructible_v<eastl::u8string_view, T>) &&
                std::is_constructible_v<std::u8string_view, T>
        static inline eastl::string_view make_formatable_string_view(T&& arg)
        {
            auto strV8 = std::u8string_view{std::forward<T>(arg)};
            return eastl::string_view(reinterpret_cast<const char*>(strV8.data()), strV8.size());
        }

        template <typename T>
        requires(!std::is_constructible_v<eastl::string_view, T>) &&
                (!std::is_constructible_v<std::string_view, T>) &&
                (!std::is_constructible_v<eastl::u8string_view, T>) &&
                (!std::is_constructible_v<std::u8string_view, T>) &&
                (!std::is_same_v<char8_t, std::remove_cvref_t<T>>)
        static inline auto make_formatable_arg(T&& arg)
        {
            return std::forward<T>(arg);
        }

        template <typename T>
        requires(std::is_same_v<char8_t, std::remove_cvref_t<T>>)
        static inline auto make_formatable_arg(T&& arg)
        {
            return char(std::forward<T>(arg));
        }

        template <typename T>
        requires std::is_constructible_v<eastl::string_view, T>
        static inline const char* make_formatable_arg(T&& arg)
        {
            return eastl::string_view{std::forward<T>(arg)}.data();
        }

        template <typename T>
        requires(!std::is_constructible_v<eastl::string_view, T>) &&
                std::is_constructible_v<std::string_view, T>
        static inline const char* make_formatable_arg(T&& arg)
        {
            return std::string_view{std::forward<T>(arg)}.data();
        }

        template <typename T>
        requires std::is_constructible_v<eastl::u8string_view, T>
        static inline const char* make_formatable_arg(T&& arg)
        {
            auto strV8 = eastl::u8string_view{std::forward<T>(arg)};
            return eastl::string_view(reinterpret_cast<const char*>(strV8.data()), strV8.size()).data();
        }

        template <typename T>
        requires(!std::is_constructible_v<eastl::u8string_view, T>) &&
                std::is_constructible_v<std::u8string_view, T>
        static inline const char* make_formatable_arg(T&& arg)
        {
            auto strV8 = std::u8string_view{std::forward<T>(arg)};
            return eastl::string_view(reinterpret_cast<const char*>(strV8.data()), strV8.size()).data();
        }
    };  // namespace details

    template <class S, typename... Args>
    static inline eastl::string format(S&& formatString)
    {
        auto strV = details::make_formatable_string_view(std::forward<S>(formatString));
        return eastl::string(strV.data(), strV.size());
    }

    template <class S, typename... Args>
    static inline eastl::string format(S&& formatString, Args&&... args)
    {
        auto strV = details::make_formatable_string_view(std::forward<S>(formatString));
        return fmt::vformat(fmt::basic_string_view<char>{strV.data(), strV.size()},
                            fmt::make_format_args<fmt::buffered_context<char>>(details::make_formatable_arg<Args>(std::forward<Args>(args))...))
            .c_str();
    }

}  // namespace nau::utils