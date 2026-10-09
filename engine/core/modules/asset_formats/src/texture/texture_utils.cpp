// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "./texture_utils.h"

namespace nau
{
    std::tuple<unsigned, unsigned> TextureUtils::getMipSize(unsigned width, unsigned height, uint32_t level)
    {
        const auto w = std::max(width >> level, 1u);
        const auto h = std::max(height >> level, 1u);
        return {w, h};
    }

    unsigned TextureUtils::roundToPowOf2(unsigned value)
    {
        // NOTE: only works if v is 32 bit
        value--;
        value |= value >> 1;
        value |= value >> 2;
        value |= value >> 4;
        value |= value >> 8;
        value |= value >> 16;
        value++;
        return value;
    }

    Result<std::tuple<uint64_t, uint64_t>> TextureUtils::getImagePitch(TinyImageFormat fmt, size_t width, size_t height)
    {
        // for more formats see:
        // https://github.com/microsoft/DirectXTex/blob/main/DirectXTex/DirectXTexUtil.cp
        uint64_t pitch = 0;
        uint64_t slice = 0;

        switch(fmt)
        {
            case TinyImageFormat_DXBC1_RGB_UNORM:
            case TinyImageFormat_DXBC1_RGB_SRGB:
            case TinyImageFormat_DXBC1_RGBA_UNORM:
            case TinyImageFormat_DXBC1_RGBA_SRGB:
            case TinyImageFormat_DXBC4_UNORM:
            case TinyImageFormat_DXBC4_SNORM:
                {
                    NAU_ASSERT(TinyImageFormat_IsCompressed(fmt));

                    uint64_t nbw = std::max<uint64_t>(1u, (uint64_t(width) + 3u) / 4u);
                    uint64_t nbh = std::max<uint64_t>(1u, (uint64_t(height) + 3u) / 4u);
                    pitch = nbw * 8u;
                    slice = pitch * nbh;

                    break;
                }

            case TinyImageFormat_DXBC2_UNORM:
            case TinyImageFormat_DXBC2_SRGB:
            case TinyImageFormat_DXBC3_UNORM:
            case TinyImageFormat_DXBC3_SRGB:
            case TinyImageFormat_DXBC5_UNORM:
            case TinyImageFormat_DXBC5_SNORM:
            case TinyImageFormat_DXBC6H_UFLOAT:
            case TinyImageFormat_DXBC6H_SFLOAT:
            case TinyImageFormat_DXBC7_UNORM:
            case TinyImageFormat_DXBC7_SRGB:
                {
                    NAU_ASSERT(TinyImageFormat_IsCompressed(fmt));

                    uint64_t nbw = std::max<uint64_t>(1u, (uint64_t(width) + 3u) / 4u);
                    uint64_t nbh = std::max<uint64_t>(1u, (uint64_t(height) + 3u) / 4u);
                    pitch = nbw * 16u;
                    slice = pitch * nbh;

                    break;
                }
            default:
                {
                    NAU_ASSERT(!TinyImageFormat_IsCompressed(fmt));

                    const auto bitsPerPixel = TinyImageFormat_BitSizeOfBlock(fmt);

                    pitch = (uint64_t(width) * bitsPerPixel + 7u) / 8u;
                    slice = pitch * uint64_t(height);
                }
        }

        return {pitch, slice};
    }

    void TextureUtils::copyImageData(DestTextureData& dest, unsigned srcWidth, unsigned srcHeight, TinyImageFormat srcFormat, const std::byte* srcBuffer)
    {
        const auto [srcRowPitch, srcSlicePitch] = *TextureUtils::getImagePitch(srcFormat, srcWidth, srcHeight);
        const auto dstRowBytesSize = dest.rowBytesSize == 0 ? srcRowPitch : dest.rowBytesSize;

        NAU_FATAL(dstRowBytesSize <= srcRowPitch);
        NAU_FATAL(dstRowBytesSize <= dest.rowPitch);
        NAU_FATAL(dest.rowPitch > 0);


        NAU_ASSERT(dest.outputBuffer != nullptr);

        if(srcRowPitch == dest.rowPitch)
        {
            const size_t bufferSize = dest.rowsCount * dest.rowPitch;
            memcpy(dest.outputBuffer, srcBuffer, bufferSize);
            return;
        }

        for(size_t y = 0; y < dest.rowsCount; ++y)
        {
            const void* const src = srcBuffer + y * srcRowPitch;
            void* const dst = reinterpret_cast<std::byte*>(dest.outputBuffer) + y * dest.rowPitch;

            memcpy(dst, src, dstRowBytesSize);
        }
    }
}  // namespace nau
