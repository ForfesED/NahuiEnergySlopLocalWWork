// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "core_task_impl.h"

#include <iostream>

#include "nau/memory/general_allocator.h"
#include "nau/utils/scope_guard.h"

namespace nau::async
{
    namespace
    {
        static_assert(alignof(CoreTaskImpl) <= alignof(std::max_align_t));

        constexpr size_t DefaultAlign = alignof(std::max_align_t);
        constexpr size_t CoreTaskSize = sizeof(std::aligned_storage_t<sizeof(CoreTaskImpl), DefaultAlign>);

        static_assert(DefaultAlign % sizeof(void*) == 0);
        static_assert(CoreTaskSize % sizeof(void*) == 0);
        static_assert(isPowerOf2(DefaultAlign));

        constexpr static uint32_t TaskFlag_Ready = 1 << 0;
        constexpr static uint32_t TaskFlag_HasContinuation = 1 << 2;
        constexpr static uint32_t TaskFlag_ContinuationScheduled = 1 << 3;
        constexpr static uint32_t TaskFlag_ResolveLocked = 1 << 5;
        constexpr static uint32_t TaskFlag_ReadyCallbackLocked = 1 << 6;

        inline bool hasFlags(const std::atomic<uint32_t>& bits, uint32_t mask)
        {
            return (bits.load() & mask) == mask;
        }

        inline void setFlagsOnce(std::atomic<uint32_t>& bits, uint32_t mask)
        {
            uint32_t value = bits.load();

            do
            {
                NAU_ASSERT((value & mask) == 0, "Flags ({}) already set", mask);
            } while (!bits.compare_exchange_weak(value, value | mask));
        }

        inline void unsetFlags(std::atomic<uint32_t>& bits, uint32_t mask)
        {
            uint32_t value = bits.load();
            while (!bits.compare_exchange_weak(value, value & (~mask)))
            {
            }
        }

        inline size_t getCoreTaskStorageSize(size_t dataSize, size_t dataAlignment)
        {
            // Notice about "+ dataAlignment".
            // If storage allocated by unaligned address (i.e. addr % dataAlignment != 0)
            // then the offset must be added, so there is need for some extra space to fit data in such cases
            return alignedSize(CoreTaskSize + dataSize, std::max(DefaultAlign, dataAlignment)) + dataAlignment;
        }

        /**
            spin-lock.
        */
        struct LockFlagGuard
        {
            std::atomic<uint32_t>& bits;
            const uint32_t flag;

            LockFlagGuard(std::atomic<uint32_t>& bitsIn, uint32_t flagIn) :
                bits(bitsIn),
                flag(flagIn)
            {
                uint32_t value = bits.load(std::memory_order_acquire);

                do
                {
                    while ((value & flag) != 0)
                    {
                        std::this_thread::yield();
                        value = bits.load(std::memory_order_relaxed);
                    }
                } while (!bits.compare_exchange_strong(value, value | flag));
            }

            ~LockFlagGuard()
            {
                unsetFlags(bits, flag);
            }
        };

        class TaskRejectorNoException final : public CoreTask::Rejector
        {
        public:
            void rejectWithError(Error::Ptr err) noexcept override
            {
                m_error = std::move(err);
            }

            Error::Ptr getError() const
            {
                return m_error;
            }

        private:
            Error::Ptr m_error;
        };

        using TaskRejector = TaskRejectorNoException;

        // TODO: need to make tracking for TaskCreationInfo optional (and possible exclude from production builds)
        struct TaskCreationInfo
        {
            // NAU-2338
            // std::stacktrace stackTrace;
        };

        std::mutex g_aliveTasksMutex;
        std::unordered_map<CoreTaskImpl*, TaskCreationInfo> g_aliveTasks;

    }  // namespace

    CoreTask::~CoreTask() = default;

    CoreTaskImpl::CoreTaskImpl(IMemAllocator::Ptr allocator, void* allocatedStorage, size_t dataSize, StateDestructorCallback destructor) :
        m_allocator(std::move(allocator)),
        m_allocatedStorage(allocatedStorage),
        m_dataSize(dataSize),
        m_destructor(destructor)
    {
        lock_(g_aliveTasksMutex);
        g_aliveTasks.emplace(this, TaskCreationInfo{});
    }

    CoreTaskImpl::~CoreTaskImpl()
    {
        if (m_destructor)
        {
            m_destructor(getData());
        }

        lock_(g_aliveTasksMutex);
        g_aliveTasks.erase(this);
    }

    void CoreTaskImpl::addRef()
    {
        NAU_VERIFY(m_refsCount.fetch_add(1) > 0);
    }

    void CoreTaskImpl::releaseRef()
    {
        if (m_refsCount.fetch_sub(1) > 1)
        {
            return;
        }

        auto allocator = std::move(m_allocator);
        NAU_ASSERT(allocator);

        void* const storage = m_allocatedStorage;
        std::destroy_at(this);
        allocator->deallocate(storage);
    }

    bool CoreTaskImpl::isReady() const
    {
        return hasFlags(m_flags, TaskFlag_Ready);
    }

