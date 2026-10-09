// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <optional>
#include <tuple>

#include "nau/async/async_timer.h"
#include "nau/async/core/core_task_linked_list.h"
#include "nau/async/cpp_coroutine.h"
#include "nau/async/executor.h"
#include "nau/async/task_base.h"
#include "nau/diag/error.h"
#include "nau/kernel/kernel_config.h"
#include "nau/utils/cancellation.h"
#include "nau/utils/scope_guard.h"
#include "nau/utils/type_utility.h"

namespace nau::async_detail
{

    /**
     */
    struct TaskPromiseTag
    {
    };

    /**
     */
    template <typename>
    struct TaskPromise;

    template <typename>
    struct RvTaskResultAwaiter;

    template <typename>
    struct LvTaskResultAwaiter;

    template <typename T>
    decltype(getTaskAwait(std::declval<T>()), std::true_type{}) hasTaskAwaitHelper(int);

    template <typename>
    std::false_type hasTaskAwaitHelper(...);

    template <typename T>
    inline constexpr bool HasTaskAwait = decltype(hasTaskAwaitHelper<std::remove_const_t<T>>(int{}))::value;

    /**
     */
    template <typename T>
    struct TaskAwaiter
    {
        async::CoreTask::Ptr coreTaskPtr;

        TaskAwaiter(async::CoreTask::Ptr ptr) :
            coreTaskPtr(std::move(ptr))
        {
        }

        bool await_ready() const noexcept;

        template <typename Promise>
        void await_suspend(std::coroutine_handle<Promise> coroutine) noexcept;

        T await_resume() const;
    };

    /**
     */
    template <typename T>
    struct TaskTryAwaiter
    {
        async::CoreTask::Ptr coreTaskPtr;

        TaskTryAwaiter(async::CoreTask::Ptr ptr) :
            coreTaskPtr(std::move(ptr))
        {
        }

        bool await_ready() const noexcept
        {
            auto task = async::Task<T>::fromCoreTask(coreTaskPtr);
            scope_on_leave
            {
                task = nullptr;
            };

            return task.isReady();
        }

        void await_suspend(std::coroutine_handle<> coroutine) noexcept
        {
            using namespace nau::async;
            NAU_ASSERT(coreTaskPtr);

            CoreTask* const coreTask = getCoreTask(coreTaskPtr);
            NAU_ASSERT(coreTask);

            TaskContinuation continuation{Executor::Invocation::fromCoroutine(coroutine), Executor::getCurrent()};
            coreTask->setContinuation(std::move(continuation));
        }

        Result<T> await_resume() const
        {
            NAU_ASSERT(coreTaskPtr);

            async::Task<T> task = async::Task<T>::fromCoreTask(coreTaskPtr);
            scope_on_leave
            {
                task = nullptr;
            };

            NAU_ASSERT(task.isReady());
            if (!task.isReady())
            {
                return NauMakeError("Task is not ready");
            }

            return task.asResult();
        }
    };

    /**

    */
    struct DelayAwaiter
    {
        const std::chrono::milliseconds delay;

        template <typename R, typename P>
        DelayAwaiter(std::chrono::duration<R, P> delay_) :
            delay(std::chrono::duration_cast<std::chrono::milliseconds>(delay_))
        {
        }

        inline bool await_ready() const noexcept
        {
            return delay == std::chrono::milliseconds(0);
        }

        template <typename Promise>
        void await_suspend(std::coroutine_handle<Promise> continuation) const noexcept;

        inline void await_resume() const noexcept
        {
        }
    };

    /**
     */
    struct ExpirationAwaiter
    {
        Expiration expiration;
        async::Executor::Ptr executor;
        std::coroutine_handle<> continuation = nullptr;
        ExpirationSubscription subscription;

        ExpirationAwaiter(Expiration&&);
        ExpirationAwaiter(const ExpirationAwaiter&) = delete;
        ExpirationAwaiter(ExpirationAwaiter&&) = default;

        bool await_ready() const noexcept;

