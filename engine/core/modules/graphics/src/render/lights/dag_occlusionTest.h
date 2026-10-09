// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#pragma once

#include "nau/math/dag_adjpow2.h"
#include "nau/math/math.h"

namespace nau::math
{

#define OCCLUSION_INVWBUFFER 1
#define OCCLUSION_WBUFFER 2
#define OCCLUSION_Z_BUFFER 3
#define OCCLUSION_BUFFER \
    OCCLUSION_INVWBUFFER  // OCCLUSION_WBUFFER////OCCLUSION_WBUFFER////OCCLUSION_WBUFFER//OCCLUSION_INVWBUFFER//OCCLUSION_Z_BUFFER//
#if OCCLUSION_BUFFER == OCCLUSION_Z_BUFFER
    static NAU_FORCE_INLINE Vector4 occlusion_depth_vmax(Vector4 a, Vector4 b)
    {
        return min(a, b);
    }
    static NAU_FORCE_INLINE Vector4 occlusion_depth_vmin(Vector4 a, Vector4 b)
    {
        return max(a, b);
    }
    static NAU_FORCE_INLINE Vector4 occlusion_depth_vcmp(Vector4 val, Vector4 zbuffer)
    {
        return v_cmp_ge(val, zbuffer);
    }
    static NAU_FORCE_INLINE int occlusion_depth_vtest(Vector4 val, Vector4 zbuffer)
    {
        return v_test_vec_x_ge(val, zbuffer);
    }
#elif OCCLUSION_BUFFER == OCCLUSION_WBUFFER
    static NAU_FORCE_INLINE Vector4 occlusion_depth_vmax(Vector4 a, Vector4 b)
    {
        return max(a, b);
    }
    static NAU_FORCE_INLINE Vector4 occlusion_depth_vmin(Vector4 a, Vector4 b)
    {
        return min(a, b);
    }
    static NAU_FORCE_INLINE Vector4 occlusion_depth_vcmp(Vector4 val, Vector4 zbuffer)
    {
        return v_cmp_ge(zbuffer, val);
    }
    static NAU_FORCE_INLINE int occlusion_depth_vtest(Vector4 val, Vector4 zbuffer)
    {
        return v_test_vec_x_ge(zbuffer, val);
    }
    static NAU_FORCE_INLINE Vector4 occlusion_convert_to_internal_zbuffer(Vector4 minw)
    {
        return minw;
    }
    static NAU_FORCE_INLINE Vector4 occlusion_convert_from_internal_zbuffer(Vector4 minw)
    {
        return minw;
    }
#elif OCCLUSION_BUFFER == OCCLUSION_INVWBUFFER
    static NAU_FORCE_INLINE Vector4 occlusion_depth_vmax(Vector4 a, Vector4 b)
    {
        return min(a, b);
    }
    static NAU_FORCE_INLINE Vector4 occlusion_depth_vmin(Vector4 a, Vector4 b)
    {
        return max(a, b);
    }
    static NAU_FORCE_INLINE BoolInVec occlusion_depth_vcmp(Vector4 val, Vector4 zbuffer)
    {
        return FloatInVec(val.get128()) >= FloatInVec(zbuffer.get128());
    }
    static NAU_FORCE_INLINE int occlusion_depth_vtest(Vector4 val, Vector4 zbuffer)
    {
        return val.getX() >= zbuffer.getX();
    }

    NAU_FORCE_INLINE Vector4 v_rcp(Vector4 a)
    {
        __m128 y0 = _mm_rcp_ps(a.get128());
        return Vector4(_mm_sub_ss(_mm_add_ss(y0, y0), _mm_mul_ss(a.get128(), _mm_mul_ss(y0, y0))));
    }

    NAU_FORCE_INLINE Vector4 v_rcp_x(Vector4 a)
    {
        __m128 y0 = _mm_rcp_ss(a.get128());
        return Vector4(_mm_sub_ss(_mm_add_ss(y0, y0), _mm_mul_ss(a.get128(), _mm_mul_ss(y0, y0))));
    }

