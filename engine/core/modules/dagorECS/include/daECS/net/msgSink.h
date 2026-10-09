// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <daECS/net/message.h>

namespace net
{

typedef void (*msg_handler_t)(const IMessage *msg);

ecs::EntityId get_msg_sink(); // if INVALID_ENTITY_ID then not created yet
void set_msg_sink_created_cb(void (*on_msg_sink_created_cb)(ecs::EntityId eid));

}; // namespace net
