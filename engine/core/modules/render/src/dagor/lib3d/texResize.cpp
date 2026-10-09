// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include "nau/3d/tql.h"
#include "nau/3d/dag_tex3d.h"
#include "nau/math/dag_adjpow2.h"
#include "nau/diag/logging.h"

// #define TRACE debug  // trace resource manager's DDSx loads
#ifndef TRACE
#define TRACE(...) ((void)0)
#endif

bool tql::resizeTexture(BaseTexture *t, unsigned w, unsigned h, unsigned d, unsigned l, unsigned tex_ld_lev)
{
  TextureInfo ti;
  t->getinfo(ti, 0);

  unsigned tex_w = ti.w, tex_h = ti.h, tex_d = ti.d, tex_a = ti.a, tex_l = ti.mipLevels, a = 1;
  unsigned tex_lev = nau::math::get_log2i(Vectormath::max(Vectormath::max(ti.w, ti.h), ti.d));
  unsigned start_src_level = tex_ld_lev >= tex_lev ? 0 : tex_lev - tex_ld_lev;

  if (t->restype() == RES3D_CUBETEX)
  {
    d = 1;
    a = 6;
  }
  else if (t->restype() == RES3D_ARRTEX)
  {
    a = d;
    d = 1;
  }

  NAU_ASSERT_RETURN(tex_a == a, false, "restype={} a={} tex_a={} tex={}x{}x{},L{} -> {}x{}x{},L{}", t->restype(), a, tex_a, tex_w,
    tex_h, Vectormath::max(tex_d, tex_a), tex_l, w, h, Vectormath::max(d, a), l);

  if (tex_w == w && tex_h == h && tex_d == d && tex_l == l)
    return true;
  if (tex_w >= w && tex_h >= h && tex_d >= d && tex_l >= l)
  {
    // resize down
    unsigned lev_ofs = 0;
    while (lev_ofs < tex_l &&
           (Vectormath::max(tex_w >> lev_ofs, 1u) != w || Vectormath::max(tex_h >> lev_ofs, 1u) != h || Vectormath::max(tex_d >> lev_ofs, tex_a) != Vectormath::max(d, a)))
      lev_ofs++;
    if (Vectormath::max(tex_w >> lev_ofs, 1u) != w || Vectormath::max(tex_h >> lev_ofs, 1u) != h || Vectormath::max(tex_d >> lev_ofs, tex_a) != Vectormath::max(d, a))
    {
      NAU_LOG_ERROR("can't resize tex={:p}({}) {}x{}x{},L{}  to {}x{}x{},L{}", (void*)t, t->getTexName(), tex_w, tex_h, Vectormath::max(tex_d, tex_a), tex_l, w, h,
          Vectormath::max(d, a), l);
      return false;
    }

    if (!t->downSize(w, h, d, l, start_src_level, lev_ofs))
    {
      return false;
    }
  }
  else
  {
    // resize up
    unsigned lev_ofs = 0;
    while (
      lev_ofs < 16 && (Vectormath::max(w >> lev_ofs, 1u) != tex_w || Vectormath::max(h >> lev_ofs, 1u) != tex_h || Vectormath::max(d >> lev_ofs, a) != Vectormath::max(tex_d, tex_a)))
      lev_ofs++;
    if (Vectormath::max(w >> lev_ofs, 1u) != tex_w || Vectormath::max(h >> lev_ofs, 1u) != tex_h || Vectormath::max(d >> lev_ofs, a) != Vectormath::max(tex_d, tex_a))
    {
      NAU_LOG_ERROR("can't resize tex={:p}({}) {}x{}x{},L{}  to {}x{}x{},L{}", (void*)t, t->getTexName(), tex_w, tex_h, Vectormath::max(tex_d, tex_a), tex_l, w, h,
          Vectormath::max(d, a), l);
      return false;
    }

    if (!t->upSize(w, h, d, l, start_src_level, lev_ofs))
    {
      return false;
    }
  }
  return true;
}
