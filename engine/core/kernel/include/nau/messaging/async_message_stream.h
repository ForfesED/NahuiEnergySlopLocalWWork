// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/messaging/async_message_stream.h


#pragma once

#include <EASTL/string.h>
#include <EASTL/string_view.h>

#include "nau/async/task.h"
#include "nau/kernel/kernel_config.h"
#include "nau/runtime/async_disposable.h"
#include "nau/runtime/disposable.h"
#include "nau/serialization/runtime_value.h"
#include "nau/utils/functor.h"

namespace nau
{
    class NAU_KERNEL_EXPORT AsyncMessageStream
    {
    public:
        AsyncMessageStream();

        AsyncMessageStream(AsyncMessageStream&&) noexcept;

        AsyncMessageStream(const AsyncMessageStream&) = delete;

        ~AsyncMessageStream();

        AsyncMessageStream& operator=(AsyncMessageStream&&) noexcept;

        AsyncMessageStream& operator=(std::nullptr_t) noexcept;

        AsyncMessageStream& operator=(const AsyncMessageStream&) = delete;

        explicit operator bool() const;

        eastl::string_view getStreamName() const;

        async::Task<RuntimeValue::Ptr> getNextMessage();

        void reset();

    private:
        AsyncMessageStream(nau::Ptr<class AsyncMessageStreamImpl>&&);

        nau::Ptr<class AsyncMessageStreamImpl> m_stream;

        friend class AsyncMessageSourceImpl;

        template <typename>
        friend class TypedMessageStream;
    };

#if 0
    class AsyncMessageSubscription final : public virtual IAsyncDisposable
    {
        // NAU_RTTI_CLASS(nau::AsyncMessageSubscription, IAsyncDisposable)
    public:
        AsyncMessageSubscription();

        template <typename Callable>
        AsyncMessageSubscription(AsyncMessageSource&, const std::string& streamName, Callable, async::Executor::Ptr);

        AsyncMessageSubscription(AsyncMessageSubscription&&);

        ~AsyncMessageSubscription();

        explicit operator bool() const;

        AsyncMessageSubscription& operator=(std::nullptr_t);

        AsyncMessageSubscription& operator=(AsyncMessageSubscription&&);

        void reset();

        async::Task<> disposeAsync() override;

    private:
        async::Task<> m_task;
        CancellationSource m_cancellationSource;
    };
#endif

    /**
     */
    struct NAU_ABSTRACT_TYPE AsyncMessageSource : virtual IRefCounted,
                                                  virtual IDisposable
    {
        NAU_INTERFACE(nau::AsyncMessageSource, IRefCounted, IDisposable);

        using Ptr = nau::Ptr<AsyncMessageSource>;

        NAU_KERNEL_EXPORT
        static Ptr create();

        virtual void setCancellation(Cancellation) = 0;

        virtual bool hasSubscribers(eastl::string_view) const = 0;

        virtual AsyncMessageStream getStream(eastl::string_view streamName) = 0;

        // virtual subscribeInplace(Functor<void (const Runtime::Ptr&)) = 0;

        virtual void post(eastl::string_view streamName, RuntimeValue::Ptr = nullptr) = 0;

        /**
                template <typename T>
                TypedMessageStream<T> getTypedStream(const std::string& streamName);

                template <typename T>
                TypedMessageStream<T> getTypedStream();

                template <typename Callable>
                async::Task<> subscribeAsTask(const std::string& streamName, Callable handler, async::Executor::Ptr = nullptr, Cancellation = Cancellation::none());

                template <typename Callable>
                AsyncMessageSubscription subscribe(const std::string& streamName, Callable handler, async::Executor::Ptr = nullptr);

                template <typename T>
                void post(const std::string& name, T value);

                template <typename T>
                void post(T messageValue);
        */
        /**
            sends void message
        */
    };
}  // namespace nau
