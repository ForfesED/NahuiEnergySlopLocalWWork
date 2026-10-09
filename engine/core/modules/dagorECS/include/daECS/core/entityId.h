// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once
#include "nau/string/string.h"
#include <stdint.h>

namespace ecs
{
typedef uint32_t entity_id_t;

const unsigned ENTITY_INDEX_BITS = 22;
const unsigned ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;

const unsigned ENTITY_GENERATION_BITS = 8;
const unsigned ENTITY_GENERATION_MASK = (1 << ENTITY_GENERATION_BITS) - 1;
const entity_id_t ECS_INVALID_ENTITY_ID_VAL = 0;
#define INVALID_ENTITY_ID ::ecs::EntityId(::ecs::ECS_INVALID_ENTITY_ID_VAL)

class EntityManager;
class EntityManagerInternal;

class EntityId
{
public:
  EntityId() = default;
  explicit EntityId(entity_id_t h) : handle(h) {}
  EntityId &operator=(const EntityId &) = default;
  explicit operator entity_id_t() const { return handle; }
  explicit operator bool() const { return handle != ECS_INVALID_ENTITY_ID_VAL; }
  bool operator==(const EntityId &rhs) const { return handle == rhs.handle; }
  bool operator!=(const EntityId &rhs) const { return handle != rhs.handle; }
  bool operator<(const EntityId &rhs) const { return handle < rhs.handle; }
  void reset() { handle = ECS_INVALID_ENTITY_ID_VAL; }
  unsigned index() const { return handle & ENTITY_INDEX_MASK; }

private:
  friend class ecs::EntityManager;
  friend class ecs::EntityManagerInternal;
  friend unsigned get_generation(const EntityId);
  entity_id_t handle = ECS_INVALID_ENTITY_ID_VAL;
  unsigned generation() const { return (handle >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK; }
};

struct EidHash
{
  size_t operator()(ecs::EntityId val) const { return static_cast<size_t>(static_cast<ecs::entity_id_t>(val)); }
};
struct EidHashFNV1a
{
  size_t operator()(ecs::EntityId val) const { return 0x01000193 * (0x811c9dc5 ^ ecs::entity_id_t(val)); }
};

}; // namespace ecs

template <typename T>
struct DebugConverter;
template <>
struct DebugConverter<ecs::EntityId>
{
  static ecs::entity_id_t getDebugType(const ecs::EntityId &v) { return ecs::entity_id_t(v); }
};

template <>
struct fmt::formatter<const ecs::EntityId, char> : fmt::formatter<uint32_t, char>
{
    auto format(const ecs::EntityId eid, fmt::buffered_context<char>& ctx) const
    {
        return formatter<uint32_t, char>::format(eid.index(), ctx);
    }
};

template <>
struct fmt::formatter<ecs::EntityId, char> : fmt::formatter<uint32_t, char>
{
    auto format(ecs::EntityId eid, fmt::buffered_context<char>& ctx) const
    {
        return formatter<uint32_t, char>::format(eid.index(), ctx);
    }
};