        void await_suspend(std::coroutine_handle<> continuation);

        void await_resume() const noexcept;
    };

    /**
     */
    template <typename T>
    struct RvTaskResultAwaiter
    {
        Result<T> result;

        RvTaskResultAwaiter(Result<T>&& res) :
            result(std::move(res))
        {
        }

        bool await_ready() const
        {
            return static_cast<bool>(result);
        }

        template <typename U>
        void await_suspend(std::coroutine_handle<U>);

        T await_resume()
        {
            NAU_ASSERT(result);
            if constexpr (!std::is_same_v<void, T>)
            {
                return *std::move(result);
            }
            else
            {
                result.ignore();
            }
        }
    };

    /**
     */
    template <typename T>
    struct LvTaskResultAwaiter
    {
        const Result<T>& result;

        LvTaskResultAwaiter(const Result<T>& res) :
            result(res)
        {
        }

        bool await_ready() const
        {
            return !result.isError();
        }

        template <typename Promise>
        void await_suspend(std::coroutine_handle<Promise>);

        T await_resume()
        {
            NAU_FATAL(result);
            if constexpr (!std::is_same_v<void, T>)
            {
                return *result;
            }
            else
            {
                result.ignore();
            }
        }
    };

    struct CoroutineBreaker
    {
        bool await_ready() const noexcept
        {
            return false;
        }

        void await_resume() const noexcept
        {
            NAU_FAILURE_ALWAYS("Must never be called");
        }

        void await_suspend(std::coroutine_handle<> coroutine)
        {
            coroutine.destroy();
        }
    };

    /**

    */
    template <typename T>
    struct TaskPromise : TaskPromiseTag
    {
        async::TaskSource<T> taskSource;

        // There is need to reject task ONLY when coroutine will be actually destroyed.
        Error::Ptr errorOnDestroy;

        ~TaskPromise()
        {
            if (errorOnDestroy)
            {
                [[maybe_unused]]
                const bool rejected = taskSource.reject(std::move(errorOnDestroy));
                if (!rejected)
                {
                    // LOG_WARN(Core::Format::format("Multiple Task<{}> rejections", typeid(T).name()));
                }
            }
        }

        async::Task<T> get_return_object()
        {
            return taskSource.getTask();
        }

        std::suspend_never initial_suspend() const noexcept
        {
            return {};
        }

        std::suspend_never final_suspend() const noexcept
        {
            return {};
        }

        void unhandled_exception() noexcept
        {
#if RUNTIME_EXCEPTIONS
            taskSource.Reject(std::current_exception());
#else
            NAU_FAILURE_ALWAYS("Unhandled exception while no-exception support enabled");
#endif
        }
        template <typename E,
                  std::enable_if_t<IsError<E>, int> = 0>
        CoroutineBreaker yield_value(Error::PtrType<E> err) noexcept
        {
            static_assert(std::is_assignable_v<Error&, E&>, "Can not assign error type: private inheritance used ?");
            NAU_ASSERT(err);

            if (err)
            {
                this->taskSource.reject(std::move(err));
            }
            else
            {
                this->taskSource.reject(NauMakeError("Unspecified error"));
            }

            return {};
        }

        /**
            Task<>&& awaiter
        */
        template <typename U>
        static TaskAwaiter<U> await_transform(async::Task<U>&& task) noexcept
        {
            return {std::move(task)};
        }

        /**
            Task<> awaiter
        */
        template <typename U>
        static TaskAwaiter<U> await_transform(async::Task<U>& task) noexcept
        {
            return {task};
        }

        template <typename U>
        static TaskTryAwaiter<U> await_transform(async::TaskTryWrapper<U> tryWrapper)
        {
            return TaskTryAwaiter<U>{std::move(tryWrapper).getCoreTaskPtr()};
        }

        /**
            Scheduler awaiter:
        */
        static async::ExecutorAwaiter await_transform(async::ExecutorAwaiter awaiter) noexcept
        {
            return awaiter;
        }