    static NAU_FORCE_INLINE Vector4 occlusion_convert_to_internal_zbuffer(Vector4 minw)
    {
        return v_rcp_x(minw);
    }
    static NAU_FORCE_INLINE Vector4 occlusion_convert_from_internal_zbuffer(Vector4 minw)
    {
        return v_rcp(minw);
    }
    NAU_FORCE_INLINE void v_stu_half(void* m, Vector4 v)
    {
        _mm_storel_epi64((__m128i*)m, _mm_castps_si128(v.get128()));
    }

    NAU_FORCE_INLINE IVector4 v_ldui_half(const void* m)
    {
        return IVector4(_mm_loadl_epi64((__m128i const*)m));
    }
    NAU_FORCE_INLINE Vector4 v_ldu_half(const void* m)
    {
        return Vector4(_mm_castsi128_ps(v_ldui_half(m).get128()));
    }

#endif

#define V_SHUFFLE(v, mask) Vector4(_mm_shuffle_ps(v.get128(), v.get128(), mask))
#define V_SHUFFLE_REV(v, maskW, maskZ, maskY, maskX) V_SHUFFLE(v, _MM_SHUFFLE(maskW, maskZ, maskY, maskX))
#define V_SHUFFLE_FWD(v, maskX, maskY, maskZ, maskW) V_SHUFFLE(v, _MM_SHUFFLE(maskW, maskZ, maskY, maskX))

    NAU_FORCE_INLINE Vector4 v_perm_yzxw(Vector4 a)
    {
        return V_SHUFFLE(a, _MM_SHUFFLE(3, 0, 2, 1));
    }

    NAU_FORCE_INLINE Vector4 v_perm_yybb(Vector4 xyzw, Vector4 abcd)
    {
        return Vector4(_mm_shuffle_ps(xyzw.get128(), abcd.get128(), _MM_SHUFFLE(1, 1, 1, 1)));
    }

    NAU_FORCE_INLINE Vector4 v_rot_1(Vector4 a)
    {
        return V_SHUFFLE(a, _MM_SHUFFLE(0, 3, 2, 1));
    }

    NAU_FORCE_INLINE Vector4 v_rot_2(Vector4 a)
    {
        return V_SHUFFLE(a, _MM_SHUFFLE(1, 0, 3, 2));
    }

    NAU_FORCE_INLINE Vector4 v_perm_xaxa(Vector4 xyzw, Vector4 abcd)
    {
        return v_perm_yzxw(Vector4(_mm_shuffle_ps(xyzw.get128(), abcd.get128(), _MM_SHUFFLE(0, 0, 0, 0))));
    }

    NAU_FORCE_INLINE Vector4 v_perm_xxyy(Vector4 a)
    {
        return V_SHUFFLE_REV(a, 1, 1, 0, 0);
    }

    NAU_FORCE_INLINE Vector4 v_perm_zzww(Vector4 a)
    {
        return V_SHUFFLE_REV(a, 3, 3, 2, 2);
    }

    NAU_FORCE_INLINE Vector4 v_perm_yzwx(Vector4 a)
    {
        return v_rot_1(a);
    }

    NAU_FORCE_INLINE Vector4 v_perm_xzxz(Vector4 b)
    {
        return V_SHUFFLE_FWD(b, 0, 2, 0, 2);
    }

    NAU_FORCE_INLINE Vector4 v_perm_xzac(Vector4 xyzw, Vector4 abcd)
    {
        return Vector4(_mm_shuffle_ps(xyzw.get128(), abcd.get128(), _MM_SHUFFLE(2, 0, 2, 0)));
    }

    NAU_FORCE_INLINE Vector4 v_perm_xbzw(Vector4 xyzw, Vector4 abcd)
    {
        return Vector4(_mm_blend_ps(xyzw.get128(), abcd.get128(), 2));
    }

    NAU_FORCE_INLINE Vector4 v_perm_xycw(Vector4 xyzw, Vector4 abcd)
    {
        return Vector4(_mm_blend_ps(xyzw.get128(), abcd.get128(), 4));
    }

