// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

//#include "nau/util/dag_globDef.h"
#include "nau/math/math.h"


inline void v_mat44_make_persp_reverse(nau::math::Matrix4 &dest, float wk, float hk, float zn, float zf)
{
  dest.setCol0(nau::math::Vector4(wk, 0, 0, 0));
  dest.setCol1(nau::math::Vector4(0, hk, 0, 0));
  dest.setCol2(nau::math::Vector4(0, 0, zn/(zn-zf), 1.f));
  dest.setCol3(nau::math::Vector4(0, 0, (zn*zf)/(zf-zn), 0));
}

inline void v_mat44_make_persp(nau::math::Matrix4 &dest, float wk, float hk, float zn, float zf)
{
  return v_mat44_make_persp_reverse(dest, wk, hk, zn, zf);
}


namespace framestateflags
{
    enum
    {
        M2VTM_OK = 0x0001,
        GLOBTM_OK = 0x0002,
        PERSP_OK = 0x0004,
        PROJTM_OK = 0x0008,
        V2MTM_OK = 0x0010,
        IDENT_WTM_SET = 0x0020,

        VIEWPORT_SET = 0x1000,
        VIEWPORT_VALID = 0x2000,
    };
};

struct alignas(16) FrameStateTM
{
  nau::math::Matrix4 d3d_mat[TM__NUM];
  nau::math::Matrix4 globtm;
  Driver3dPerspective persp;
  uint32_t flags;

public:
  FrameStateTM() { init(); }

  void init()
  {
    globtm = nau::math::Matrix4::identity(); //v_mat44_ident(globtm);
    for (int i = 0; i < TM__NUM; ++i)
      d3d_mat[i] = globtm;
    flags = framestateflags::IDENT_WTM_SET;
    persp.wk = persp.hk = persp.zn = persp.zf = persp.ox = persp.oy = 0.f;
  }

  void calc_globtm()
  {
    if (flags & framestateflags::GLOBTM_OK)
      return;
    
    //v_mat44_mul(d3d_mat[TM_LOCAL2VIEW], d3d_mat[TM_VIEW], d3d_mat[TM_WORLD]); //
    d3d_mat[TM_LOCAL2VIEW] = d3d_mat[TM_VIEW] * d3d_mat[TM_WORLD];

    calcglobtm(d3d_mat[TM_LOCAL2VIEW], d3d_mat[TM_PROJ], globtm);

    flags |= framestateflags::GLOBTM_OK | framestateflags::M2VTM_OK;
  }

  void calc_m2vtm() { calc_globtm(); }
  void calc_v2mtm()
  {
    if (flags & framestateflags::V2MTM_OK)
      return;
    nau::math::Matrix4 itmView, itmWorld;
    //v_mat44_inverse43(itmView, d3d_mat[TM_VIEW]);
    //v_mat44_inverse43(itmWorld, d3d_mat[TM_WORLD]);
    itmView = inverse(d3d_mat[TM_VIEW]);
    itmWorld = inverse(d3d_mat[TM_WORLD]);
    
    //v_mat44_mul(d3d_mat[TM_VIEW2LOCAL], itmWorld, itmView);
    d3d_mat[TM_VIEW2LOCAL] = itmWorld * itmView;

    flags |= framestateflags::V2MTM_OK;
  }

  // d3d:: functions
  __forceinline void calcproj(const Driver3dPerspective &p, nau::math::Matrix4 &proj_tm)
  {
    v_mat44_make_persp(proj_tm, p.wk, p.hk, p.zn, p.zf);
      if(p.ox != 0.f || p.oy != 0.f)
      {
          // proj_tm.col2 = v_add(proj_tm.col2, v_make_vec4f(p.ox, p.oy, 0.f, 0.f));
      }
  }

  __forceinline void calcglobtm(const nau::math::Matrix4 &view_tm, const nau::math::Matrix4 &proj_tm, nau::math::Matrix4 &result)
  {
    result = proj_tm * view_tm; //v_mat44_mul(result, proj_tm, view_tm);
  }

  __forceinline void calcglobtm(const nau::math::Matrix4 &view_tm, const Driver3dPerspective &p, nau::math::Matrix4 &result)
  {
    nau::math::Matrix4 proj;
    calcproj(p, proj);
    calcglobtm(view_tm, proj, result);
  }

