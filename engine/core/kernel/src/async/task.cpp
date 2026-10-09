// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/async/task.h"

#include "core_task_impl.h"
#include "nau/diag/logging.h"
#include "nau/threading/event.h"

namespace nau::async_detail
{
#pragma region CoreTaskLinkedList

    bool operator==(const CoreTaskLinkedList::iterator& iter1, const CoreTaskLinkedList::iterator& iter2)
    {
        return iter1.m_taskPtr == iter2.m_taskPtr;
    }

    bool operator!=(const CoreTaskLinkedList::iterator& iter1, const CoreTaskLinkedList::iterator& iter2)
    {
        return iter1.m_taskPtr != iter2.m_taskPtr;
    }

    CoreTaskLinkedList::iterator& CoreTaskLinkedList::iterator::operator++()
    {
        NAU_ASSERT(m_taskPtr, "Task iterator is not dereferenceable");
        m_taskPtr = static_cast<async::CoreTaskImpl*>(m_taskPtr)->getNext();

        return *this;
    }

    CoreTaskLinkedList::iterator CoreTaskLinkedList::iterator::operator++(int)
    {
        NAU_ASSERT(m_taskPtr, "Task iterator is not dereferenceable");

        iterator temp{*this};
        this->operator++();
        return temp;
    }

    async::CoreTask* CoreTaskLinkedList::iterator::operator*() const
    {
        NAU_ASSERT(m_taskPtr, "Task iterator is not dereferenceable");
        return m_taskPtr;
    }

    CoreTaskLinkedList::iterator::iterator(async::CoreTask* taskPtr) :
        m_taskPtr(taskPtr)
    {
    }

    CoreTaskLinkedList::CoreTaskLinkedList(TaskContainerIterator taskIterator, void* iteratorState)
    {
        using namespace nau::async;
        CoreTaskImpl* prev = nullptr;

        do
        {
            CoreTaskPtr current = taskIterator(iteratorState);
            if(!current)
            {
                break;
            }

            CoreTaskImpl* const task = static_cast<CoreTaskImpl*>(getCoreTask(current));
            task->addRef();

            if(!m_head)
            {
                m_head = task;
            }

            if(prev)
            {
                NAU_ASSERT(!prev->getNext());
                prev->setNext(task);
            }

            prev = task;
            ++m_size;
        } while(true);
    }

    CoreTaskLinkedList::~CoreTaskLinkedList()
    {
        NAU_ASSERT(!m_head, "CoreTaskLinkedList::reset() must be used explicitly");
    }

    CoreTaskLinkedList::CoreTaskLinkedList(CoreTaskLinkedList&& other) :
        m_head(other.m_head),
        m_size(other.m_size)
    {
        other.m_head = nullptr;
        other.m_size = 0;
    }

    CoreTaskLinkedList& CoreTaskLinkedList::operator=(CoreTaskLinkedList&& other)
    {
        NAU_ASSERT(m_head == nullptr);
        NAU_ASSERT(m_size == 0);

        std::swap(m_head, other.m_head);
        std::swap(m_size, other.m_size);

        return *this;
    }

    CoreTaskLinkedList::iterator CoreTaskLinkedList::begin()
    {
        return {m_head};
    }

    CoreTaskLinkedList::iterator CoreTaskLinkedList::end()
    {
        return iterator{};
    }

    size_t CoreTaskLinkedList::size() const
    {
#ifndef NDEBUG
        using namespace nau::async;

        const CoreTaskImpl* next = static_cast<const CoreTaskImpl*>(m_head);
        size_t counter = 0;
        while(next)
        {
            ++counter;
            next = next->getNext();
        }

        NAU_ASSERT(m_size == counter);
#endif

        return m_size;
    }

    bool CoreTaskLinkedList::empty() const
    {
        return m_head == nullptr;
    }

    void CoreTaskLinkedList::reset()
    {
        using namespace nau::async;

        CoreTaskImpl* next = static_cast<CoreTaskImpl*>(m_head);
        m_head = nullptr;

        while(next)
        {
            CoreTaskImpl* const current = next;
            next = next->getNext();

            current->setNext(nullptr);
            current->setReadyCallback(nullptr, nullptr);
            current->releaseRef();
        }
    }