    NAU_FORCE_INLINE Vector4 v_perm_xyzd(Vector4 xyzw, Vector4 abcd)
    {
        return Vector4(_mm_blend_ps(xyzw.get128(), abcd.get128(), 8));
    }

    NAU_FORCE_INLINE void vis_transform_points_4(Vector4* dest, Vector4 x, Vector4 y, Vector4 z, const Matrix4& mat)
    {
#define COMP(c, attr)                                             \
    Vector4 res_##c = Vector4(mat.getCol3().get##attr());         \
    res_##c = (z * Vector4(mat.getCol2().get##attr()) + res_##c); \
    res_##c = (y * Vector4(mat.getCol1().get##attr()) + res_##c); \
    res_##c = (x * Vector4(mat.getCol0().get##attr()) + res_##c); \
    dest[c] = res_##c;
        COMP(0, X);
        COMP(1, Y);
        COMP(2, Z);
        COMP(3, W);
#undef COMP
    }

    /// return zero if not visible in frustum or if bbox screen size is smaller, than threshold. 1, if all vertex are in front of near plane, -1 (and fullscreen rect) otherwise
    // also, screen_box is minX, maxX, minY, maxY - in clipspace coordinates  (-1, -1) .. (1,1), minmax_w is wWwW (minw, maxw, minw, maxw)
    NAU_FORCE_INLINE int
    v_screen_size_b(Vector3 bmin, Vector3 bmax, Vector3 threshold, Vector4& screen_box, Vector4& minmax_w, const Matrix4& clip)
    {
        // get aabb points (SoA)
        Vector4 minmax_x = v_perm_yzxw(Vector4(_mm_shuffle_ps(bmin.get128(), bmax.get128(), _MM_SHUFFLE(0, 0, 0, 0))));
        Vector4 minmax_y = v_perm_yybb(Vector4(bmin), Vector4(bmax));
        Vector4 minmax_z_0 = Vector4(bmin);
        Vector4 minmax_z_1 = Vector4(bmax);

        // transform points to clip space
        Vector4 points_cs_0[4];
        Vector4 points_cs_1[4];

        vis_transform_points_4(points_cs_0, minmax_x, minmax_y, minmax_z_0, clip);
        vis_transform_points_4(points_cs_1, minmax_x, minmax_y, minmax_z_1, clip);

        // calculate -w
        Vector4 points_cs_0_negw = -points_cs_0[3];
        Vector4 points_cs_1_negw = -points_cs_1[3];

#define NOUT(a, b, c, d) (unsigned(-_mm_movemask_ps(_mm_or_ps(_mm_cmpgt_ps(a.get128(), b.get128()), _mm_cmpgt_ps(c.get128(), d.get128())))))
        unsigned nout;
        nout = (NOUT(points_cs_0[0], points_cs_0_negw, points_cs_1[0], points_cs_1_negw));
        nout &= (NOUT(points_cs_0[3], points_cs_0[0], points_cs_1[3], points_cs_1[0]));
        nout &= (NOUT(points_cs_0[1], points_cs_0_negw, points_cs_1[1], points_cs_1_negw));
        nout &= (NOUT(points_cs_0[3], points_cs_0[1], points_cs_1[3], points_cs_1[1]));
        nout &= (NOUT(points_cs_0[2], Vector4(0), points_cs_1[2], Vector4(0)));
        nout &= (NOUT(points_cs_0[3], points_cs_0[2], points_cs_1[3], points_cs_1[2]));

        // merge "not outside" flags
        if ((nout & (1 << 31)) == 0)
            return 0;
#undef NOUT

        Vector4 min_w = min(points_cs_0[3], points_cs_1[3]);
        min_w = min(min_w, v_rot_2(min_w));
        min_w = min(min_w, v_rot_1(min_w));

        Vector4 max_w = max(points_cs_0[3], points_cs_1[3]);
        max_w = max(max_w, v_rot_2(max_w));
        minmax_w = v_perm_xaxa(min_w, max(max_w, v_rot_1(max_w)));

        if (min_w.getX() < MATH_SMALL_NUMBER)
        {
            return -1;
        }

        Vector4 inv_cs0_3 = v_rcp(points_cs_0[3]);
        Vector4 inv_cs1_3 = v_rcp(points_cs_1[3]);
        Vector4 xxxx0 = points_cs_0[0] * inv_cs0_3;
        Vector4 xxxx1 = points_cs_1[0] * inv_cs1_3;
        Vector4 yyyy0 = points_cs_0[1] * inv_cs0_3;
        Vector4 yyyy1 = points_cs_1[1] * inv_cs1_3;

        Vector4 point01 = Vector4(sseMergeH(xxxx0.get128(), yyyy0.get128()));  // xy, xy
        Vector4 point23 = Vector4(sseMergeL(xxxx0.get128(), yyyy0.get128()));  // xy, xy
        Vector4 point45 = Vector4(sseMergeH(xxxx1.get128(), yyyy1.get128()));  // xy, xy
        Vector4 point67 = Vector4(sseMergeL(xxxx1.get128(), yyyy1.get128()));  // xy, xy
        Vector4 minXY = min(min(point01, point23), min(point45, point67));
        minXY = min(minXY, v_rot_2(minXY));
        Vector4 maxXY = max(max(point01, point23), max(point45, point67));
        maxXY = max(maxXY, v_rot_2(maxXY));

        screen_box = Vector4(sseMergeH(minXY.get128(), maxXY.get128()));
        Vector4 screenSizeVisible = maxXY - minXY;
        auto screenSizeVisibleBool = FloatInVec(threshold.get128()) >= screenSizeVisible.get128();

        if ((screenSizeVisibleBool.getFlags() & 3ul) != 0)
            return 0;

        return 1;
    }