  __forceinline void setpersp(const Driver3dPerspective &p, nau::math::Matrix4 *proj_tm)
  {
    persp = p;
    calcproj(p, d3d_mat[TM_PROJ]);

    flags &= ~(framestateflags::GLOBTM_OK | framestateflags::PROJTM_OK);
    flags |= framestateflags::PERSP_OK;

    if (proj_tm)
    {
      //v_stu(proj_tm->m[0], d3d_mat[TM_PROJ].col0);
      //v_stu(proj_tm->m[1], d3d_mat[TM_PROJ].col1);
      //v_stu(proj_tm->m[2], d3d_mat[TM_PROJ].col2);
      //v_stu(proj_tm->m[3], d3d_mat[TM_PROJ].col3);
      *proj_tm = d3d_mat[TM_PROJ];
    }
  }

  __forceinline bool validatepersp(nau::math::Matrix4 &proj)
  {
      // TODO: fix it
//    nau::math::Vector4 zero = v_zero();
//    nau::math::Vector4 result = v_cmp_eq(v_and(proj.col0, (vec4f)V_CI_MASK0111), zero);
//    result = v_and(result, v_cmp_eq(v_and(proj.col1, (vec4f)V_CI_MASK1011), zero));
//    result = v_and(result, v_cmp_eq(v_and(proj.col2, (vec4f)V_CI_MASK1100), zero));
//    result = v_and(result, v_cmp_eq(v_and(proj.col3, (vec4f)V_CI_MASK1101), zero));
//    result = v_andnot(v_and(v_cmp_eq(proj.col0, zero), (vec4f)V_CI_MASK1000), result);
//    result = v_andnot(v_and(v_cmp_eq(proj.col1, zero), (vec4f)V_CI_MASK0100), result);
//    result = v_andnot(v_and(v_cmp_eq(proj.col2, zero), (vec4f)V_CI_MASK0010), result);
//    result = v_andnot(v_and(v_cmp_eq(proj.col3, zero), (vec4f)V_CI_MASK0010), result);
//    result = v_and(result, v_cmp_eq(v_and(proj.col2, (vec4f)V_CI_MASK0001), V_C_UNIT_0001));
//#if !_TARGET_SIMD_SSE
//    result = v_and(result, v_perm_yzwx(result));
//    result = v_and(result, v_perm_zwxy(result));
//
//    return !v_test_vec_x_eqi_0(result);
//#else
//    return _mm_movemask_ps(result) == 0xF;
//#endif

      return true;
  }

  __forceinline bool validatepersp(const Driver3dPerspective &p)
  {
    nau::math::Matrix4 projTm;
    calcproj(p, projTm);
    return validatepersp(projTm);
  }

  __forceinline bool getpersp(Driver3dPerspective &p)
  {
    if (!(flags & framestateflags::PERSP_OK))
    {
      if (validatepersp(d3d_mat[TM_PROJ]))
      {
        float c3z = d3d_mat[TM_PROJ].getCol3().getZ(); //v_extract_z(d3d_mat[TM_PROJ].col3);
        float c2z = d3d_mat[TM_PROJ].getCol2().getZ();//v_extract_z(d3d_mat[TM_PROJ].col2);
        persp.zf = -c3z / c2z; // v_mat44_make_persp defaults to reverse projection.
        persp.zn = c3z / (1.f - c2z);
        persp.wk = d3d_mat[TM_PROJ].getCol0().getX();//v_extract_x(d3d_mat[TM_PROJ].col0);
        persp.hk = d3d_mat[TM_PROJ].getCol1().getY();//v_extract_y(d3d_mat[TM_PROJ].col1);
        persp.ox = 0.f;
        persp.oy = 0.f;
        flags |= framestateflags::PERSP_OK;
      }
      else
        return false;
    }

    p = persp;

    return true;
  }

  __forceinline void setglobtm(nau::math::Matrix4 &tm)
  {
    //globtm.col0 = v_ldu(tm.m[0]);
    //globtm.col1 = v_ldu(tm.m[1]);
    //globtm.col2 = v_ldu(tm.m[2]);
    //globtm.col3 = v_ldu(tm.m[3]);
    globtm = tm;
    
    flags =
      (flags & ~(framestateflags::M2VTM_OK | framestateflags::PROJTM_OK | framestateflags::PERSP_OK)) | framestateflags::GLOBTM_OK;
  }