    void CoreTaskLinkedList::append(async::CoreTaskPtr task)
    {
        using namespace nau::async;

        CoreTaskImpl* const taskImpl = static_cast<CoreTaskImpl*>(getCoreTask(task));

        taskImpl->addRef();

        if(m_head)
        {
            taskImpl->setNext(static_cast<CoreTaskImpl*>(m_head));
        }

        m_head = taskImpl;

        ++m_size;
    }

#pragma endregion

    /*
        IMPORTANT !!!
        taskList.reset() must be explicitly called before whenAllInternal/whenAnyInternal will return result
        (i.e. it can not be called 'automatically' from destructor or scope_leave guard).
        CoreTaskLinkedList will reset its tasks list state.
        But if it will be at the same time as completion of the current coroutine
        the tasks state right after co_await Async::whenAll/Async::whenAny will be invalid (because callbacks and next still are set ).
    */

    struct AwaiterState
    {
        enum class CompletionState
        {
            None,
            WithTrue,
            WithFalse
        };

        CoreTaskLinkedList* taskList;
        std::atomic<size_t> counter;
        async::TaskSource<> taskSource;
        Expiration expiration;
        ExpirationSubscription expirationSubscription;
        std::atomic<CompletionState> completionState = CompletionState::None;

        static AwaiterState* create(CoreTaskLinkedList& list, Expiration&& expiration, size_t initialCounter = 0)
        {
            void* const mem = getDefaultAllocator()->allocate(sizeof(AwaiterState));
            NAU_ASSERT(reinterpret_cast<uintptr_t>(mem) % alignof(AwaiterState) == 0);

            return new (mem) AwaiterState(list, std::move(expiration), initialCounter);
        }

        AwaiterState(CoreTaskLinkedList& list, Expiration&& inExpiration, size_t initialCounter = 0) :
            taskList(&list),
            expiration(std::move(inExpiration)),
            counter(initialCounter)
        {
            if(!expiration.isEternal())
            {
                expirationSubscription = expiration.subscribe([](void* ptr)
                {
                    auto& awaiterState = *reinterpret_cast<AwaiterState*>(ptr);

                    // BE AWARE !!!:
                    // this 'resolve(false)' call will resume awaited coroutine within whenAllInternal/whenAnyInternal
                    // and because the resumed coroutine does not switch executor (see comments below about async::wait), it will completed and destroyed right inside resolve.
                    // That is why AwaiterState must be created on heap and deleted in delayed fashion.
                    awaiterState.resolve(false);
                }, this);
            }
        }

        /**
            Called when:
                - operation is cancelled (explicitly or timeout),
                - operation's condition is satisfied
                only first result will be taken into account (all subsequent calls will be ignored)
        */
        void resolve(bool result)
        {
            const CompletionState newState = result ? CompletionState::WithTrue : CompletionState::WithFalse;
            CompletionState expectedNoState = CompletionState::None;

            if(completionState.compare_exchange_strong(expectedNoState, newState))
            {
                taskList->reset();
                taskList = nullptr;
                taskSource.resolve();
            }
        }

        bool isCompleted() const
        {
            const auto state = completionState.load(std::memory_order_acquire);
            NAU_ASSERT(state != CompletionState::None);
            return state == CompletionState::WithTrue;
        }

        void deleteDelayed()
        {
            NAU_FATAL(!taskList);

            auto executor = async::Executor::getDefault();

            if (!executor)
            {   // This code path should be executed only for tests, when there is no default executor.
                // In any case this is unsafe to destroy state right here when timeout is supposed to be used.
                // When timeout is specified for whenAll/whenAny operation the default executor must exists.

                [[maybe_unused]] const bool isSafeToDestroyInplace = !expirationSubscription;
                NAU_FATAL(isSafeToDestroyInplace);

                AwaiterState* self = this;
                std::destroy_at(self);
                getDefaultAllocator()->deallocate(self);

                return;
            }

            NAU_FATAL(executor);
            async::run([](AwaiterState* self)
            {
                std::destroy_at(self);
                getDefaultAllocator()->deallocate(self);
            }, std::move(executor), this)
                .detach();
        }
    };

