// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <optional>
#include <chrono>
#include "EASTL/shared_ptr.h"

#include "nau/kernel/kernel_config.h"

namespace nau::rt_detail
{
    class CancellationState;
    class ExpirationState;

}  // namespace nau::rt_detail

namespace nau
{

    /**
     */
    class NAU_KERNEL_EXPORT [[nodiscard]] CancellationSubscription final
    {
    public:
        ~CancellationSubscription();

        CancellationSubscription();

        CancellationSubscription(CancellationSubscription&&);

        CancellationSubscription(const CancellationSubscription&) = delete;

        CancellationSubscription& operator= (CancellationSubscription&&);

        CancellationSubscription& operator= (const CancellationSubscription&) = delete;

        CancellationSubscription& operator= (std::nullptr_t);

        explicit operator bool () const;

        void reset();

    private:
        CancellationSubscription(eastl::shared_ptr<rt_detail::CancellationState>, uintptr_t);

        eastl::shared_ptr<rt_detail::CancellationState> m_cancellation;
        uintptr_t m_subscriptionHandle = 0;

        friend class Cancellation;
    };

    /**
     */
    class NAU_KERNEL_EXPORT [[nodiscard]] ExpirationSubscription final
    {
    public:
        ~ExpirationSubscription();

        ExpirationSubscription();

        ExpirationSubscription(ExpirationSubscription&&);

        ExpirationSubscription(const ExpirationSubscription&) = delete;

        ExpirationSubscription& operator= (ExpirationSubscription&&);

        ExpirationSubscription& operator= (const ExpirationSubscription&) = delete;

        explicit operator bool () const;

        void reset();

    private:
        ExpirationSubscription(eastl::shared_ptr<rt_detail::ExpirationState>, uintptr_t);

        eastl::shared_ptr<rt_detail::ExpirationState> m_expiration;
        uintptr_t m_subscriptionHandle = 0;

        friend class Expiration;
    };

    /**
     */
    class NAU_KERNEL_EXPORT Cancellation final
    {
    public:
        static Cancellation none();

        ~Cancellation();

        Cancellation();

        Cancellation(std::nullptr_t)
        {
        }

        Cancellation(Cancellation&&);

        Cancellation(const Cancellation&);

        Cancellation& operator= (Cancellation&&);

        Cancellation& operator= (const Cancellation&);

        bool isCancelled() const;

        bool isEternal() const;

        CancellationSubscription subscribe(void (*callback)(void*), void* callbackData);

    private:
        Cancellation(eastl::shared_ptr<rt_detail::CancellationState> token);

        eastl::shared_ptr<rt_detail::CancellationState> m_cancellation;

        friend class CancellationSource;
        friend class Expiration;
    };

    /**
     */
    class NAU_KERNEL_EXPORT CancellationSource final
    {
    public:
        ~CancellationSource();

        CancellationSource();

        CancellationSource(std::nullptr_t);

        CancellationSource(CancellationSource&&);

        CancellationSource(const CancellationSource&) = delete;

        CancellationSource& operator= (CancellationSource&&);

        CancellationSource& operator= (const CancellationSource&) = delete;

        explicit operator bool () const;

        Cancellation getCancellation();

        bool isCancelled() const;

        void cancel();

        template <typename Rep, typename Period>
        void setTimeout(std::chrono::duration<Rep, Period> timeout)
        {
            using namespace std::chrono;

            setTimeoutInternal(duration_cast<milliseconds>(timeout));
        }

    private:
        void SetTimeoutInternal(std::chrono::milliseconds);

        eastl::shared_ptr<rt_detail::CancellationState> m_cancellation;
    };

    /**
     */
    class NAU_KERNEL_EXPORT Expiration
    {
    public:
        static Expiration never();

        Expiration(Cancellation cancellation, std::chrono::milliseconds timeout);

        Expiration(Cancellation cancellation);

        Expiration(std::chrono::milliseconds timeout);

        bool isExpired() const;

        bool isEternal() const;

        ExpirationSubscription subscribe(void (*)(void*), void*);

        std::optional<std::chrono::milliseconds> getTimeout() const;

    private:
        Expiration();

        eastl::shared_ptr<rt_detail::ExpirationState> m_expiration;
    };

}  // namespace nau
