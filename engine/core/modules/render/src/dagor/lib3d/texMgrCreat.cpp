// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include "texMgrData.h"
#include "nau/3d/dag_tex3d.h"
#include "nau/3d/dag_drv3d.h"
#include "nau/3d/dag_drv3dReset.h"
#include "nau/threading/dag_atomic.h"

using namespace texmgr_internal;

D3dResource *acquire_managed_res(D3DRESID rid)
{
  TextureFactory *factory = nullptr;
  int idx = RMGR.toIndex(rid);
  if (idx < 0)
    return nullptr;

  {
    TexMgrAutoLock lock;

    int rc = RMGR.getRefCount(idx);
    if (RMGR.getRefCount(idx) < 0)
      return NULL;
    if (rc & RMGR.RCBIT_FOR_REMOVE)
    {
      NAU_FAILURE("[TEXMGR] trying to get removed tex {:#x} ({})", (unsigned)rid, RMGR.getName(idx));
      return NULL;
    }

    rc = RMGR.incRefCount(idx);
    auto d3dRes = RMGR.getD3dRes(idx);
    auto f = RMGR.getFactory(idx);
    if (d3dRes || (!f && rc > 1))
    {
      if (rc == 1 && f)
        RMGR.decReadyForDiscardTex(idx);
      return d3dRes;
    }

    if (!f)
    {
      RMGR.decRefCount(idx);
      NAU_FAILURE("cannot get res: d3dRes & factory is NULL!\nname=<{}>", RMGR.getName(idx));
      return NULL;
    }

    TEX_REC_LOCK();
    RMGR.setD3dRes(idx, nullptr);
    TEX_REC_UNLOCK();
    factory = RMGR.getFactory(idx);
  }

  BaseTexture *t = factory->createTexture(rid);

  {
    TexMgrAutoLock lock;
    TEX_REC_LOCK();
    const bool isSameTextureLoadedInOtherThread = RMGR.getD3dRes(idx) == t; // under acquire_texmgr_lock
    // factory is not always creating a texture if it is already created.
    //  instead, it can returns current texture (pack factories do that)
    //   so, it can happen that two threads will try to load same texture, and return same pointer in t.
    //  then, under acquire_texmgr_lock, we check if texture rec is empty, and if it is, we fill it.
    //  If not, we should check if it is same texture, as loaded in other thread (isSameTextureLoadedInOtherThread)
    //  if there is still no d3dRes in record, or if it is SAME texture that is returned from factory
    //  we store it in record, and do not destroy it.
    //  otherwise it mean we concurrently have created two textures from factory
    //  (i.e. factory is not returning same Texture* on same file name, so it always creating new texture)
    //  and need to destroy currently loaded duplicate

    apply_mip_bias_rules(t, RMGR.getName(idx));

    D3dResource *exp_d3dres = isSameTextureLoadedInOtherThread ? t : nullptr;
    if (RMGR.exchangeD3dRes(idx, t, exp_d3dres) == exp_d3dres)
    {
      if (!RMGR.resQS[idx].getRdLev())
        RMGR.markUpdated(idx, t ? RMGR.resQS[idx].getLdLev() : 0);
      t = NULL;
    }

    TEX_REC_UNLOCK();

    del_d3dres(t);

    return RMGR.getD3dRes(idx);
  }
}

BaseTexture *acquire_managed_tex(D3DRESID id)
{
  D3dResource *res = acquire_managed_res(id);
  if (!res)
    return nullptr;
  int t = res->restype();
  if (t == RES3D_TEX || t == RES3D_CUBETEX || t == RES3D_VOLTEX || t == RES3D_ARRTEX || t == RES3D_CUBEARRTEX)
    return static_cast<BaseTexture *>(res);
  NAU_ASSERT(0, "non-tex res in acquire_managed_tex({:#x}), type={}, name={}", (unsigned)id, t, get_managed_res_name(id));
  return nullptr;
}