    bool waitInternal(async::CoreTaskPtr taskPtr, std::optional<std::chrono::milliseconds> timeout)
    {
        using namespace nau::async;

        CoreTask* const task = getCoreTask(taskPtr);
        if(task->isReady())
        {
            return true;
        }

        scope_on_leave
        {
            task->setReadyCallback(nullptr, nullptr);
        };

        threading::Event signal;
        task->setReadyCallback([](void* ptr, void*) noexcept
        {
            auto& signal = *reinterpret_cast<threading::Event*>(ptr);
            signal.set();
        }, &signal);

        return signal.wait(timeout);
    }

    async::Task<bool> whenAllInternal(CoreTaskLinkedList tasks, Expiration expiration)
    {
        using namespace nau::async;

        const size_t size = tasks.size();
        if(std::all_of(tasks.begin(), tasks.end(), [](const CoreTask* task)
        {
            return task->isReady();
        }))
        {
            // see notes above (about CoreTaskLinkedList::reset())
            tasks.reset();
            co_return true;
        }

        if(expiration.isExpired())
        {
            tasks.reset();
            co_return false;
        }

        AwaiterState* const awaiterState = AwaiterState::create(tasks, std::move(expiration), size);
        scope_on_leave
        {
            awaiterState->deleteDelayed();
        };

        for(CoreTask* const task : tasks)
        {
            task->setReadyCallback([](void* ptr, void*) noexcept
            {
                auto& awaiterState = *reinterpret_cast<AwaiterState*>(ptr);

                if(awaiterState.counter.fetch_sub(1) == 1)
                {
                    awaiterState.resolve(true);
                }
            }, awaiterState);
        }

        Task<> awaiterTask = awaiterState->taskSource.getTask();

        // there is no need to switch back to original executor (where we currently)
        // when awaiterTask is ready there is no difference on which executor it will be completed.
        //
        // Also this is especially critical when task (from whenAny/whenAll) supposed to be used
        // with async::wait().  In in this case deadlock  will be guaranteed
        // because after co_await std::move(awaiterTask) completed it will try to switch back to the captured executor
        //  which blocked with async::wait()
        awaiterTask.setContinueOnCapturedExecutor(false);

        co_await std::move(awaiterTask);

        co_return awaiterState->isCompleted();
    }

    async::Task<bool> whenAnyInternal(CoreTaskLinkedList tasks, Expiration expiration)
    {
        using namespace nau::async;

        if(tasks.empty() || std::any_of(tasks.begin(), tasks.end(), [](const CoreTask* task)
        {
            return task->isReady();
        }))
        {
            // see notes above (about CoreTaskLinkedList::reset())
            tasks.reset();
            co_return true;
        }

        if(expiration.isExpired())
        {
            tasks.reset();
            co_return false;
        }

        AwaiterState* const awaiterState = AwaiterState::create(tasks, std::move(expiration));
        scope_on_leave
        {
            awaiterState->deleteDelayed();
        };

        for(CoreTask* const task : tasks)
        {
            task->setReadyCallback([](void* ptr, void*) noexcept
            {
                auto& awaiterState = *reinterpret_cast<AwaiterState*>(ptr);
                awaiterState.resolve(true);
            }, awaiterState);
        }

        Task<> awaiterTask = awaiterState->taskSource.getTask();

        // see comments within whenAnyInternal
        awaiterTask.setContinueOnCapturedExecutor(false);

        co_await std::move(awaiterTask);

        co_return awaiterState->isCompleted();
    }

    ExpirationAwaiter::ExpirationAwaiter(Expiration&& exp) :
        expiration(std::move(exp))
    {
        NAU_ASSERT(!expiration.isEternal(), "Can not await never expired expiration");
    }

    bool ExpirationAwaiter::await_ready() const noexcept
    {
        return expiration.isExpired();
    }

    void ExpirationAwaiter::await_suspend(std::coroutine_handle<> cont)
    {
        executor = async::Executor::getCurrent();
        continuation = std::move(cont);

        this->subscription = expiration.subscribe([](void* ptr)
        {
            ExpirationAwaiter& self = *reinterpret_cast<ExpirationAwaiter*>(ptr);

            auto continuation = std::move(self.continuation);
            self.continuation = nullptr;

            NAU_ASSERT(continuation);

            if(auto executor = std::move(self.executor))
            {
                executor->execute(std::move(continuation));
            }
            else
            {
                continuation();
            }
        }, this);
    }

    void ExpirationAwaiter::await_resume() const noexcept
    {
    }
}  // namespace nau::async_detail