    template <int sizeX, int sizeY>
    class OcclusionTest
    {
    public:
        static constexpr int RESOLUTION_X = sizeX;
        static constexpr int RESOLUTION_Y = sizeY;
        static constexpr int mip_count(int w, int h)
        {
            return 1 + ((w > 1 && h > 1) ? mip_count(w >> 1, h >> 1) : 0);
        }
        static constexpr int mip_chain_count = mip_count(RESOLUTION_X, RESOLUTION_Y);

        static constexpr int get_log2(int w)
        {
            return 1 + (w > 2 ? get_log2(w >> 1) : 0);
        }
        static constexpr int pitch_shift = get_log2(RESOLUTION_X);

        static float* getZbuffer(int mip)
        {
            return zBuffer + mip_chain_offsets[mip];
        }
        static float* getZbuffer()
        {
            return zBuffer;
        }

        OcclusionTest()
        {
            clipToScreenVec = Vector4(0.5 * sizeX, -0.5 * sizeY, 0.5 * sizeX, 0.5 * sizeY);
            screenMax = Vector4(sizeX - 1, sizeX - 1, sizeY - 1, sizeY - 1);
            mip_chain_offsets[0] = 0;
            for (int mip = 1; mip < mip_chain_count; mip++)
                mip_chain_offsets[mip] = mip_chain_offsets[mip - 1] + (sizeX >> (mip - 1)) * (sizeY >> (mip - 1));
        }
        static void clear()
        {
#if OCCLUSION_BUFFER == OCCLUSION_WBUFFER
            Vector4* dst = (Vector4*)zBuffer;
            Vector4 zfar = v_splats(1e6);
            for (int i = 0; i < mip_chain_size / 4; ++i, dst++)
                *dst = zfar;
#else
            memset(zBuffer, 0, mip_chain_size * sizeof(zBuffer[0]));
#endif
        }
        static void buildMips()
        {
            for (int mip = 1; mip < mip_chain_count; mip++)
                downsample4x_simda_max(getZbuffer(mip), getZbuffer(mip - 1), sizeX >> mip, sizeY >> mip);
        }
        enum
        {
            CULL_FRUSTUM = 0,
            VISIBLE = 1,
            CULL_OCCLUSION = 2
        };