        /**
            Scheduler awaiter:
        */
        static async::ExecutorAwaiter await_transform(async::Executor::Ptr scheduler) noexcept
        {
            return std::move(scheduler);
        }

        /**
            Scheduler awaiter:
        */
        static async::ExecutorAwaiter await_transform(async::Executor::WeakPtr scheduler) noexcept
        {
            return scheduler.acquire();
        }

        /**
            timeout
        */
        template <typename Rep, typename Period>
        static DelayAwaiter await_transform(std::chrono::duration<Rep, Period> delay) noexcept
        {
            return delay;
        }

        /**
            Expiration
        */
        static ExpirationAwaiter await_transform(Expiration expiration) noexcept
        {
            return ExpirationAwaiter{std::move(expiration)};
        }

        /**
            Result<T>&&
        */
        template <typename U>
        static RvTaskResultAwaiter<U> await_transform(Result<U>&& result) noexcept
        {
            return std::move(result);
        }

        /**
            Result<T>&
        */
        template <typename U>
        static LvTaskResultAwaiter<U> await_transform(const Result<U>& result) noexcept
        {
            return (result);
        }

        template <typename U,
                  std::enable_if_t<async_detail::HasTaskAwait<std::decay_t<U>>, int> = 0>
        static decltype(auto) await_transform(U&& awaitable)
        {
            return GetTaskAwait(std::forward<U>(awaitable));
        }
    };

    template <typename T>
    bool TaskAwaiter<T>::await_ready() const noexcept
    {
        auto task = async::Task<T>::fromCoreTask(coreTaskPtr);
        scope_on_leave
        {
            task = nullptr;
        };
        // If task already completed with error, then TaskAwaiter can not continue to execute coroutine (if error must be intercepted then Task<>::doTry() should be used).
        // To implement coroutine cancellation await_ready must return false to switch execution into await_suspend where coroutine will be destroyed and task will finish with error.
        const bool taskIsReady = task.isReady();

        if (taskIsReady && task.getError())
        {
            return false;
        }

        return taskIsReady;
    }

    template <typename T>
    template <typename Promise>
    void TaskAwaiter<T>::await_suspend(std::coroutine_handle<Promise> coroutine) noexcept
    {
        using namespace nau::async;
        NAU_ASSERT(this->coreTaskPtr);

        CoreTask* const coreTask = getCoreTask(this->coreTaskPtr);
        NAU_ASSERT(coreTask);

        auto& promise = coroutine.promise();
        auto& promiseTaskSource = promise.taskSource;

        if (coreTask->isReady())
        {
            // Case when task already finished with an error: coroutine can not continued and must be immediately destroyed (passing through error as result).
            if (auto error = coreTask->getError())
            {
                promiseTaskSource.reject(std::move(error));
                coroutine.destroy();
                return;
            }
        }

        coreTask->addRef();

        Executor::Invocation invoke{[](void* coroAddress, void* taskPtr) noexcept
        {
            CoreTask* const cTask = reinterpret_cast<CoreTask*>(taskPtr);

            auto error = cTask ? cTask->getError() : Error::Ptr{};
            if (cTask)
            {
                cTask->releaseRef();
            }

            std::coroutine_handle<Promise> coro = std::coroutine_handle<Promise>::from_address(coroAddress);

            if (error)
            {
                auto& adPromise = coro.promise();
                adPromise.errorOnDestroy = std::move(error);
                coro.destroy();
            }
            else
            {
                coro();
            }
        }, coroutine.address(), coreTask};

        TaskContinuation continuation{std::move(invoke), Executor::getCurrent()};

        coreTask->setContinuation(std::move(continuation));
    }

    template <typename T>
    T TaskAwaiter<T>::await_resume() const
    {
        NAU_ASSERT(coreTaskPtr);

        auto task = async::Task<T>::fromCoreTask(coreTaskPtr);
        NAU_ASSERT(task.isReady());
        NAU_ASSERT(!task.getError());

        scope_on_leave
        {
            task = nullptr;
        };

        if constexpr (std::is_same_v<void, T>)
        {
            task.rethrow();
        }
        else
        {
            return std::move(task).result();
        }
    }

