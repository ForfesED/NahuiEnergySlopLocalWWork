// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "scene_test_base.h"

namespace nau::test
{
    void SceneTestBase::SetUp()
    {
        m_app = createApplication([this]
        {
            nau::loadModulesList(NAU_MODULES_LIST).ignore();

            initializeApp();

            return ResultSuccess;
        });
        m_app->startupOnCurrentThread();
    }

    void SceneTestBase::TearDown()
    {
        m_app->stop();
        while (m_app->step())
        {
            std::this_thread::yield();
        }
    }

    void SceneTestBase::initializeApp()
    {
    }

    Application& SceneTestBase::getApp()
    {
        return *m_app;
    }

    async::Task<> SceneTestBase::skipFrames(unsigned frameCount)
    {
        if (frameCount == 0)
        {
            return async::makeResolvedTask();
        }

        m_frameSkipAwaiters.emplace_back(frameCount);
        auto& awaiter = m_frameSkipAwaiters.back();
        return awaiter.signal.getTask();
    }

    testing::AssertionResult SceneTestBase::runTestApp(TestCallback callback)
    {
        using namespace std::chrono_literals;
        using namespace testing;
        using namespace nau::async;

        auto task = [](TestCallback&& callback) -> Task<AssertionResult>
        {
            scope_on_leave
            {
                getApplication().stop();
            };

            if (!callback)
            {
                co_return AssertionSuccess();
            }
            auto testTask = callback();
            NAU_FATAL(testTask);

            co_await testTask;
            co_return *testTask;
        }(std::move(callback));

        while (m_app->step())
        {
            std::this_thread::sleep_for(1ms);
            ++m_stepCounter;

            auto iter = std::remove_if(m_frameSkipAwaiters.begin(), m_frameSkipAwaiters.end(), [](SkipFrameAwaiter& awaiter)
            {
                if (--awaiter.skipFramesCount == 0)
                {
                    awaiter.signal.resolve();
                }

                return awaiter.skipFramesCount == 0;
            });

            m_frameSkipAwaiters.erase(iter, m_frameSkipAwaiters.end());
        }

        NAU_FATAL(task.isReady());

        return *task;
    }
}  // namespace nau::test
