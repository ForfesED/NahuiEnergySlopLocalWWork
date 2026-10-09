// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#include "gtest/gtest.h"
#include <chrono>

#include <nau/rtti/rtti_impl.h>
#include "daECS/core/componentTypes.h"
#include "daECS/core/coreEvents.h"
#include "daECS/core/dataComponent.h"
#include "daECS/core/entityManager.h"
#include "daECS/core/entitySystem.h"
#include "daECS/core/internal/performQuery.h"
#include "daECS/core/sharedComponent.h"
#include "daECS/core/updateStage.h"
#include "dag_perfTimer.h"
#include "nau/async/task.h"
#include "nau/async/thread_pool_executor.h"
#include "nau/async/work_queue.h"
#include "nau/utils/cancellation.h"
#include "nau/utils/span.h"
#include "nau/app/application_services.h"


using namespace std::literals::chrono_literals;

struct LoadGameResJob
{
    ecs::gameres_list_t resnm;
    eastl::vector<ecs::EntityId> entities;
    ::nau::async::Task<eastl::vector<ecs::EntityId>> doJob()
    {
        co_await nau::async::Executor::getDefault();
        // ECS_LOG("doJob");
        co_await 100ms;
        // ECS_LOG("jobDone");
        co_return entities;
    }
    void releaseJob()
    {
        if(g_entity_mgr && !entities.empty())  // in case if this callback is called after destruction of EntityManager
            g_entity_mgr->onEntitiesLoaded(entities, true);
    }
};

struct TestResourceManagerImpl : public ecs::IECSResourceManager
{
    NAU_RTTI_CLASS(TestResourceManagerImpl, IECSResourceManager)

    eastl::vector<ecs::EntityId> loadedEntities;
    bool load_gameres_list(const ecs::gameres_list_t& list) override
    {
        for(auto& [resName, resId] : list)
        {
            loadedEntities.push_back(ecs::EntityId(resId));  // In this test resId are used as storages for eid
        }
        return true;
    }
    size_t filter_out_loaded_gameres(ecs::gameres_list_t& list) override
    {
        // list.clear();
        return list.size();
    }
    eastl::vector<eastl::unique_ptr<LoadGameResJob>> jobs;
    eastl::vector<nau::async::Task<eastl::vector<ecs::EntityId>>> jobTasks;
    void async_load_gameres_list(eastl::vector<ecs::EntityId>&& eids, ecs::gameres_list_t&& nms) override
    {
        NAU_UNUSED(nms);
        for(auto& n : nms)
        {
            ECS_VERBOSE_LOG("place_gameres_request <{}>", n.first.c_str());
        }
        // g_entity_mgr->onEntitiesLoaded(eids);
        auto& job = jobs.emplace_back(eastl::make_unique<LoadGameResJob>());
        job->entities = eastl::move(eids);
        job->resnm = eastl::move(nms);

        // int jobMgr = get_common_loading_job_mgr();
        //  if (jobMgr >= 0
        // G_VERIFY(cpujobs::add_job(jobMgr, job));
        jobTasks.emplace_back(job->doJob());
    }

    eastl::vector<ecs::EntityId>&& wait_all_tasks()
    {
        auto awaiter = [this]() -> nau::async::Task<>
        {
            co_await nau::async::Executor::getDefault();
            co_await nau::async::whenAll(jobTasks, nau::Expiration::never());
        }();

        nau::async::wait(awaiter);
        for(int i = 0; i < jobTasks.size(); i++)
        {
            for(auto eid : jobTasks[i].result())
            {
                loadedEntities.push_back(eid);
            }
            jobs[i]->releaseJob();
        }
        jobs.clear();
        jobTasks.clear();
        return std::move(loadedEntities);
    }

    ~TestResourceManagerImpl()
    {
        wait_all_tasks();
    }
};

class TestDagorECS : public ::testing::Test
{
protected:
    static inline constexpr bool SuccessFlag = true;
    static inline constexpr bool FailureFlag = false;
    eastl::unique_ptr<nau::Application> app;
    TestResourceManagerImpl* resourceManager;

    TestDagorECS()
    {
        app = nau::createApplication();
        app->startupOnCurrentThread();
        g_entity_mgr.demandInit();
        resourceManager = new TestResourceManagerImpl();
        ecs::setECSResourceManager(eastl::unique_ptr<TestResourceManagerImpl>(resourceManager));
    }

    ~TestDagorECS()
    {
         g_entity_mgr.demandDestroy();
        ecs::setECSResourceManager(nullptr);
        nau::getApplication().stop();
        while(app->step())
        {
            std::this_thread::sleep_for(50ms);
        }
    }
};

struct TestData
{
    bool test_bool = false;
    nau::math::IVector2 test_ip2 = {};
    nau::math::IVector4 test_color = {};
    nau::math::Point2 test_p2 = {};
};

struct TestStructureComponent
{
    TestData validData;
    uint32_t resource_loaded = 0;
    uint32_t constructor_called = 0;
    uint32_t update_called = 0;
    uint32_t event_called = 0;
    uint32_t update_valid = 0;
    uint32_t event_valid = 0;
    uint32_t query_called = 0;
    uint32_t query_valid = 0;
};

ECS_DECLARE_TYPE(TestStructureComponent);

struct TestStructureComponentConstruct final : public TestStructureComponent
{
    EA_NON_COPYABLE(TestStructureComponentConstruct);
    TestStructureComponentConstruct() = default;

    static void requestResources(const char* name, const ecs::resource_request_cb_t& requestCb)
    {
        auto eid = requestCb.eid;
        requestCb("<test_string>", ecs::entity_id_t(eid));
    }

    TestStructureComponentConstruct(const ecs::EntityManager& mgr, ecs::EntityId eid, const ecs::ComponentsMap& map)
    {
        this->validData = map.find(ECS_HASH("test_structure"))->get<TestStructureComponent>().validData;
        this->constructor_called++;
        const char* billboardTextureRes = mgr.getOr(eid, ECS_HASH("test_string"), ecs::nullstr);
    }

    ~TestStructureComponentConstruct() = default;
};



ecs::template_t create_template(ecs::ComponentsMap&& map, ecs::Template::component_set&& tracked = ecs::Template::component_set(), const char* name = nullptr);

void validateEntities(const eastl::vector<ecs::EntityId>& entities, uint32_t event_called, uint32_t update_called, uint32_t query_called = 0);

void updateResourcesState(TestResourceManagerImpl* resourceManager);


struct MyTestEventAsyncEvent : public ecs::Event
{
    ECS_INSIDE_EVENT_DECL(MyTestEventAsyncEvent, ::ecs::EVCAST_BROADCAST | ::ecs::EVFLG_PROFILE)
    MyTestEventAsyncEvent() :
        ECS_EVENT_CONSTRUCTOR(MyTestEventAsyncEvent),
        data(0){};
    float data;
};

enum : uint32_t
{
    TESTS = 5000,
    ECS_RUNS = 10,
    CMP_RUNS = 40,
    CREATE_RUNS = 100,
    EID_QUERY_RUNS = 10,
    Q_CACHE_CNT = 5,
    Q_CNT = 40
};

void call_codegen_query();