    template <typename T>
    template <typename Promise>
    void LvTaskResultAwaiter<T>::await_suspend(std::coroutine_handle<Promise> continuation)
    {
        static_assert(std::is_base_of_v<TaskPromiseTag, Promise>, "Result<> can awaited only from within Task<> coroutine");

        NAU_ASSERT(this->result.isError());

        auto& promise = continuation.promise();
        promise.taskSource.reject(this->result.getError());
        continuation.destroy();
    }

    template <typename T>
    template <typename Promise>
    void RvTaskResultAwaiter<T>::await_suspend(std::coroutine_handle<Promise> continuation)
    {
        static_assert(std::is_base_of_v<TaskPromiseTag, Promise>, "Result<> can awaited only from within Task<> coroutine");

        NAU_ASSERT(this->result.isError());

        auto& promise = continuation.promise();
        promise.taskSource.reject(this->result.getError());
        continuation.destroy();
    }

    template <typename Promise>
    void DelayAwaiter::await_suspend(std::coroutine_handle<Promise> continuation) const noexcept
    {
        async::executeAfter(delay, async::Executor::getCurrent(), [](Error::Ptr error, void* addr) noexcept
        {
            auto coroutine = std::coroutine_handle<Promise>::from_address(addr);

            if (!error)
            {
                coroutine();
                return;
            }

            auto& promise = coroutine.promise();
            promise.errorOnDestroy = std::move(error);
            coroutine.destroy();
        }, continuation.address());
    }

    NAU_KERNEL_EXPORT bool waitInternal(async::CoreTaskPtr, std::optional<std::chrono::milliseconds>);

    NAU_KERNEL_EXPORT async::Task<bool> whenAllInternal(CoreTaskLinkedList, Expiration expiration);

    NAU_KERNEL_EXPORT async::Task<bool> whenAnyInternal(CoreTaskLinkedList, Expiration expiration);

}  // namespace nau::async_detail

namespace nau::async
{

    template <typename T>
    inline constexpr bool IsTask = IsTemplateOf<Task, T>;

    /**
     */
    template <typename T>
    inline bool wait(Task<T>& task, std::optional<std::chrono::milliseconds> timeout = std::nullopt)
    {
        return async_detail::waitInternal(static_cast<CoreTaskPtr&>(task), timeout);
    }

    template <typename T>
    inline decltype(auto) waitResult(Task<T> task)
    {
        NAU_ASSERT(task);
        [[maybe_unused]]
        const bool waitOk = async::wait(task);
        NAU_ASSERT(waitOk);

        return std::move(task).asResult();
    }

    template <typename T>
    inline decltype(auto) waitResult(std::reference_wrapper<Task<T>> taskReference)
    {
        decltype(auto) task = static_cast<Task<T>&>(taskReference);
        NAU_ASSERT(task);
        [[maybe_unused]]
        const bool waitOk = async::wait(task);
        NAU_ASSERT(waitOk);

        return task.asResult();
    }

    /**
     */
    template <typename Container>
    inline Task<bool> whenAll(Container& tasks, Expiration expiration = Expiration::never())
    {
        using namespace nau::async_detail;

        return whenAllInternal(CoreTaskLinkedList::fromContainer(std::begin(tasks), std::end(tasks)), std::move(expiration));
    }

    template <typename... T>
    inline Task<bool> whenAll(Expiration expiration, Task<T>&... tasks)
    {
        using namespace nau::async_detail;

        return whenAllInternal(CoreTaskLinkedList::fromTasks(tasks...), std::move(expiration));
    }

    /**
     */
    template <typename Container>
    Task<bool> whenAny(Container& tasks, Expiration expiration = Expiration::never())
    {
        using namespace nau::async_detail;

        return whenAnyInternal(CoreTaskLinkedList::fromContainer(std::begin(tasks), std::end(tasks)), std::move(expiration));
    }