  //__forceinline void settm(int which, const nau::math::Matrix4 *m)
  //{
  //  switch (which)
  //  {
  //    case TM_WORLD:
  //      if (m != &nau::math::Matrix4::identity())
  //        flags &= ~framestateflags::IDENT_WTM_SET;
  //      else if (flags & framestateflags::IDENT_WTM_SET)
  //        return;
  //      else
  //        flags |= framestateflags::IDENT_WTM_SET;
  //      [[fallthrough]];
  //    case TM_VIEW: flags &= ~(framestateflags::GLOBTM_OK | framestateflags::M2VTM_OK | framestateflags::V2MTM_OK); break;
  //    case TM_PROJ: flags &= ~(framestateflags::GLOBTM_OK | framestateflags::PROJTM_OK | framestateflags::PERSP_OK); break;
  //    default: NAU_ASSERT(0, "settm({}) is not allowed", which); return;
  //  }
  //  NAU_ASSERT(m);
  //  //d3d_mat[which].col0 = v_ldu(m->m[0]);
  //  //d3d_mat[which].col1 = v_ldu(m->m[1]);
  //  //d3d_mat[which].col2 = v_ldu(m->m[2]);
  //  //d3d_mat[which].col3 = v_ldu(m->m[3]);
  //  d3d_mat[which] = *m;
  //}

  //__forceinline void settm(int which, const nau::math::Matrix4 &t)
  //{
  //  switch (which)
  //  {
  //    case TM_WORLD:
  //      if (&t != &nau::math::Matrix4::identity())
  //        flags &= ~framestateflags::IDENT_WTM_SET;
  //      else if (flags & framestateflags::IDENT_WTM_SET)
  //        return;
  //      else
  //        flags |= framestateflags::IDENT_WTM_SET;
  //      [[fallthrough]];
  //    case TM_VIEW: flags &= ~(framestateflags::GLOBTM_OK | framestateflags::M2VTM_OK | framestateflags::V2MTM_OK); break;
  //    case TM_PROJ: flags &= ~(framestateflags::GLOBTM_OK | framestateflags::PROJTM_OK | framestateflags::PERSP_OK); break;
  //    default: NAU_ASSERT(0, "settm({}) is not allowed", which); return;
  //  }
  //  //v_mat44_make_from_43cu(d3d_mat[which], &t.m[0][0]);
  //  d3d_mat[which] = t;
  //}

  __forceinline void settm(int which, const nau::math::Matrix4 &m)
  {
    switch (which)
    {
      case TM_WORLD: flags &= ~framestateflags::IDENT_WTM_SET; [[fallthrough]];
      case TM_VIEW: flags &= ~(framestateflags::GLOBTM_OK | framestateflags::M2VTM_OK | framestateflags::V2MTM_OK); break;
      case TM_PROJ: flags &= ~(framestateflags::GLOBTM_OK | framestateflags::PROJTM_OK | framestateflags::PERSP_OK); break;
      default: NAU_ASSERT(0, "settm({}) is not allowed", which); return;
    }
    d3d_mat[which] = m;
  }

  __forceinline const nau::math::Matrix4 &gettm_cref(int which)
  {
    NAU_ASSERT((uint32_t)which < TM__NUM, "gettm({})", which);
    switch (which)
    {
      case TM_LOCAL2VIEW: calc_m2vtm(); break;
      case TM_VIEW2LOCAL: calc_v2mtm(); break;
      case TM_GLOBAL: calc_globtm(); break;
    }

    return d3d_mat[which];
  }

  __forceinline void gettm(int which, nau::math::Matrix4 *out_m)
  {
    NAU_ASSERT(out_m);
    const nau::math::Matrix4 &m = gettm_cref(which);
    *out_m = m;
    //v_stu(out_m->m[0], m.col0);
    //v_stu(out_m->m[1], m.col1);
    //v_stu(out_m->m[2], m.col2);
    //v_stu(out_m->m[3], m.col3);
  }

  __forceinline void gettm(int which, nau::math::Matrix4 &out_m) { out_m = gettm_cref(which); }

  __forceinline void getm2vtm(nau::math::Matrix4 &tm) { gettm(TM_LOCAL2VIEW, tm); }

  __forceinline void getglobtm(nau::math::Matrix4 &tm)
  {
    calc_globtm();
    tm = globtm;
    //v_stu(tm.m[0], globtm.col0);
    //v_stu(tm.m[1], globtm.col1);
    //v_stu(tm.m[2], globtm.col2);
    //v_stu(tm.m[3], globtm.col3);
  }

  __forceinline void setglobtm(const nau::math::Matrix4 &tm)
  {
    globtm = tm;
    flags =
      (flags & ~(framestateflags::M2VTM_OK | framestateflags::PROJTM_OK | framestateflags::PERSP_OK)) | framestateflags::GLOBTM_OK;
  }
};
