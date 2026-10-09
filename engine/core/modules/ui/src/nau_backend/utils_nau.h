// Copyright 2024 N-GINN LLC. All rights reserved.
/****************************************************************************
 Copyright (c) 2018-2019 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/


#pragma once

#include "base/ccMacros.h"
#include "nau/3d/dag_drv3d.h"
#include "renderer/backend/Types.h"

DAGOR_CC_BACKEND_BEGIN

/**
 * Convert backend enum class to corresponding opengl defined value.
 */
namespace cocos_utils
{

    static inline VSDT toNauAttributeFormat(cocos2d::backend::VertexFormat vertexFormat, bool needToBeNormallized)
    {
        using namespace cocos2d::backend;
        VSDT ret = VSDT_FLOAT1;
        switch (vertexFormat)
        {
            case VertexFormat::FLOAT4:
                ret = VSDT_FLOAT4;
                break;
            case VertexFormat::INT4:
                ret = VSDT_INT4;
                break;
            case VertexFormat::UBYTE4:
                if (needToBeNormallized)
                {
                    ret = VSDT_E3DCOLOR;
                }
                else
                {
                    ret = VSDT_UBYTE4;
                }
                break;
            case VertexFormat::FLOAT3:
                ret = VSDT_FLOAT3;
                break;
            case VertexFormat::INT3:
                ret = VSDT_INT3;
                break;
            case VertexFormat::FLOAT2:
                ret = VSDT_FLOAT2;
                break;
            case VertexFormat::INT2:
                ret = VSDT_INT2;
                break;
            case VertexFormat::FLOAT:
                ret = VSDT_FLOAT1;
                break;
            case VertexFormat::INT:
                ret = VSDT_INT1;
                break;
            default:
                break;
        }
        return ret;
    }

    /**
     * Convert magnification filter to d3d::MipMapMode. i.e. convert SamplerFilter::LINEAR to d3d::MipMapMode::Linear.
     * @param magFilter Specifies the magnification filter to convert.
     * @return Magnification filter.
     */
    static inline d3d::MipMapMode toNauMipMapMode(cocos2d::backend::SamplerFilter minFilter)
    {
        using namespace cocos2d::backend;
        switch (minFilter)
        {
            case SamplerFilter::NEAREST:
            case SamplerFilter::NEAREST_MIPMAP_NEAREST:
            case SamplerFilter::LINEAR_MIPMAP_NEAREST:
                return d3d::MipMapMode::Point;
            case SamplerFilter::LINEAR:
            case SamplerFilter::LINEAR_MIPMAP_LINEAR:
            case SamplerFilter::NEAREST_MIPMAP_LINEAR:
                return d3d::MipMapMode::Linear;
            default:
                break;
        }
        return d3d::MipMapMode::Point;
    };
    /**
     * Convert minifying filter to d3d::FilterMode. i.e. convert SamplerFilter::LINEAR to d3d::FilterMode::Linear.
     * If mipmaps is enabled and texture width and height are not power of two, then if minFilter is SamplerFilter::LINEAR, d3d::FilterMode::Linear is returned, otherwise return d3d::FilterMode::Point.
     * @param minFilter Specifies minifying filter.
     * @return Minifying filter
     */
    static inline d3d::FilterMode toNauFilter(cocos2d::backend::SamplerFilter minFilter)
    {
        using namespace cocos2d::backend;
        switch (minFilter)
        {
            case SamplerFilter::LINEAR:
            case SamplerFilter::LINEAR_MIPMAP_LINEAR:
            case SamplerFilter::LINEAR_MIPMAP_NEAREST:
                return d3d::FilterMode::Linear;
            case SamplerFilter::NEAREST:
            case SamplerFilter::NEAREST_MIPMAP_NEAREST:
            case SamplerFilter::NEAREST_MIPMAP_LINEAR:
                return d3d::FilterMode::Point;
            default:
                break;
        }

        return d3d::FilterMode::Point;
    };

    static inline d3d::AddressMode toNauAddressMode(cocos2d::backend::SamplerAddressMode addressMode)
    {
        using namespace cocos2d::backend;
        switch (addressMode)
        {
            case SamplerAddressMode::REPEAT:
                return d3d::AddressMode::Wrap;
                break;
            case SamplerAddressMode::MIRROR_REPEAT:
                return d3d::AddressMode::Mirror;
                break;
            case SamplerAddressMode::CLAMP_TO_EDGE:
                return d3d::AddressMode::Clamp;
                break;
            default:
                break;
        }
        return d3d::AddressMode::Wrap;
    }

