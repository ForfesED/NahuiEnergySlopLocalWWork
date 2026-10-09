// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include "drv3d_DX12/device.h"

using namespace drv3d_dx12;

namespace
{
void report_resources_impl(const PipelineStageStateBase &state, uint32_t b_reg_mask, uint32_t t_reg_mask, uint32_t u_reg_mask,
  uint32_t s_reg_mask, uint32_t s_reg_with_cmp_mask)
{
  for (auto i : nau::math::LsbVisitor{b_reg_mask})
  {
    auto gpuPointer = state.bRegisters[i].BufferLocation;
    if (gpuPointer)
    {
      auto &bufferResource = state.bRegisterBuffers[i];
        NAU_LOG_DEBUG("DX12: ...B register slot {} with {:p} / {} @ {}...", i, gpuPointer, (void*)bufferResource.buffer, bufferResource.resourceId.get(),
        gpuPointer - bufferResource.buffer->GetGPUVirtualAddress());
    }
    else if (i == 0)
    {
      NAU_LOG_DEBUG("DX12: ...B register slot {} with register constant buffer...", i);
    }
    else
    {
      NAU_LOG_DEBUG("DX12: ...B register slot {} has no buffer bound to...", i);
    }
  }

  for (auto i : nau::math::LsbVisitor{t_reg_mask})
  {
    auto &slot = state.tRegisters[i];
    if (slot.image)
    {
      NAU_LOG_DEBUG("DX12: ...T register slot {} with texture {:p} ({}) and view {}...", i, (void*)slot.image->getHandle(), (int)slot.image->getType(),
        slot.view.ptr);
    }
    else if (slot.buffer)
    {
      NAU_LOG_DEBUG("DX12: ...T register slot {} with buffer {:p} and view {}...", i, (void*)slot.buffer.buffer, slot.view.ptr);
    }
    else
    {
      NAU_LOG_DEBUG("DX12: ...T register slot {} has no resource bound to, null resource is used instead...", i);
    }
  }

  for (auto i : nau::math::LsbVisitor{u_reg_mask})
  {
    auto &slot = state.uRegisters[i];
    if (slot.image)
    {
      NAU_LOG_DEBUG("DX12: ...U register slot {} with texture {:p} ({}) and view {}...", i, (void*)slot.image->getHandle(), (int)slot.image->getType(),
        slot.view.ptr);
    }
    else if (slot.buffer)
    {
      NAU_LOG_DEBUG("DX12: ...U register slot {} with buffer {:p} and view {}...", i, (void*)slot.buffer.buffer, slot.view.ptr);
    }
    else
    {
      NAU_LOG_DEBUG("DX12: ...U register slot {} has no resource bound to, null resource is used instead...", i);
    }
  }

  for (auto i : nau::math::LsbVisitor{s_reg_mask})
  {
    auto slot = state.sRegisters[i];
    if (slot.ptr)
    {
      if (s_reg_with_cmp_mask & (1u << i))
      {
        NAU_LOG_DEBUG("DX12: ...S register slot {} with comparison sampler {}...", i, slot.ptr);
      }
      else
      {
        NAU_LOG_DEBUG("DX12: ...S register slot {} with sampler {}...", i, slot.ptr);
      }
    }
    else
    {
      NAU_LOG_DEBUG("DX12: ...S register slot {} has no sampler bound to!...", i);
    }
  }

  // TODO: bindless is a global state and we don't have any debug info for those
}
} // namespace

void debug::report_resources(const PipelineStageStateBase &state, ComputePipeline *pipe)
{
  const auto &header = pipe->getHeader();
  report_resources_impl(state, header.resourceUsageTable.bRegisterUseMask, header.resourceUsageTable.tRegisterUseMask,
    header.resourceUsageTable.uRegisterUseMask, header.resourceUsageTable.sRegisterUseMask, header.sRegisterCompareUseMask);
}

void debug::report_resources(const PipelineStageStateBase &vs, const PipelineStageStateBase &ps, BasePipeline *base_pipe)
{
  const auto &signature = base_pipe->getSignature();
  const auto &psHeader = base_pipe->getPSHeader();
  report_resources_impl(vs, signature.vsCombinedBRegisterMask, signature.vsCombinedTRegisterMask, signature.vsCombinedURegisterMask,
    signature.vsCombinedSRegisterMask, base_pipe->getVertexShaderSamplerCompareMask());
  report_resources_impl(ps, psHeader.resourceUsageTable.bRegisterUseMask, psHeader.resourceUsageTable.tRegisterUseMask,
    psHeader.resourceUsageTable.uRegisterUseMask, psHeader.resourceUsageTable.sRegisterUseMask, psHeader.sRegisterCompareUseMask);
}
