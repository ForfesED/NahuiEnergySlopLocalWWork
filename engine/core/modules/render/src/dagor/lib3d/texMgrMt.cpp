// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include "texMgrData.h"
#include "nau/3d/dag_drv3d.h"


void enable_res_mgr_mt(bool enable, int max_tex_entry_count)
{
  using namespace texmgr_internal;
  int prev = mt_enabled;

  texmgr_internal::drv3d_cmd = &d3d::driver_command;
  if (prev)
    dag::enter_critical_section(crit_sec);
  if (enable)
    mt_enabled++;
  else if (mt_enabled > 0)
    mt_enabled--;
  else
    NAU_FAILURE("incorrect enable_tex_mgr_mt refcount={}", mt_enabled);

  if (!prev && mt_enabled)
  {
    NAU_LOG_DEBUG("d3dResMgr: multi-threaded access ENABLED  (reserving {} entries)", max_tex_entry_count);
    dag::create_critical_section(crit_sec, "tex_mgr");
    if (!RMGR.getAccurateIndexCount())
    {
      RMGR.term();
      RMGR.init(max_tex_entry_count);
      managed_tex_map_by_name.reserve(RMGR.getMaxTotalIndexCount());
      managed_tex_map_by_idx.resize(RMGR.getMaxTotalIndexCount());
      mem_set_0(managed_tex_map_by_idx);
    }
    else if (max_tex_entry_count)
      NAU_ASSERT(max_tex_entry_count <= RMGR.getMaxTotalIndexCount() && RMGR.getAccurateIndexCount() < max_tex_entry_count,
        "{}({}, {}) while indexCount={} and maxTotalIndexCount={}", __FUNCTION__, enable ? "true" : "false", max_tex_entry_count,
        RMGR.getAccurateIndexCount(), RMGR.getMaxTotalIndexCount());
  }
  else if (max_tex_entry_count > RMGR.getMaxTotalIndexCount())
    NAU_LOG_ERROR("d3dResMgr: cannot change reserved entries {} -> {} while in multi-threaded mode; disable it first!",
      RMGR.getMaxTotalIndexCount(), max_tex_entry_count);

  if (prev > 0)
    dag::leave_critical_section(crit_sec);

  if (prev && !mt_enabled)
  {
    dag::destroy_critical_section(crit_sec);
    NAU_LOG_DEBUG("d3dResMgr: multi-threaded access disabled (used {}/{} entries)", RMGR.getAccurateIndexCount(),
      RMGR.getMaxTotalIndexCount());
  }
}