    /**
     * Get textrue parameters from texture pixle format.
     * @param in textureFormat Specifies texture pixel format.
     * @return out TEXFMT Specifies the internal format of the texture.
     */
    static inline TEXFMT toNauTypes(cocos2d::backend::PixelFormat textureFormat)
    {
        using namespace cocos2d::backend;
        switch (textureFormat)
        {
            case PixelFormat::RGBA8888:
                return TEXFMT::TEXFMT_R8G8B8A8;
                break;

            case PixelFormat::RGB888:
                return TEXFMT::TEXFMT_R8G8B8A8;
                break;

            case PixelFormat::RGBA4444:
                return TEXFMT::TEXFMT_A4R4G4B4;
                break;

            case PixelFormat::A8:
                return TEXFMT::TEXFMT_A8;
                break;

            case PixelFormat::I8:
                return TEXFMT::TEXFMT_R8;
                break;

            case PixelFormat::AI88:
                return TEXFMT::TEXFMT_R8G8;
                break;

            case PixelFormat::RGB565:
                return TEXFMT::TEXFMT_R5G6B5;
                break;

            case PixelFormat::RGB5A1:
                return TEXFMT::TEXFMT_A1R5G5B5;
                break;

            case PixelFormat::D24S8:
                return TEXFMT::TEXFMT_DEPTH24;
                break;

            case PixelFormat::ETC:
            case PixelFormat::ATC_RGB:
            case PixelFormat::ATC_EXPLICIT_ALPHA:
            case PixelFormat::ATC_INTERPOLATED_ALPHA:
            case PixelFormat::PVRTC2:
            case PixelFormat::PVRTC2A:
            case PixelFormat::PVRTC4:
            case PixelFormat::PVRTC4A:
            case PixelFormat::S3TC_DXT1:
            case PixelFormat::S3TC_DXT3:
            case PixelFormat::S3TC_DXT5:
                NAU_FAILURE_ALWAYS("Compressed formats are unsupported.");
                break;
            default:
                break;
        }
        return TEXFMT::TEXFMT_A8R8G8B8;
        //NAU_FAILURE_ALWAYS("Current format is unsupported.");
        //return TEXFMT(0);
    }

    /**
     * Convert compare function to D3D_CMPF. i.e. convert CompareFunction::NEVER to CMPF_NEVER.
     * @param compareFunction Specifies the compare function to convert.
     * @return Compare function.
     */
    static inline D3D_CMPF toNauCompareFunction(cocos2d::backend::CompareFunction compareFunction)
    {
        using namespace cocos2d::backend;
        D3D_CMPF ret = CMPF_ALWAYS;
        switch (compareFunction)
        {
            case CompareFunction::NEVER:
                ret = CMPF_NEVER;
                break;
            case CompareFunction::LESS:
                ret = CMPF_LESS;
                break;
            case CompareFunction::LESS_EQUAL:
                ret = CMPF_LESSEQUAL;
                break;
            case CompareFunction::GREATER:
                ret = CMPF_GREATER;
                break;
            case CompareFunction::GREATER_EQUAL:
                ret = CMPF_GREATEREQUAL;
                break;
            case CompareFunction::NOT_EQUAL:
                ret = CMPF_NOTEQUAL;
                break;
            case CompareFunction::EQUAL:
                ret = CMPF_EQUAL;
                break;
            case CompareFunction::ALWAYS:
                ret = CMPF_ALWAYS;
                break;
            default:
                break;
        }
        return ret;
    }

    /**
     * Convert stencil operation to D3D_STNCLOP. i.e. convert StencilOperation::KEEP to STNCLOP_KEEP.
     * @param stencilOperation Specifies stencil operation.
     * @return Stencil operation.
     */
    static inline D3D_STNCLOP toNauStencilOperation(cocos2d::backend::StencilOperation stencilOperation)
    {
        using namespace cocos2d::backend;
        D3D_STNCLOP ret = STNCLOP_KEEP;
        switch (stencilOperation)
        {
            case StencilOperation::KEEP:
                ret = STNCLOP_KEEP;
                break;
            case StencilOperation::ZERO:
                ret = STNCLOP_ZERO;
                break;
            case StencilOperation::REPLACE:
                ret = STNCLOP_REPLACE;
                break;
            case StencilOperation::INVERT:
                ret = STNCLOP_INVERT;
                break;
            case StencilOperation::INCREMENT_WRAP:
                ret = STNCLOP_INCR;
                break;
            case StencilOperation::DECREMENT_WRAP:
                ret = STNCLOP_DECR;
                break;
            default:
                break;
        }
        return ret;
    }

    /**
     * Convert blend operation to D3D_BLENDOP. i.e. convert BlendOperation::ADD to BLENDOP_ADD.
     * @param blendOperation Specifies blend function to convert.
     * @return Blend operation.
     */
    static inline D3D_BLENDOP toNauBlendOperation(cocos2d::backend::BlendOperation blendOperation)
    {
        using namespace cocos2d::backend;
        D3D_BLENDOP ret = BLENDOP_ADD;
        switch (blendOperation)
        {
            case BlendOperation::ADD:
                ret = BLENDOP_ADD;
                break;
            case BlendOperation::SUBTRACT:
                ret = BLENDOP_SUBTRACT;
                break;
            case BlendOperation::RESERVE_SUBTRACT:
                ret = BLENDOP_REVSUBTRACT;
                break;
            default:
                break;
        }
        return ret;
    }

