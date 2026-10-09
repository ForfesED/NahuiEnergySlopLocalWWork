// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved


#if __has_include ("util/dag_threadPool.h")

#include "osApiWrappers/dag_cpuJobs.h"
#include "nau/async/executor.h"
#include "nau/diag/assertion.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/threading/lock_guard.h"
#include "nau/threading/spin_lock.h"
#include "util/dag_threadPool.h"

namespace nau::async
{

    class DagThreadPoolExecutor final : public Executor,
                                        public cpujobs::IJob
    {
        NAU_CLASS_(nau::async::DagThreadPoolExecutor, Executor)

    public:
        DagThreadPoolExecutor(bool manageCpuJobs, int maxThreads) :
            m_manageCpuJobs{manageCpuJobs}
        {
            if(m_manageCpuJobs)
            {
                cpujobs::init();
            }
            threadpool::init(maxThreads, 2048);
        }

        ~DagThreadPoolExecutor()
        {
            NAU_ASSERT(m_taskCounter == 0);

            threadpool::shutdown();

            if(m_manageCpuJobs)
            {
                cpujobs::term(false);
            }
        }

        void scheduleInvocation(Invocation invocation) noexcept override
        {
            NAU_ASSERT(invocation);
            if(!invocation)
            {
                return;
            }

            {
                lock_(m_mutex);
                m_invocations.emplace_back(std::move(invocation));
                ++m_taskCounter;
            }

            threadpool::add(this);
        }

        void waitAnyActivity() noexcept override
        {
            using namespace std::chrono_literals;

            constexpr auto SleepTimeout = 2ms;

            while(m_taskCounter.load() > 0)
            {
                std::this_thread::sleep_for(SleepTimeout);
            }
        }

        void doJob() override
        {
            auto invocation = EXPR_Block->Invocation
            {
                lock_(m_mutex);
                if(m_invocations.empty())
                {
                    return {};
                }

                auto head = m_invocations.begin();

                auto i = std::move(*head);
                eraseInvocation(head);
                return i;
            };

            if(invocation)
            {
                NAU_ASSERT(m_taskCounter > 0);
                scope_on_leave
                {
                    --m_taskCounter;
                };

                const Executor::InvokeGuard guard{*this};
                Executor::invoke(*this, std::move(invocation));
            }
        }

    private:
        void eraseInvocation(std::list<Invocation>::iterator where)
        {
            m_invocations.erase(where);
  /*          if(auto& last = m_invocations.back(); &(*where) != &last)
            {
                *where = std::move(last);
            }

            const auto newSize = m_invocations.size() - 1;
            m_invocations.resize(newSize);*/
        }

        const bool m_manageCpuJobs;
        std::list<Invocation> m_invocations;
        //std::vector<Invocation> m_invocations;
        //threading::SpinLock m_mutex;
        std::mutex m_mutex;
        std::atomic_size_t m_taskCounter = 0;
    };

    Executor::Ptr createDagThreadPoolExecutor(bool initCpuJobs, std::optional<size_t> threadsCount)
    {
        const int maxThreads = threadsCount ? static_cast<int>(*threadsCount) : 8;
        return rtti::createInstance<DagThreadPoolExecutor, Executor>(initCpuJobs, maxThreads);
    }
}  // namespace nau::async

#endif