    bool CoreTaskImpl::tryResolveInternal(ResolverCallback callback, void* ptr)
    {
        {
            const LockFlagGuard lockResolve{m_flags, TaskFlag_ResolveLocked};

            if (hasFlags(m_flags, TaskFlag_Ready))
            {
                return false;
            }

            TaskRejector rejector;
            if (callback)
            {
                callback(rejector, ptr);
                m_error = rejector.getError();
            }

            setFlagsOnce(m_flags, TaskFlag_Ready);
        }

        invokeReadyCallback();
        tryScheduleContinuation();

        return true;
    }

    Error::Ptr CoreTaskImpl::getError() const
    {
        NAU_ASSERT(isReady(), "Can request state/error only after task is ready");

        const LockFlagGuard lockResolve{m_flags, TaskFlag_ResolveLocked};
        return m_error;
    }

    const void* CoreTaskImpl::getData() const
    {
        return reinterpret_cast<const std::byte*>(this) + CoreTaskSize;
    }

    void* CoreTaskImpl::getData()
    {
        return reinterpret_cast<std::byte*>(this) + CoreTaskSize;
    }

    size_t CoreTaskImpl::getDataSize() const
    {
        return m_dataSize;
    }

    void CoreTaskImpl::setContinuation(TaskContinuation continuation)
    {
        NAU_ASSERT(!m_continuation);
        NAU_ASSERT(continuation);

        m_continuation = std::move(continuation);
        if (!m_isContinueOnCapturedExecutor.load(std::memory_order_acquire))
        {  // Anyway m_isContinueOnCapturedExecutor will be checked inside tryScheduleContinuation
           // but there we can release executor as soon as possible.
            m_continuation.executor = nullptr;
        }

        setFlagsOnce(m_flags, TaskFlag_HasContinuation);
        tryScheduleContinuation();
    }

    bool CoreTaskImpl::hasContinuation() const
    {
        return hasFlags(m_flags, TaskFlag_HasContinuation);
    }

    bool CoreTaskImpl::hasCapturedExecutor() const
    {
        return m_continuation && m_continuation.executor;
    }

    void CoreTaskImpl::setContinueOnCapturedExecutor(bool continueOnCapturedExecutor)
    {
        NAU_ASSERT(!hasFlags(m_flags, TaskFlag_HasContinuation | TaskFlag_ContinuationScheduled), "Can not change ContinueOnCapturedExecutor after continuation is set");

        m_isContinueOnCapturedExecutor.store(continueOnCapturedExecutor, std::memory_order_release);
    }

    bool CoreTaskImpl::isContinueOnCapturedExecutor() const
    {
        return m_isContinueOnCapturedExecutor.load(std::memory_order_acquire);
    }

    void CoreTaskImpl::setReadyCallback(ReadyCallback callback, void* data1, void* data2)
    {
        // callback must be called outside of the LockFlagGuard's scope: potential recursion must not block each other
        auto readyCallback = EXPR_Block->Executor::Invocation
        {
            const LockFlagGuard lockReady{m_flags, TaskFlag_ReadyCallbackLocked};

            if (isReady())
            {
                return {callback, data1, data2};
            }

            m_readyCallback = {callback, data1, data2};
            return {};
        };

        if (readyCallback)
        {
            readyCallback();
        }
    }

    void CoreTaskImpl::invokeReadyCallback()
    {
        auto callback = EXPR_Block->Executor::Invocation
        {
            const LockFlagGuard lockReady{m_flags, TaskFlag_ReadyCallbackLocked};
            Executor::Invocation callback = std::move(m_readyCallback);
            NAU_ASSERT(!m_readyCallback);
            return callback;
        };

        if (callback)
        {
            callback();
        }
    }

    void CoreTaskImpl::tryScheduleContinuation()
    {
        if (!hasFlags(m_flags, TaskFlag_Ready | TaskFlag_HasContinuation))
        {
            return;
        }

        {
            uint32_t flags = m_flags.load(std::memory_order::acquire);

            do
            {
                if ((flags & TaskFlag_ContinuationScheduled) == TaskFlag_ContinuationScheduled)
                {
                    return;
                }
            } while (!m_flags.compare_exchange_weak(flags, flags | TaskFlag_ContinuationScheduled));
        }

        TaskContinuation continuation = std::move(m_continuation);
        NAU_ASSERT(continuation);
        NAU_ASSERT(!m_continuation);

        Executor::Ptr executor = continuation.executor ? std::move(continuation.executor) : Executor::getCurrent();
        if (executor && m_isContinueOnCapturedExecutor.load(std::memory_order_acquire))
        {
            // !!! BE AWARE !!!
            // In any moment right after continuation is scheduled 'this' Task Core instance can be destructed.
            // Continuation will resume awaiter that holds reference to this task instance and this reference can be released.
            executor->execute(std::move(continuation.invocation));
            // So 'this' task can not be used from this point any more.
        }
        else
        {
            continuation.invocation();
        }
    }

    CoreTaskImpl* CoreTaskImpl::getNext() const
    {
        return m_next;
    }

    void CoreTaskImpl::setNext(CoreTaskImpl* nextTask)
    {
        NAU_ASSERT(!nextTask || !m_next);
        m_next = nextTask;
    }

