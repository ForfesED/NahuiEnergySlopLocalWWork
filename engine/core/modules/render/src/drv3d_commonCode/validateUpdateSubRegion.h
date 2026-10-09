// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

static inline bool validate_update_sub_region_params(BaseTexture *src, int src_subres_idx, int src_x, int src_y, int src_z, int src_w,
  int src_h, int src_d, BaseTexture *dst, int dst_subres_idx, int dst_x, int dst_y, int dst_z)
{
  NAU_ASSERT_RETURN(src_x >= 0 && src_y >= 0 && src_z >= 0 && src_w > 0 && src_h > 0 && src_d > 0, /*return*/ false,
    "Invalid updateSubRegion src: {},{},{}, {}x{}x{}, {}", src_x, src_y, src_z, src_w, src_h, src_d, src->getTexName());
  NAU_ASSERT_RETURN(dst_x >= 0 && dst_y >= 0 && dst_z >= 0, /*return*/ false, "Invalid updateSubRegion dst: {},{},{}, {}", dst_x, dst_y,
    dst_z, dst->getTexName());

  TextureInfo si, di;
  src->getinfo(si, 0);
  dst->getinfo(di, 0);
  int smip = src_subres_idx % si.mipLevels, dmip = dst_subres_idx % di.mipLevels;

  NAU_ASSERT_RETURN(src_subres_idx / si.mipLevels < si.a, /*return*/ false,
    "Invalid updateSubRegion src: subres=%d -> mip=%d slice=%d >= tex.slices=%d, %s", src_subres_idx, smip,
    src_subres_idx / si.mipLevels, si.a, src->getTexName());
  NAU_ASSERT_RETURN(dst_subres_idx / di.mipLevels < di.a, /*return*/ false,
    "Invalid updateSubRegion dst: subres=%d -> mip=%d slice=%d >= tex.slices=%d, %s", dst_subres_idx, smip,
    dst_subres_idx / di.mipLevels, di.a, dst->getTexName());

  int sw = si.w, sh = si.h, sd = si.d;
  if (smip)
    sw = Vectormath::max(sw >> smip, 1), sh = Vectormath::max(sh >> smip, 1), sd = Vectormath::max(sd >> smip, 1);
  if (is_bc_texformat(si.cflg))
  {
    sw = (sw + 3) & ~3;
    sh = (sh + 3) & ~3;
    NAU_ASSERT_RETURN((src_x & 3) == 0, false, "Invalid source x({}), must be block size (4) aligned", src_x);
    NAU_ASSERT_RETURN((src_y & 3) == 0, false, "Invalid source y({}), must be block size (4) aligned", src_y);
  }

  int dw = di.w, dh = di.h, dd = di.d;
  if (dmip)
    dw = Vectormath::max(dw >> dmip, 1), dh = Vectormath::max(dh >> dmip, 1), dd = Vectormath::max(dd >> dmip, 1);
  if (is_bc_texformat(di.cflg))
  {
    dw = (dw + 3) & ~3;
    dh = (dh + 3) & ~3;
    NAU_ASSERT_RETURN((dst_x & 3) == 0, false, "Invalid destination x({}), must be block size (4) aligned", dst_x);
    NAU_ASSERT_RETURN((dst_y & 3) == 0, false, "Invalid destination y({}), must be block size (4) aligned", dst_y);
  }

  NAU_ASSERT_RETURN(src_x + src_w <= sw && src_y + src_h <= sh && src_z + src_z <= sd && dst_x + src_w <= dw && dst_y + src_h <= dh &&
                     dst_z + src_z <= dd,
    /*return*/ false,
    "Invalid updateSubRegion rect size: (src mip {}: {}x{}x{}) {},{},{}, {}x{}x{} -> {},{},{} (dst mip {}: {}x{}x{})\n"
    "src {}x{}x{},L{} {}\ndst {}x{}x{},L{} {}",
    smip, sw, sh, sd, src_x, src_y, src_z, src_w, src_h, src_d, dst_x, dst_y, dst_z, dmip, dw, dh, dd, si.w, si.h, si.d, si.mipLevels,
    src->getTexName(), di.w, di.h, di.d, di.mipLevels, dst->getTexName());
  return true;
}