void release_managed_res_impl(D3DRESID rid, D3dResource *cmp)
{
  int idx = RMGR.toIndex(rid);
  if (idx < 0)
    return;

  int new_rc = RMGR.decRefCount(idx) & ~RMGR.RCBIT_FOR_REMOVE;
  if (new_rc > 0)
    return;

  if (new_rc < RMGR.INVALID_REFCOUNT)
    NAU_FAILURE("trying to free removed texture tid={:#x}", (unsigned)rid);

  if (new_rc < 0 && RMGR.getFactory(idx))
  {
    NAU_FAILURE("trying to release texture {:#x}='{}' with refcount = {}! factory={:p}", (unsigned)rid, RMGR.getName(idx), new_rc + 1,
      (void*)RMGR.getFactory(idx));
    TEX_REC_AUTO_LOCK();
    RMGR.initAllocatedRec(idx, nullptr, RMGR.getFactory(idx));
    return;
  }
  if (new_rc < 0)
  {
    NAU_LOG_ERROR("trying to release texture {:#x}='{}' with refcount = {}!", (unsigned)rid, RMGR.getName(idx), new_rc + 1);
    RMGR.incRefCount(idx);
    return;
  }

  // new_rc == 0 here and later
  if (RMGR.getFactory(idx))
  {
    // release original texture if needed
    TEX_REC_LOCK();
    RMGR.incReadyForDiscardTex(idx);
    if (should_release_tex(RMGR.baseTexture(idx)) || RMGR.isScheduledForRemoval(idx))
    {
      TEX_REC_UNLOCK();
      discard_unused_managed_texture(rid);
    }
    else
      TEX_REC_UNLOCK();
  }
  if (!RMGR.getFactory(idx) || RMGR.isScheduledForRemoval(idx))
  {
    if (cmp && RMGR.getD3dResRelaxed(idx) != cmp)
    {
      NAU_LOG_ERROR("release_managed_res_verified({:#x}={}, {:p} != {:p}) fails res verification", (unsigned)rid, RMGR.getName(idx), (void*)cmp,
        (void*)RMGR.getD3dResRelaxed(idx));
      RMGR.setD3dRes(idx, nullptr);
    }
    evict_managed_tex_and_id(rid);
  }
}
void release_managed_res(D3DRESID id) { release_managed_res_impl(id, nullptr); }
void release_managed_res_verified(D3DRESID &id, D3dResource *check_res)
{
  release_managed_res_impl(id, check_res);
  id = BAD_TEXTUREID;
}
void discard_unused_managed_texture(TEXTUREID tid)
{
  int idx = RMGR.toIndex(tid);
  if (idx < 0)
    return;

  TextureFactory *f = NULL;
  BaseTexture *t = NULL;
  TEX_REC_LOCK();
  if (RMGR.getRefCount(idx) == 0 && RMGR.getFactory(idx) && RMGR.getD3dRes(idx) && !RMGR.resQS[idx].isReading())
  {
    // NAU_LOG_DEBUG("[TEXMGR] free & destroy tex {:#x} (ptr={:p})", tid, RMGR.baseTexture(idx));
    f = RMGR.getFactory(idx);
    t = RMGR.baseTexture(idx);
    if (t)
    {
      RMGR.decReadyForDiscardTex(idx);
      RMGR.changeTexUsedMem(idx, 0, 0);
    }

    if (!t || t->getTID() == BAD_TEXTUREID) // only for non-texPackMgr2
    {
      RMGR.setD3dRes(idx, nullptr);
      RMGR.markUpdated(idx, 0);
    }
  }
  TEX_REC_UNLOCK();

  if (f && t)
    f->releaseTexture(t, tid);
}

bool texmgr_internal::evict_managed_tex_and_id(TEXTUREID tid)
{
  if (!RMGR.isValidID(tid, nullptr))
    return false;

  int idx = tid.index();
  acquire_texmgr_lock();

  int rc = RMGR.getRefCount(idx);
  if (rc < 0)
  {
    release_texmgr_lock();
    NAU_FAILURE("remove already removed texture {:#x}", (unsigned)tid);
    return false;
  }

  bool ret = false;
  if (rc == 0 || rc == RMGR.RCBIT_FOR_REMOVE)
  {
    // if (RMGR.isScheduledForRemoval(idx))
    //   NAU_LOG_DEBUG("[TEXMGR] remove (delayed) tex {:#x}, current ref = {}, ptr = {:p}",
    //     tid, RMGR.getRefCount(idx), RMGR.getD3dResRelaxed(idx));
    // else
    //   NAU_LOG_DEBUG("[TEXMGR] remove tex {:#x}, current ref = {}, ptr = {:p}",
    //     tid, RMGR.getRefCount(idx), RMGR.getD3dResRelaxed(idx));
    // NAU_LOG_DEBUG_dump_stack();
    TEX_REC_AUTO_LOCK();
    if (RMGR.getD3dResRelaxed(idx) && RMGR.getFactory(idx))
    {
      TEX_REC_UNLOCK();
      discard_unused_managed_texture(tid);
      TEX_REC_LOCK();
    }
    if (auto f = RMGR.getFactory(idx))
      f->onUnregisterTexture(tid);
    else if (auto res = RMGR.getD3dResRelaxed(idx))
    {
      if (res != d3d::get_backbuffer_tex() && res != d3d::get_secondary_backbuffer_tex())
      {
        RMGR.setD3dRes(idx, nullptr);
        del_d3dres(res);
      }
    }

    remove_from_managed_tex_map(idx);

    RMGR.clearReleasedRec(idx);
    RMGR.markUpdated(idx, 0);
    RMGR.releaseEntry(idx);
    ret = true;
  }
  else
  {
    // delayed
    RMGR.scheduleForRemoval(idx);
    NAU_LOG_ERROR("[TEXMGR] QUEUE for remove tex {:#x}, current ref = {}, ptr = {:p}", (unsigned)tid, RMGR.getRefCount(idx) & ~RMGR.RCBIT_FOR_REMOVE,
      (void*)RMGR.getD3dRes(idx));
    // NAU_LOG_DEBUG_dump_stack();
    ret = false;
  }
  release_texmgr_lock();

  return ret;
}
bool evict_managed_tex_id(TEXTUREID &id)
{
  bool ret = texmgr_internal::evict_managed_tex_and_id(id);
  id = BAD_TEXTUREID;
  return ret;
}