        // max_test_mip define coarsest mip to test. The bigger number, the rougher cull tests are (but each test is faster)
        // return 0 if frustum culled
        // return 1 if visible
        // return 2 if occlusion culled
        static NAU_FORCE_INLINE int testVisibility(Vector3 bmin, Vector3 bmax, Vector3 threshold, const Matrix4& clip, int max_test_mip)
        {
            Vector4 minmax_w, clipScreenBox;
            // return v_screen_size_b(bmin, bmax, threshold, clipScreenBox, minmax_w, clip);
            int visibility = v_screen_size_b(bmin, bmax, threshold, clipScreenBox, minmax_w, clip);
            if (!visibility)
                return CULL_FRUSTUM;
            if (visibility == -1)
                return VISIBLE;  // todo: better check using rasterization?
            Vector4 screenBox = (clipScreenBox * v_perm_xxyy(clipToScreenVec) + v_perm_zzww(clipToScreenVec));
            screenBox = max(screenBox, Vector4(0));                     // on min part only
            screenBox = min(screenBox, screenMax);                      // on max part only
            IVector4 screenBoxi = IVector4(FromFloatTrunc(screenBox));  // todo: ceil max xy

            int sz[2] = {screenBoxi[1] - screenBoxi[0], screenBoxi[2] - screenBoxi[3]};
            // G_ASSERT(sz[1]>=0 && sz[0]>=0);//should not ever happen!
            int mip = std::min(std::max<int>(get_log2i(min(sz[0], sz[1])) - 1, 0), std::min(max_test_mip, mip_chain_count));
            return CULL_OCCLUSION - testCulledMip(screenBoxi[0], screenBoxi[1], screenBoxi[3], screenBoxi[2], mip, minmax_w);
        }

        static int testCulledFull(int xMin, int xMax, int yMin, int yMax, Vector4 minw)
        {
            NAU_ASSERT(xMin >= 0 && yMin >= 0 && xMax < sizeX && yMax < sizeX);
            return testCulledZbuffer(xMin, xMax, yMin, yMax, minw, zBuffer, 0);
        }

        static NAU_FORCE_INLINE int testCulledMip(int xMin, int xMax, int yMin, int yMax, int mip, Vector4 minw)
        {
            // G_ASSERT(xMin>=0 && yMin>=0 && xMax<sizeX && yMax<sizeX && mip < mip_chain_count-1);
            xMin >>= mip;
            yMin >>= mip;
            xMax = xMax >> mip;
            yMax = yMax >> mip;
            // NAU_CORE_DEBUG_LF("%d: sz - %dx%d %dx%d", mip, xMax-xMin+1, yMax-yMin+1, xMin, yMin);
            return testCulledZbuffer(xMin, xMax, yMin, yMax, minw, getZbuffer(mip), mip);
        }

    protected:
        static constexpr int mip_sum(int w, int h)
        {
            return w * h + ((w > 1 && h > 1) ? mip_sum(w >> 1, h >> 1) : 0);
        }
        static constexpr int mip_chain_size = mip_sum(RESOLUTION_X, RESOLUTION_Y);
        static constexpr unsigned bitShiftX = get_const_log2(RESOLUTION_X);
        static constexpr unsigned bitMaskX = (1 << bitShiftX) - 1;
        static constexpr unsigned bitMaskY = ~bitMaskX;

