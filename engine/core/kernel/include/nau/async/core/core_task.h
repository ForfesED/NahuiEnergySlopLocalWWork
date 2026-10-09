// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include <type_traits>

#include "nau/async/executor.h"
#include "nau/diag/assertion.h"
#include "nau/diag/error.h"
#include "nau/kernel/kernel_config.h"
#include "nau/memory/mem_allocator.h"
#include "nau/rtti/type_info.h"

namespace nau::async_detail
{

    template <typename T>
    struct RvTaskAwaiter;

    template <typename T>
    struct LvTaskAwaiter;

}  // namespace nau::async_detail

namespace nau::async
{

    class CoreTaskPtr;
    struct CoreTask;
    struct TaskContinuation;

    template <typename T, typename... Args>
    CoreTaskPtr createCoreTask(nau::IMemAllocator::Ptr, Args&&... argss);

    NAU_KERNEL_EXPORT CoreTask* getCoreTask(CoreTaskPtr&);

    /**
     */
    struct CoreTaskOwnership
    {
        CoreTask* const coreTaskPtr;

        CoreTaskOwnership(CoreTask* ptr) :
            coreTaskPtr{ptr}
        {
        }

        CoreTaskOwnership(const CoreTaskOwnership&) = delete;
    };

    /**
        @brief
        Actual implementation for task.
    */
    struct NAU_ABSTRACT_TYPE CoreTask
    {
        using ReadyCallback = Executor::Callback;
        using Ptr = CoreTaskPtr;

        struct NAU_ABSTRACT_TYPE Rejector
        {
            virtual ~Rejector() = default;

            virtual void rejectWithError(Error::Ptr) noexcept = 0;
        };

        virtual ~CoreTask();

        virtual void addRef() = 0;
        virtual void releaseRef() = 0;
        virtual bool isReady() const = 0;

        void rethrow() const
        {
            if(auto err = this->getError(); err)
            {
            }
        }
        virtual Error::Ptr getError() const = 0;
        virtual const void* getData() const = 0;
        virtual void* getData() = 0;
        virtual size_t getDataSize() const = 0;
        virtual void setContinuation(TaskContinuation) = 0;
        /**
         */
        virtual void setContinueOnCapturedExecutor(bool continueOnCapturedExecutor) = 0;

        /**
         */
        virtual bool isContinueOnCapturedExecutor() const = 0;
        virtual bool hasContinuation() const = 0;
        virtual bool hasCapturedExecutor() const = 0;

        virtual void setReadyCallback(ReadyCallback callback, void*, void* = nullptr) = 0;

        bool tryRejectWithError(Error::Ptr error)
        {
            return tryResolveInternal([](Rejector& rejector, void* ptr) noexcept
                                      {
                                          Error::Ptr* const err = reinterpret_cast<Error::Ptr*>(ptr);
                                          rejector.rejectWithError(*err);
                                      },
                                      &error);
        }

        template <typename F>
        bool tryResolve(F f)
        {
            static_assert(std::is_invocable_r_v<void, F, Rejector&>, "Invalid functor");

            return tryResolveInternal([](Rejector& rejector, void* ptr) noexcept
                                      {
                                          F& callback = *reinterpret_cast<F*>(ptr);
                                          callback(rejector);
                                      },
                                      &f);
        }

        bool tryResolve()
        {
            return tryResolveInternal(nullptr, nullptr);
        }

    protected:
        using ResolverCallback = void (*)(Rejector&, void*) noexcept;
        using StateDestructorCallback = void (*)(void*) noexcept;

        virtual bool tryResolveInternal(ResolverCallback, void*) = 0;

    private:
        NAU_KERNEL_EXPORT
        static CoreTaskPtr create(IMemAllocator::Ptr, size_t size, size_t alignment, StateDestructorCallback);

        template <typename T, typename... Args>
        friend CoreTaskPtr createCoreTask(IMemAllocator::Ptr, Args&&... args);
    };

    /**
        @brief
        CoreTask smart pointer
    */
    class NAU_KERNEL_EXPORT CoreTaskPtr
    {
    public:
        virtual ~CoreTaskPtr();

        CoreTaskPtr() = default;

        CoreTaskPtr(std::nullptr_t);

        CoreTaskPtr(const CoreTaskPtr& other);

        CoreTaskPtr(CoreTaskPtr&& other) noexcept;

        CoreTaskPtr(const CoreTaskOwnership& ownership_) :
            m_coreTask(ownership_.coreTaskPtr)
        {
        }

        CoreTaskPtr& operator=(const CoreTaskPtr& other);

        CoreTaskPtr& operator=(CoreTaskPtr&& other) noexcept;

        CoreTaskPtr& operator=(std::nullptr_t) noexcept;

        CoreTask* giveUp()
        {
            return std::exchange(m_coreTask, nullptr);
        }

        explicit operator bool() const noexcept
        {
            return m_coreTask != nullptr;
        }

    protected:
        CoreTask& getCoreTask() noexcept;

        const CoreTask& getCoreTask() const noexcept;

        void reset();

    private:
        CoreTask* m_coreTask = nullptr;

        friend struct CoreTask;

        template <typename T, typename... Args>
        friend CoreTaskPtr createCoreTask(IMemAllocator::Ptr, Args&&... args);

        NAU_KERNEL_EXPORT
        friend CoreTask* getCoreTask(CoreTaskPtr&);

        template <typename T>
        friend struct async_detail::RvTaskAwaiter;

        template <typename T>
        friend struct async_detail::LvTaskAwaiter;
    };

    /**
     */
    struct TaskContinuation
    {
        Executor::Invocation invocation;
        Executor::Ptr executor;

        TaskContinuation() = default;

        /**
            invocation
            executor
        */
        TaskContinuation(Executor::Invocation invocationIn, Executor::Ptr executorIn) :
            invocation(std::move(invocationIn)),
            executor(std::move(executorIn))
        {
        }

        explicit inline operator bool() const
        {
            return static_cast<bool>(invocation);
        }
    };

    template <typename T, typename... Args>
    CoreTaskPtr createCoreTask(IMemAllocator::Ptr allocator, Args&&... args)
    {
        CoreTaskPtr coreTaskPtr = CoreTask::create(std::move(allocator), sizeof(T), alignof(T), [](void* ptr) noexcept
                                                   {
                                                       T* const state = reinterpret_cast<T*>(ptr);
                                                       std::destroy_at(state);
                                                   });

        new (coreTaskPtr.getCoreTask().getData()) T(std::forward<Args>(args)...);

        return coreTaskPtr;
    }

    inline CoreTask* getCoreTask(CoreTaskPtr& coreTaskPtr)
    {
        return coreTaskPtr ? &coreTaskPtr.getCoreTask() : nullptr;
    }

}  // namespace nau::async