bool change_managed_texture(TEXTUREID tid, BaseTexture *new_texture, TextureFactory *factory)
{
  int idx = RMGR.toIndex(tid);
  if (idx < 0 || RMGR.getRefCount(idx) < 0)
    return false;

  TEX_REC_LOCK();
  TextureFactory *f = RMGR.getFactory(idx);
  BaseTexture *t = f ? RMGR.baseTexture(idx) : NULL;
  RMGR.setD3dRes(idx, new_texture);
  RMGR.setFactory(idx, factory);
  apply_mip_bias_rules(t, RMGR.getName(idx));
  TEX_REC_UNLOCK();

  if (f && t)
    f->releaseTexture(t, tid);
  return true;
}

void reset_managed_textures_streaming_state()
{
  acquire_texmgr_lock();
  {
    int cnt = 0;
    for (unsigned idx = 0, ie = RMGR.getAccurateIndexCount(); idx < ie; idx++)
      if (RMGR.getRefCount(idx) >= 0)
      {
        if (RMGR.getFactory(idx) && RMGR.resQS[idx].getLdLev() > 1 && RMGR.getD3dResRelaxed(idx))
        {
          BaseTexture *bt = RMGR.baseTexture(idx);
          if (!bt || bt->getTID() == BAD_TEXTUREID)
          {
            continue; // skip non-texPackMgr2 textures
          }
          if (!RMGR.downgradeTexQuality(idx, *bt, Vectormath::max<int>(RMGR.getLevDesc(idx, TQL_thumb), 1)))
          {
            continue; // could not downgrade, do not count
          }
          cnt++;
        }
      }
    NAU_LOG_DEBUG("reset_managed_textures_streaming_state: reset for {} managed textures", cnt);
  }
  release_texmgr_lock();
}

static void texmgr_before_device_reset(bool full_reset)
{
  if (!full_reset)
    return;
  acquire_texmgr_lock();
  int cnt = 0;
  for (unsigned idx = 0, ie = RMGR.getAccurateIndexCount(); idx < ie; idx++)
    if (RMGR.getRefCount(idx) >= 0)
    {
      if (RMGR.getFactory(idx) && RMGR.resQS[idx].getLdLev() > 1 && RMGR.getD3dResRelaxed(idx))
      {
        if (BaseTexture *t = RMGR.baseTexture(idx))
          if (t->getTID() == BAD_TEXTUREID) // skip non-texPackMgr2 textures
            continue;
        // NAU_LOG_DEBUG("{}: ldLev={} -> 1", RMGR.getName(idx), RMGR.resQS[idx].getLdLev());
        RMGR.resQS[idx].setMaxReqLev(
          (RMGR.getRefCount(idx) > 0) ? Vectormath::min(RMGR.resQS[idx].getLdLev(), RMGR.getLevDesc(idx, TQL_base)) : 1);
        RMGR.resQS[idx].setLdLev(1);
        RMGR.resQS[idx].setCurQL(TQL_stub);
        RMGR.changeTexUsedMem(idx, 0, 0);
        cnt++;
      }
    }
  NAU_LOG_DEBUG("texmgr: reset ldState for {} managed textures", cnt);
  release_texmgr_lock();
}
REGISTER_D3D_BEFORE_RESET_FUNC(texmgr_before_device_reset);