    /**
     * Convert blend factor to D3D_BLEND. i.e. convert BlendFactor::ZERO to BLEND_ZERO.
     * @param blendFactor Specifies the blend factor to convert.
     * @return Blend factor.
     */
    static inline D3D_BLEND toNauBlendFactor(cocos2d::backend::BlendFactor blendFactor)
    {
        using namespace cocos2d::backend;
        D3D_BLEND ret = BLEND_ONE;
        switch (blendFactor)
        {
            case BlendFactor::ZERO:
                ret = BLEND_ZERO;
                break;
            case BlendFactor::ONE:
                ret = BLEND_ONE;
                break;
            case BlendFactor::SRC_COLOR:
                ret = BLEND_SRCCOLOR;
                break;
            case BlendFactor::ONE_MINUS_SRC_COLOR:
                ret = BLEND_INVSRCCOLOR;
                break;
            case BlendFactor::SRC_ALPHA:
                ret = BLEND_SRCALPHA;
                break;
            case BlendFactor::ONE_MINUS_SRC_ALPHA:
                ret = BLEND_INVSRCALPHA;
                break;
            case BlendFactor::DST_COLOR:
                ret = BLEND_DESTCOLOR;
                break;
            case BlendFactor::ONE_MINUS_DST_COLOR:
                ret = BLEND_INVDESTCOLOR;
                break;
            case BlendFactor::DST_ALPHA:
                ret = BLEND_DESTALPHA;
                break;
            case BlendFactor::ONE_MINUS_DST_ALPHA:
                ret = BLEND_INVDESTALPHA;
                break;
            case BlendFactor::SRC_ALPHA_SATURATE:
                ret = BLEND_SRCALPHASAT;
                break;
            case BlendFactor::BLEND_CLOLOR:
                ret = BLEND_BLENDFACTOR;
                break;
            default:
                break;
        }
        return ret;
    }

    static inline D3D_CULL toNauCullMode(cocos2d::backend::CullMode cullMode)
    {
        using namespace cocos2d::backend;
        switch (cullMode)
        {
            case cocos2d::backend::CullMode::BACK:
                return CULL_CW;
            case cocos2d::backend::CullMode::FRONT:
                return CULL_CCW;
            case cocos2d::backend::CullMode::NONE:
                return CULL_NONE;
        }
        return CULL_CCW;
    }

    /**
     * Convert primitive type to PRIM. i.e. convert PrimitiveType::TRIANGLE to PRIM_TRILIST.
     * @param primitiveType Specifies the kind of primitives to convert.
     * @return Primitive type.
     */
    static inline PRIM toNauPrimitiveType(cocos2d::backend::PrimitiveType primitiveType)
    {
        using namespace cocos2d::backend;
        PRIM ret = PRIM_TRILIST;
        switch (primitiveType)
        {
            case PrimitiveType::POINT:
                ret = PRIM_POINTLIST;
                break;
            case PrimitiveType::LINE:
                ret = PRIM_LINELIST;
                break;
            case PrimitiveType::LINE_STRIP:
                ret = PRIM_LINESTRIP;
                break;
            case PrimitiveType::TRIANGLE:
                ret = PRIM_TRILIST;
                break;
            case PrimitiveType::TRIANGLE_STRIP:
                ret = PRIM_TRISTRIP;
                break;
            default:
                break;
        }
        return ret;
    }

    static inline int toNauPrimitiveCountFromVertexCount(std::size_t vertexCount, cocos2d::backend::PrimitiveType primitiveType)
    {
        using namespace cocos2d::backend;
        int ret = vertexCount;
        switch (primitiveType)
        {
            case PrimitiveType::POINT:
                ret = vertexCount;
                break;
            case PrimitiveType::LINE:
                ret = vertexCount / 2;
                break;
            case PrimitiveType::LINE_STRIP:
                ret = vertexCount - 1;
                break;
            case PrimitiveType::TRIANGLE:
                ret = vertexCount / 3;
                break;
            case PrimitiveType::TRIANGLE_STRIP:
                ret = vertexCount - 2;
                break;
            default:
                break;
        }
        return ret;
    }

    static inline uint32_t toNauWriteMask(cocos2d::backend::ColorWriteMask writeMask)
    {
        uint32_t ret = 0;
        switch (writeMask)
        {
            case cocos2d::backend::ColorWriteMask::NONE:
                ret = 0x00000000;
                break;
            case cocos2d::backend::ColorWriteMask::RED:
                ret = 0xFF000000;
                break;
            case cocos2d::backend::ColorWriteMask::GREEN:
                ret = 0x00FF0000;
                break;
            case cocos2d::backend::ColorWriteMask::BLUE:
                ret = 0x0000FF00;
                break;
            case cocos2d::backend::ColorWriteMask::ALPHA:
                ret = 0x000000FF;
                break;
            case cocos2d::backend::ColorWriteMask::ALL:
                ret = 0xFFFFFFFF;
                break;
        }
        return ret;
    }
};  // namespace cocos_utils

DAGOR_CC_BACKEND_END