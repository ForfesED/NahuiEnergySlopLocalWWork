// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <exception>
#include <type_traits>

#include "nau/diag/assertion.h"
#include "nau/kernel/kernel_config.h"
#include "nau/utils/preprocessor.h"

namespace nau::rt_detail
{

    // template<typename F>
    // concept ScopeGuardCallback = std::is_invocable_r_v<void, F>;

    /// <summary>
    ///
    /// </summary>
    template <typename F>
    struct ScopeGuard_OnLeave
    {
        F callback;

        ScopeGuard_OnLeave(F callback_) :
            callback(std::move(callback_))
        {
        }

        ~ScopeGuard_OnLeave() noexcept(noexcept(callback()))
        {
            callback();
        }
    };

    template <typename F>
    ScopeGuard_OnLeave(F) -> ScopeGuard_OnLeave<F>;

#if 1

    #if defined(__APPLE__)  //

    template <typename F>
    struct ScopeGuard_OnSuccess
    {
        F callback;

        ScopeGuard_OnSuccess(F callback_) :
            callback(std::move(callback_))
        {
            G_ASSERTF(!std::uncaught_exception(), "SCOPE Success can not be declared while there is uncaught exception.");
        }

        ~ScopeGuard_OnSuccess() noexcept(noexcept(callback()))
        {
            if(!std::uncaught_exception())
            {
                callback();
            }
        }
    };

    #else

    template <typename F>
    struct ScopeGuard_OnSuccess
    {
        F callback;
        const int exceptionsCount = std::uncaught_exceptions();

        ScopeGuard_OnSuccess(F callback_) :
            callback(std::move(callback_))
        {
            NAU_ASSERT(std::uncaught_exceptions() == 0, "SCOPE Success can not be declared while there is uncaught exception.");
        }

        ~ScopeGuard_OnSuccess() noexcept(noexcept(callback()))
        {
            if(exceptionsCount == std::uncaught_exceptions())
            {
                callback();
            }
        }
    };

    #endif  // __APPLE__

    template <typename F>
    ScopeGuard_OnSuccess(F) -> ScopeGuard_OnSuccess<F>;

    #if defined(__APPLE__)  //
    template <typename F>
    struct ScopeGuard_OnFail
    {
        F callback;

        ScopeGuard_OnFail(F callback_) :
            callback(std::move(callback_))
        {
            static_assert(noexcept(callback()), "Failure scope guard must be noexcept(true)");
            G_ASSERTF(!std::uncaught_exception(), "SCOPE Fail can not be declared while there is uncaught exception.");
        }

        ~ScopeGuard_OnFail() noexcept
        {
            if(std::uncaught_exception())
            {
                callback();
            }
        }
    };

    #else
    template <typename F>
    struct ScopeGuard_OnFail
    {
        F callback;
        const int exceptionsCount = std::uncaught_exceptions();

        ScopeGuard_OnFail(F callback_) :
            callback(std::move(callback_))
        {
            static_assert(noexcept(callback()), "Failure scope guard must be noexcept(true)");
        }

        ~ScopeGuard_OnFail() noexcept
        {
            if(exceptionsCount < std::uncaught_exceptions())
            {
                callback();
            }
        }
    };

    #endif

    template <typename F>
    ScopeGuard_OnFail(F) -> ScopeGuard_OnFail<F>;

#endif  //

    struct ExprBlock
    {
        template <typename F>
        inline constexpr friend decltype(auto) operator+(const ExprBlock&, F f)
        {
            static_assert(std::is_invocable_v<F>);
            return f();
        }
    };

}  // namespace nau::rt_detail

#define SCOPE_ON_FAIL \
    ::nau::rt_detail::ScopeGuard_OnFail ANONYMOUS_VAR(onScopeFailure__) = [&]() noexcept

#define SCOPE_ON_SUCCESS \
    ::nau::rt_detail::ScopeGuard_OnSuccess ANONYMOUS_VAR(onScopeSuccess__) = [&]

#define SCOPE_ON_LEAVE \
    ::nau::rt_detail::ScopeGuard_OnLeave ANONYMOUS_VAR(onScopeLeave__) = [&]

#define scope_on_leave SCOPE_ON_LEAVE
#define scope_on_success SCOPE_ON_SUCCESS
#define scope_on_fail SCOPE_ON_FAIL

#define EXPR_Block ::nau::rt_detail::ExprBlock{} + [&]()
