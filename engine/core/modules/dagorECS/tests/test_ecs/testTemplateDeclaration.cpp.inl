// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#include "test_ecs_common.h"
#include "test_template_declaration.h"

using namespace nau::math;

ECS_REGISTER_TYPE(TestTemplateDeclarationStruct, nullptr);

ECS_AUTO_REGISTER_COMPONENT(ecs::Tag, "testTemplate1", nullptr, 0);
ECS_AUTO_REGISTER_COMPONENT(ecs::Tag, "testTemplate2", nullptr, 0);
ECS_AUTO_REGISTER_COMPONENT(ecs::Tag, "testTemplate3", nullptr, 0);
ECS_AUTO_REGISTER_COMPONENT(TestTemplateDeclarationStruct, "test_struct", nullptr, 0);
ECS_AUTO_REGISTER_COMPONENT(int, "test_int", nullptr, 0);

template <class Callable>
inline void call_t1_ecs_query(Callable c);

template <class Callable>
inline void call_t2_ecs_query(Callable c);

template <class Callable>
inline void call_t3_ecs_query(Callable c);

uint32_t call_t1()
{
    uint32_t counter = 0;
    call_t1_ecs_query(
        [&](
            ECS_REQUIRE(ecs::Tag testTemplate1) ECS_REQUIRE_NOT(ecs::Tag testTemplate2)
                ecs::EntityId eid)
    {
        counter++;
    });
    return counter;
}

uint32_t call_t2()
{
    uint32_t counter = 0;
    call_t2_ecs_query(
        [&](ECS_REQUIRE(ecs::Tag testTemplate1, ecs::Tag testTemplate2) ECS_REQUIRE_NOT(ecs::Tag testTemplate3)
                ecs::EntityId eid,
            int& test_int)
    {
        counter++;
    });
    return counter;
}

uint32_t call_t3()
{
    uint32_t counter = 0;
    call_t3_ecs_query(

        [&](ECS_REQUIRE(ecs::Tag testTemplate1, ecs::Tag testTemplate2, ecs::Tag testTemplate3)
                ecs::EntityId eid,
            int& test_int, TestTemplateDeclarationStruct& test_struct)
    {
        counter++;
    });
    return counter;
}