    CoreTaskPtr::~CoreTaskPtr()
    {
        reset();
    }

    CoreTaskPtr::CoreTaskPtr(std::nullptr_t)
    {
    }

    CoreTaskPtr::CoreTaskPtr(const CoreTaskPtr& other) :
        m_coreTask(other.m_coreTask)
    {
        if (m_coreTask)
        {
            m_coreTask->addRef();
        }
    }

    CoreTaskPtr::CoreTaskPtr(CoreTaskPtr&& other) noexcept :
        m_coreTask(other.m_coreTask)
    {
        other.m_coreTask = nullptr;
    }

    CoreTaskPtr& CoreTaskPtr::operator=(const CoreTaskPtr& other)
    {
        reset();
        if (m_coreTask = other.m_coreTask; m_coreTask)
        {
            m_coreTask->addRef();
        }

        return *this;
    }

    CoreTaskPtr& CoreTaskPtr::operator=(CoreTaskPtr&& other) noexcept
    {
        reset();
        std::swap(m_coreTask, other.m_coreTask);

        return *this;
    }

    CoreTaskPtr& CoreTaskPtr::operator=(std::nullptr_t) noexcept
    {
        reset();
        return *this;
    }

    CoreTask& CoreTaskPtr::getCoreTask() noexcept
    {
        NAU_ASSERT(static_cast<bool>(*this), "Task is stateless");
        return *m_coreTask;
    }

    const CoreTask& CoreTaskPtr::getCoreTask() const noexcept
    {
        NAU_ASSERT(static_cast<bool>(*this), "Task is stateless");
        return *m_coreTask;
    }

    void CoreTaskPtr::reset()
    {
        if (m_coreTask)
        {
            m_coreTask->releaseRef();
        }

        m_coreTask = nullptr;
    }

    CoreTaskPtr CoreTask::create(IMemAllocator::Ptr customAllocator, size_t dataSize, size_t dataAlignment, StateDestructorCallback destructor)
    {
        IMemAllocator::Ptr allocator = customAllocator ? std::move(customAllocator) : getDefaultAllocator();
        NAU_ASSERT(allocator);
        NAU_ASSERT(isPowerOf2(dataAlignment));
        NAU_ASSERT(dataAlignment < DefaultAlign || (dataAlignment % DefaultAlign) == 0);

        // storageSize must be sufficient to store any properly aligned object.
        const size_t storageSize = getCoreTaskStorageSize(dataSize, dataAlignment);

        // the allocated storage may be different from where the CoreTaskImpl will actually be created.
        void* const allocatedStorage = allocator->allocate(storageSize);
        NAU_ASSERT(allocatedStorage);

        // By default the placement storage is the same as the allocated one, but it can be changed if it requires by type alignment
        void* placementStorage = allocatedStorage;
        {
            const auto clientData = reinterpret_cast<uintptr_t>(reinterpret_cast<std::byte*>(placementStorage) + CoreTaskSize);
            if (const uintptr_t alignmentOffset = clientData % dataAlignment; alignmentOffset > 0)
            {
                const size_t offsetGap = dataAlignment - alignmentOffset;
                placementStorage = reinterpret_cast<std::byte*>(allocatedStorage) + offsetGap;
                NAU_FATAL(storageSize >= CoreTaskSize + dataSize + offsetGap);
            }
        }

        NAU_FATAL(reinterpret_cast<uintptr_t>(placementStorage) % alignof(CoreTaskImpl) == 0);
        NAU_FATAL(reinterpret_cast<uintptr_t>(reinterpret_cast<std::byte*>(placementStorage) + CoreTaskSize) % dataAlignment == 0);

        auto const coreTask = new(placementStorage) CoreTaskImpl{std::move(allocator), allocatedStorage, dataSize, destructor};
        return CoreTaskOwnership{coreTask};
    }

    NAU_KERNEL_EXPORT void dumpAliveTasks()
    {
        lock_(g_aliveTasksMutex);

        const size_t aliveTasksWithCapturedExecutorCount = std::count_if(g_aliveTasks.begin(), g_aliveTasks.end(), [](const auto& pair)
        {
            return !pair.first->hasCapturedExecutor();
        });

        if (aliveTasksWithCapturedExecutorCount == 0)
        {
            std::cout << "There is no alive tasks with captured executor\n";
            return;
        }

        std::cout << std::format("Has ({}) alive tasks with captured executor\n", aliveTasksWithCapturedExecutorCount);

        for (const auto& [coreTask, taskCreationInfo] : g_aliveTasks)
        {
            if (!coreTask->hasCapturedExecutor())
            {
                continue;
            }

            // NAU-2338
            // dump task's creation stack trace.
        }
    }

    NAU_KERNEL_EXPORT bool hasAliveTasksWithCapturedExecutor()
    {
        lock_(g_aliveTasksMutex);

        return std::any_of(g_aliveTasks.begin(), g_aliveTasks.end(), [](const auto& pair)
        {
            return pair.first->hasCapturedExecutor();
        });
    }

}  // namespace nau::async
