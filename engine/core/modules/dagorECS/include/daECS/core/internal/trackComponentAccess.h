// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "daECS/core/entityId.h"

namespace ecs
{
    struct BaseQueryDesc;
}

namespace ecsdebug
{
    enum TrackOp
    {
        TRACK_READ,
        TRACK_WRITE
    };

#if NAU_DEBUG
    void NAU_DAGORECS_EXPORT start_track_ecs_component(ecs::component_t comp);
    void NAU_DAGORECS_EXPORT stop_dump_track_ecs_components();

    void NAU_DAGORECS_EXPORT track_ecs_component(const ecs::BaseQueryDesc& desc, const char* details, ecs::EntityId eid = INVALID_ENTITY_ID, bool need_stack = false);

    void NAU_DAGORECS_EXPORT track_ecs_component(ecs::component_t comp, TrackOp op, const char* details, ecs::EntityId eid = INVALID_ENTITY_ID, bool need_stack = false);
    void NAU_DAGORECS_EXPORT track_ecs_component_by_index(ecs::component_index_t cidx, TrackOp op, const char* details, ecs::EntityId eid = INVALID_ENTITY_ID, bool need_stack = false);
    inline void NAU_DAGORECS_EXPORT track_ecs_component_by_index_with_stack(ecs::component_index_t cidx, TrackOp op, const char* details, ecs::EntityId eid = INVALID_ENTITY_ID)
    {
        ecsdebug::track_ecs_component_by_index(cidx, op, details, eid, true);
    }
#else
    inline NAU_DAGORECS_EXPORT void start_track_ecs_component(ecs::component_t)
    {
    }
    inline NAU_DAGORECS_EXPORT void stop_dump_track_ecs_components()
    {
    }
    inline NAU_DAGORECS_EXPORT void track_ecs_component(const ecs::BaseQueryDesc&, const char*, ecs::EntityId = INVALID_ENTITY_ID, bool = false)
    {
    }
    inline NAU_DAGORECS_EXPORT void track_ecs_component(ecs::component_t, TrackOp, const char*, ecs::EntityId = INVALID_ENTITY_ID, bool = false)
    {
    }
    inline NAU_DAGORECS_EXPORT void track_ecs_component_by_index(ecs::component_index_t, TrackOp, const char*, ecs::EntityId = INVALID_ENTITY_ID, bool = false)
    {
    }
    inline NAU_DAGORECS_EXPORT void track_ecs_component_by_index_with_stack(ecs::component_index_t, TrackOp, const char*, ecs::EntityId = INVALID_ENTITY_ID)
    {
    }
#endif
}  // namespace ecsdebug