    inline Task<bool> whenAny(std::vector<CoreTaskPtr>& tasks, Expiration expiration = Expiration::never())
    {
        using namespace nau::async_detail;

        return whenAnyInternal(CoreTaskLinkedList::fromContainer(std::begin(tasks), std::end(tasks)), std::move(expiration));
    }

    /**
     */
    template <typename... T>
    inline Task<bool> whenAny(Expiration expiration, Task<T>&... tasks)
    {
        using namespace nau::async_detail;

        return whenAnyInternal(CoreTaskLinkedList::fromTasks(tasks...), std::move(expiration));
    }

    template <typename F, typename... Args>
    requires(IsTask<std::invoke_result_t<F, Args...>>)
    std::invoke_result_t<F, Args...> run(F operation, Executor::Ptr executor, Args... args)
    {
        static_assert(IsTask<std::invoke_result_t<F, Args...>>);
        static_assert(std::is_invocable_v<F, Args...>, "Invalid functor. Arguments does not match expected parameters.");

        if (!executor)
        {
            executor = Executor::getDefault();
        }

        co_await executor;

        using TaskType = std::invoke_result_t<F, Args...>;

        // Executor::InvokeGuard invoke {*executor}; will be applied above (by call stack),
        // because currently we are inside of executor's invocation.
        TaskType task = operation(std::move(args)...);

        co_return (co_await std::move(task));
    }

    template <typename F, typename... Args>
    requires(!IsTask<std::invoke_result_t<F, Args...>>)
    Task<std::invoke_result_t<F, Args...>> run(F operation, Executor::Ptr scheduler, Args... args)
    {
        using Result = std::invoke_result_t<F, Args...>;

        static_assert(std::is_invocable_v<F, Args...>, "Invalid functor. Arguments does not match expected parameters.");

        if (!scheduler)
        {
            scheduler = Executor::getDefault();
        }

        co_await scheduler;

        // using TaskType = std::invoke_result_t<F, Args...>;

        // Executor::InvokeGuard invoke {*executor}; will be applied above (by call stack),
        // because currently we are inside of executor's invocation.
        if constexpr (!std::is_same_v<Result, void>)
        {
            co_return operation(std::move(args)...);
        }
        else
        {
            operation(std::move(args)...);
        }
    }

}  // namespace nau::async

namespace std
{
    template <typename T, typename... Args>
    struct coroutine_traits<nau::async::Task<T>, Args...>
    {
        struct promise_type : nau::async_detail::TaskPromise<T>
        {
            template <typename U,
                      enable_if_t<!::nau::IsErrorPtr<U> && !::nau::IsResult<U>, int> = 0>
            void return_value(U&& value)
            {
                static_assert(is_constructible_v<T, decltype(value)>, "Invalid return value. Check co_return statement.");
                this->taskSource.resolve(std::forward<U>(value));
            }

            template <typename E,
                      enable_if_t<::nau::IsError<E>, int> = 0>
            void return_value(::nau::Error::PtrType<E> error)
            {
                static_assert(is_assignable_v<nau::Error&, E&>, "Can not assign error type: private inheritance used ?");
                NAU_ASSERT(error);

                if (error)
                {
                    this->taskSource.reject(std::move(error));
                }
                else
                {
                    this->taskSource.reject(NauMakeError("Unspecified error"));
                }
            }

            template <typename U>
            void return_value(const ::nau::Result<U>& result)
            {
                static_assert(is_constructible_v<T, U>, "Invalid return Result<> value. Check co_return statement.");

                if (result.isError())
                {
                    this->taskSource.reject(result.getError());
                }
                else
                {
                    this->taskSource.resolve(*result);
                }
            }
        };
    };

    template <typename... Args>
    struct coroutine_traits<nau::async::Task<void>, Args...>
    {
        struct promise_type : nau::async_detail::TaskPromise<void>
        {
            void return_void()
            {
                this->taskSource.resolve();
            }
        };
    };

}  // namespace std