        static void downsample4x_simda_max(float* __restrict destData, float* __restrict srcData, int destW, int destH)
        {
            unsigned int srcPitch = destW * 2;
            NAU_ASSERT(destW > 1);  // we can implement last mip, if needed, later

            if (destW >= 4)
            {
                for (int y = 0; y < destH; y++, srcData += srcPitch)
                {
                    for (int x = 0; x < destW; x += 4, srcData += 8, destData += 4)
                    {
                        Vector4 up0 = Vector4(_mm_load_ps(srcData));
                        Vector4 up1 = Vector4(_mm_load_ps(srcData + 4));
                        Vector4 down0 = Vector4(_mm_load_ps(srcData + srcPitch));
                        Vector4 down1 = Vector4(_mm_load_ps(srcData + srcPitch + 4));
                        Vector4 left = occlusion_depth_vmax(up0, down0);
                        Vector4 right = occlusion_depth_vmax(up1, down1);
                        left = occlusion_depth_vmax(left, v_perm_yzwx(left));
                        right = occlusion_depth_vmax(right, v_perm_yzwx(right));
                        _mm_store_ps(destData, v_perm_xzac(left, right).get128());
                    }
                }
            }
            else
            {
                for (int y = 0; y < destH; y++, srcData += srcPitch)
                {
                    for (int x = 0; x < destW; x += 2, srcData += 4, destData += 2)
                    {
                        Vector4 up = Vector4(_mm_load_ps(srcData));
                        Vector4 down = Vector4(_mm_load_ps(srcData + srcPitch));
                        Vector4 left = occlusion_depth_vmax(up, down);
                        left = occlusion_depth_vmax(left, v_perm_yzwx(left));
                        v_stu_half(destData, v_perm_xzxz(left));
                    }
                }
            }
        }
        static float zBuffer[mip_chain_size];
        static Vector4 clipToScreenVec;
        static Vector4 screenMax;
        static int mip_chain_offsets[mip_chain_count];

