// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/3d/rayTrace/dag_drvRayTrace.h"

#include "driver.h"
#include "resource_memory.h"

#include "resource_manager/basic_buffer.h"


namespace drv3d_dx12
{

#if D3D_HAS_RAY_TRACING
struct RaytraceAccelerationStructure : protected resource_manager::BasicBuffer
{
  D3D12_CPU_DESCRIPTOR_HANDLE handle{};

  using resource_manager::BasicBuffer::create;
  using resource_manager::BasicBuffer::getGPUPointer;
  using resource_manager::BasicBuffer::reset;

  ID3D12Resource *getResourceHandle() { return buffer.Get(); }

  size_t size() const { return bufferMemory.size(); }

  ResourceMemory getMemory() const { return bufferMemory; }
};
#endif

} // namespace drv3d_dx12
