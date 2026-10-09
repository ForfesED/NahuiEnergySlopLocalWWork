// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <array>
#include <exception>
#include <type_traits>
#include <typeinfo>
#include <utility>

#include "nau/diag/assertion.h"
#include "nau/diag/error.h"
#include "nau/memory/mem_allocator.h"
#include "nau/kernel/kernel_config.h"
#include "nau/utils/scope_guard.h"
#include "nau/utils/type_utility.h"

namespace nau
{

    template <typename T = void>
    class Result;

    /**
     */
    template <typename T>
    class [[nodiscard]] Result
    {
        static_assert(!std::is_same_v<T, std::exception_ptr>, "std::exception_ptr is not acceptable type for Result<>");
        static_assert(!IsTemplateOf<Result, T>, "Result is not acceptable type for Result<>");

    public:
        using ValueType = T;

        Result()
            requires(std::is_default_constructible_v<T>)
            :
            m_value(std::in_place)
        {
        }

        Result(const Result& other)
            requires(std::is_copy_constructible_v<T>)
            :
            m_error(other.m_error),
            m_value(other.m_value)
        {
        }

        Result(Result&& other)
            requires(std::is_move_constructible_v<T>)
            :
            m_error(std::move(other.m_error)),
            m_value(std::move(other.m_value))
        {
        }

        template <typename U>
            requires(!std::is_same_v<T, U> && std::is_constructible_v<T, const U&>)
        Result(const Result<U>& other)
        {
            if(other.isError())
            {
                m_error = other.getError();
            }
            else
            {
                emplace(*other);
            }
        }

        template <typename U>
            requires(!std::is_same_v<T, U> && std::is_constructible_v<T, U &&>)
        Result(Result<U>&& other)
        {
            if(other.isError())
            {
                m_error = other.getError();
            }
            else
            {
                emplace(*std::move(other));
            }

        }

        template <typename U>
            requires(std::is_constructible_v<T, U &&>)
        Result(U&& value) noexcept
        {
            emplace(std::forward<U>(value));
        }
        /**
            construct in-place
        */
        template <typename... A>
            requires(std::is_constructible_v<T, A...>)
        Result(A&&... arg) :
            m_value(std::in_place, std::forward<A>(arg)...)
        {
        }

        /**
            construct error
        */
        template <ErrorConcept U>
        Result(Error::PtrType<U> error) :
            m_error(std::move(error))
        {
        }

        Result& operator=(const Result&) = default;

        Result& operator=(Result&& other) noexcept
            requires(std::is_move_assignable_v<T>)
        {
            m_error = std::move(other.m_error);
            m_value = std::move(other.m_value);

            return *this;
        }

        template <typename U>
            requires(!std::is_same_v<U, T> && std::is_assignable_v<T&, const U&>)
        Result& operator=(const Result<U>& other)
        {
            if(other.isError())
            {
                m_value.reset();
                m_error = other.getError();
            }
            else
            {
                assign(*other);
            }

            return *this;
        }

        /**
            move assign
        */
        template <typename U>
            requires(!std::is_same_v<U, T> && std::is_assignable_v<T&, U &&>)
        Result& operator=(Result<U>&& other)
        {
            if(other.isError())
            {
                m_value.reset();
                m_error = other.getError();
            }
            else
            {
                m_error.reset();
                assign(std::move(*other));
            }

            return *this;
        }

        template <typename U>
            requires(std::is_assignable_v<T&, U &&>)
        Result& operator=(U&& value)
        {
            m_error.reset();
            assign(std::forward<U>(value));

            return *this;
        }

        template <typename U>
            requires(IsError<U>)
        Result& operator=(Error::PtrType<U> error)
        {
            NAU_ASSERT(error);

            m_value.reset();
            m_error = std::move(error);
            return *this;
        }

        template <typename... A>
            requires(std::is_constructible_v<T, A...>)
        void emplace(A&&... args)
        {
            NAU_ASSERT(!m_error);
            m_value.emplace(std::forward<A>(args)...);
        }

        bool isError() const
        {
            return static_cast<bool>(m_error);
        }

        nau::Error::Ptr getError() const
        {
            NAU_ASSERT(isError(), "Result<T> has no error");
            return m_error;
        }

        void ignore() const noexcept
        {
            NAU_ASSERT(!m_error, "Ignoring Result<T> that holds an error:{}", m_error->getMessage());
        }

        const T& operator*() const&
        {
            NAU_ASSERT(m_value, "Result<T> is valueless");
            return *m_value;
        }

        T& operator*() &
        {
            NAU_ASSERT(m_value, "Result<T> is valueless");
            return *m_value;
        }

        T&& operator*() &&
        {
            NAU_ASSERT(m_value, "Result<T> is valueless");
            return std::move(*m_value);
        }

        const T* operator->() const
        {
            NAU_ASSERT(m_value, "Result<T> is valueless");
            return &(*m_value);
        }

        T* operator->()
        {
            NAU_ASSERT(m_value, "Result<T> is valueless");
            return &(*m_value);
        }

        explicit operator bool() const
        {
            return m_value.has_value();
        }

    private:
        template <typename U>
        void assign(U&& value)
        {
            static_assert(std::is_assignable_v<T&, U>);
            m_error.reset();

            if(m_value)
            {
                *m_value = std::forward<U>(value);
            }
            else
            {
                m_value.emplace(std::forward<U>(value));
            }
        }

        Error::Ptr m_error = nullptr;
        std::optional<T> m_value;
    };

    /**
     */
    template <>
    class NAU_KERNEL_EXPORT [[nodiscard]] Result<void>
    {
    public:
        using ValueType = void;

        Result() = default;
        Result(const Result<>&) = default;
        Result(Result&& other);

        template <typename U>
            requires(IsError<U>)
        Result(Error::PtrType<U> error) :
            m_error(std::move(error))
        {
            NAU_ASSERT(m_error);
        }

        Result<>& operator=(const Result&) = default;
        Result<>& operator=(Result&& other) noexcept
        {
            m_error = std::move(other.m_error);
            return *this;
        }

        template <typename U>
            requires(IsError<U>)
        Result<>& operator=(Error::PtrType<U> error)
        {
            NAU_ASSERT(error);
            m_error = std::move(error);
            return *this;
        }

        explicit operator bool() const;

        bool isError() const;

        bool isSuccess(Error::Ptr* = nullptr) const;

        nau::Error::Ptr getError() const;

        void ignore() const noexcept;

    private:
        Error::Ptr m_error;
    };

    template <typename T>
    inline constexpr bool IsResult = nau::IsTemplateOf<Result, std::decay_t<T>>;

    inline static Result<> ResultSuccess {};

}  // namespace nau


#define NauCheckResult(expr)                                                                           \
    {                                                                                                  \
        decltype(auto) exprResult = (expr);                                                            \
        static_assert(::nau::IsTemplateOf<::nau::Result, decltype(exprResult)>, "Expected Result<T>"); \
        if(exprResult.isError())                                                                       \
        {                                                                                              \
            return exprResult.getError();                                                              \
        }                                                                                              \
    }