        // return 0 if occluded
        static NAU_FORCE_INLINE int testCulledZbuffer(int xMin, int xMax, int yMin, int yMax, Vector4 minw, float* zbuffer, int mip)
        {
            const float* zbufferRow = zbuffer + (yMin << (pitch_shift - mip));
            const int pitch = (1 << (pitch_shift - mip));
            Vector4 closestPoint = occlusion_convert_to_internal_zbuffer(minw);

            if (xMax - xMin <= 1)  // very simple up to 2xN pixel test
            {
                zbufferRow += xMin;
                Vector4 farDepth;
                farDepth = v_ldu_half(zbufferRow);
                zbufferRow += pitch;
                for (int y = yMin + 1; y <= yMax; ++y, zbufferRow += pitch)
                    farDepth = occlusion_depth_vmax(farDepth, v_ldu_half(zbufferRow));

                if (xMax - xMin == 0)
                    return occlusion_depth_vtest(closestPoint, farDepth);

                farDepth = occlusion_depth_vmax(farDepth, v_rot_1(farDepth));
                return occlusion_depth_vtest(closestPoint, farDepth);
            }
            closestPoint = Vector4(closestPoint.getX());

            ///*
            {
                const int xEnd4 = xMin + ((xMax - xMin + 1) & (~3));
                const uint32_t endXMask = xEnd4 < xMax + 1 ? ((1 << ((xMax - xEnd4 + 1))) - 1) : 0;
                // NAU_CORE_DEBUG_LF("%dx%d %dx%d %f xEnd4=%d", xMin,yMin, xMax, yMax, v_extract_x(minw), xEnd4);
                zbufferRow += xMin;
                if (endXMask)
                {
                    for (int y = yMin; y <= yMax; ++y, zbufferRow += pitch)
                    {
                        const float* zbufferP = (float*)zbufferRow;
                        // int mask = 0;
                        int x = xMin;
                        for (; x < xEnd4; x += 4, zbufferP += 4)
                        {
                            auto passTest = occlusion_depth_vcmp(closestPoint, Vector4(_mm_loadu_ps(zbufferP)));
                            if (passTest.getFlags())
                            {
                                // NAU_CORE_DEBUG_LF("entry: %dx%d 0x%X mask (%f %f %f %f)", x, y, _mm_movemask_ps(passTest), V4D(v_ldu(zbuffer)));
                                return 1;
                            }
                        }
                        auto passTest = occlusion_depth_vcmp(closestPoint, Vector4(_mm_loadu_ps(zbufferP)));
                        // mask |= _mm_movemask_ps(passTest)&endXMask;
                        if (passTest.getFlags() & endXMask)
                        {
                            // NAU_CORE_DEBUG_LF("end %dx%d mas=%d&endXMask = %x", x, y, _mm_movemask_ps(passTest), endXMask);
                            return 1;
                        }
                        // if (mask)
                        //   return 1;
                    }
                }
                else
                    for (int y = yMin; y <= yMax; ++y, zbufferRow += pitch)
                    {
                        const float* zbufferP = (float*)zbufferRow;
                        // int mask = 0;
                        for (int x = xMin; x < xEnd4; x += 4, zbufferP += 4)
                            if ((occlusion_depth_vcmp(closestPoint, Vector4(_mm_loadu_ps(zbufferP)))).getFlags())
                                return 1;
                    }
            }
            /*/
            //this one works slower (generally)
            zbufferRow = zbuffer + pitch*yMin;
            const int xMin4 = (xMin+3)&~3, xEnd4 = (xMax+1)&(~3);
            const uint32_t startXMask = 0xF&~( (1<<(4-(xMin4-xMin))) - 1);
            const uint32_t endXMask = xMax>=xEnd4 ? ((1<<((xMax-xEnd4+1)))-1) : 0;

            zbufferRow += (xMin&(~3));
            //NAU_CORE_DEBUG_LF("%dx%d %dx%d %f", xMin,yMin, xMax, yMax, v_extract_x(minw));
            for (int y = yMin; y <= yMax; ++y, zbufferRow+=pitch)
            {
              const Vector4 *zbuffer = (Vector4*)zbufferRow;
              if (startXMask)
              {
                Vector4 passTest = occlusion_depth_vcmp(closestPoint, *zbuffer);
                if (_mm_movemask_ps(passTest)&startXMask)
                {
                  //NAU_CORE_DEBUG_LF("entry: %dx%d 0x%X &0x%X mask (%f %f %f %f)", xMin&(~3), y, _mm_movemask_ps(passTest), startXMask, V4D(*zbuffer));
                  return 1;
                }
                zbuffer++;
              }
              for (int x = xMin4; x < xEnd4; x+=4, zbuffer++)
              {
                Vector4 passTest = occlusion_depth_vcmp(closestPoint, *zbuffer);
                if (_mm_movemask_ps(passTest))
                {
                  //NAU_CORE_DEBUG_LF("entry: %dx%d 0x%X mask (%f %f %f %f)", x, y, _mm_movemask_ps(passTest), V4D(*zbuffer));
                  return 1;
                }
              }
              if (endXMask)
              {
                Vector4 passTest = occlusion_depth_vcmp(closestPoint, *zbuffer);
                if (_mm_movemask_ps(passTest)&endXMask)
                {
                  //NAU_CORE_DEBUG_LF("trail: %dx%d 0x%X &0x%X mask (%f %f %f %f)", xMax, y, _mm_movemask_ps(passTest), endXMask, V4D(*zbuffer));
                  return 1;
                }
              }
            }
            //*/
            // NAU_CORE_DEBUG_LF(deb.str());
            // NAU_CORE_DEBUG_LF("%dx%d %dx%d %f xMin4=%d, xEnd4=%d, smask=0x%X emask=0x%X",
            //   xMin,yMin, xMax, yMax, v_extract_x(minw), xMin4, xEnd4, startXMask, endXMask);
            return 0;
        }
    };

    // default sizes
    enum
    {
        OCCLUSION_W = 256,
        OCCLUSION_H = 128
    };

    template <int sizeX, int sizeY>
    alignas(32) float OcclusionTest<sizeX, sizeY>::zBuffer[] = {};

    template <int sizeX, int sizeY>
    Vector4 OcclusionTest<sizeX, sizeY>::clipToScreenVec = {};

    template <int sizeX, int sizeY>
    Vector4 OcclusionTest<sizeX, sizeY>::screenMax = {};

    template <int sizeX, int sizeY>
    int OcclusionTest<sizeX, sizeY>::mip_chain_offsets[] = {};
}  // namespace nau::math
