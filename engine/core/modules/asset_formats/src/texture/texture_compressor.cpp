// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "texture_compressor.h"

// TODO: replace malloc/realloc/free with our allocator
#define STBI_MALLOC(sz) malloc(sz)
#define STBI_REALLOC(p, newsz) realloc(p, newsz)
#define STBI_FREE(p) free(p)

// ISPC texcomp
#include "ispc_texcomp.h"
#include "nau/diag/assertion.h"

namespace nau
{
    namespace
    {
        enum DXT
        {
            DXT_NONE,
            DXT_BC1,  // RGB + 1A
            DXT_BC3,  // RGB + 8A
            DXT_BC4,  // Single Channel
            DXT_BC5,  // Two channels
            DXT_BC6,  // RGB HalfFloats
            DXT_BC7,
        };

        DXT getDXTCompression(TinyImageFormat format)
        {
            const uint32_t channels = TinyImageFormat_ChannelCount(format);
            const bool isFloat = TinyImageFormat_IsFloat(format);
            DXT dxtCompression = DXT_NONE;

            if (isFloat)
            {
                NAU_ASSERT(false);  // TODO: NAU-1797 Support BC compression for float textures (BC6 / BC7)
            }
            else if (channels == 1)
            {
                dxtCompression = DXT_BC4;
            }
            else if (channels == 2)
            {
                dxtCompression = DXT_BC5;
            }
            else if (channels == 3)
            {
                dxtCompression = DXT_BC1;
            }
            else if (channels == 4)
            {
                dxtCompression = DXT_BC3;
            }

            return dxtCompression;
        }

        TinyImageFormat getBCFormat(TinyImageFormat format)
        {
            const bool isSigned = TinyImageFormat_IsSigned(format);
            const uint32_t channels = TinyImageFormat_ChannelCount(format);

            TinyImageFormat outFormat = TinyImageFormat_UNDEFINED;
            switch (getDXTCompression(format))
            {
                case DXT_BC1:
                    outFormat = channels < 4 ? TinyImageFormat_DXBC1_RGB_UNORM : TinyImageFormat_DXBC1_RGBA_UNORM;
                    break;
                case DXT_BC3:
                    outFormat = TinyImageFormat_DXBC3_UNORM;
                    break;
                case DXT_BC4:
                    outFormat = isSigned ? TinyImageFormat_DXBC4_SNORM : TinyImageFormat_DXBC4_UNORM;
                    break;
                case DXT_BC5:
                    outFormat = isSigned ? TinyImageFormat_DXBC5_SNORM : TinyImageFormat_DXBC5_UNORM;
                    break;
                case DXT_BC6:
                    outFormat = isSigned ? TinyImageFormat_DXBC6H_SFLOAT : TinyImageFormat_DXBC6H_UFLOAT;
                    break;
                case DXT_BC7:
                    outFormat = TinyImageFormat_DXBC7_UNORM;
                    break;
                default:
                    NAU_ASSERT("Uknown DXT compression format");
                    break;
            }

            return outFormat;
        }
    }  // namespace

    unsigned char* ASTCCompression(unsigned char* data, TinyImageFormat format, unsigned width, unsigned height)
    {
        NAU_ASSERT(data);
        NAU_ASSERT(width && height);  // widht/height cannot be 0

        const uint32_t blockSizeX = 4;
        const uint32_t blockSizeY = 4;
        const uint32_t channels = TinyImageFormat_ChannelCount(format);
        NAU_ASSERT(channels >= 3);  // ISPC astc compression requires atleast 3 channels

        if (TinyImageFormat_BitSizeOfBlock(format) != 32)
        {
            NAU_ASSERT("Fast ISPC Texture Compressor only supports 32bits per pixel for ASTC");
            return nullptr;
        }

        // Get astc encoder settings
        astc_enc_settings astcEncSettings = {};
        if (channels > 3)
        {
            GetProfile_astc_alpha_fast(&astcEncSettings, blockSizeX, blockSizeY);
        }
        else
        {
            GetProfile_astc_fast(&astcEncSettings, blockSizeX, blockSizeY);
        }

        rgba_surface input;
        input.width = width;
        input.height = height;
        input.stride = width * channels;
        input.ptr = data;
        unsigned char* result = (unsigned char*)STBI_MALLOC(width * height * channels);
        CompressBlocksASTC(&input, result, &astcEncSettings);

        NAU_ASSERT(result);
        return result;
    }

    typedef void (*BCCompressionFunc)(const rgba_surface* src, uint8_t* dst);

#define DECLARE_COMPRESS_FUNCTION_BC6H(profile)                              \
    void CompressBlocksBC6H_##profile(const rgba_surface* src, uint8_t* dst) \
    {                                                                        \
        bc6h_enc_settings settings;                                          \
        GetProfile_bc6h_##profile(&settings);                                \
        CompressBlocksBC6H(src, dst, &settings);                             \
    }

    DECLARE_COMPRESS_FUNCTION_BC6H(veryfast);
    DECLARE_COMPRESS_FUNCTION_BC6H(fast);
    DECLARE_COMPRESS_FUNCTION_BC6H(basic);
    DECLARE_COMPRESS_FUNCTION_BC6H(slow);
    DECLARE_COMPRESS_FUNCTION_BC6H(veryslow);

#define DECLARE_COMPRESS_FUNCTION_BC7(profile)                              \
    void CompressBlocksBC7_##profile(const rgba_surface* src, uint8_t* dst) \
    {                                                                       \
        bc7_enc_settings settings;                                          \
        GetProfile_##profile(&settings);                                    \
        CompressBlocksBC7(src, dst, &settings);                             \
    }

    DECLARE_COMPRESS_FUNCTION_BC7(ultrafast);
    DECLARE_COMPRESS_FUNCTION_BC7(veryfast);
    DECLARE_COMPRESS_FUNCTION_BC7(fast);
    DECLARE_COMPRESS_FUNCTION_BC7(basic);
    DECLARE_COMPRESS_FUNCTION_BC7(slow);
    DECLARE_COMPRESS_FUNCTION_BC7(alpha_ultrafast);
    DECLARE_COMPRESS_FUNCTION_BC7(alpha_veryfast);
    DECLARE_COMPRESS_FUNCTION_BC7(alpha_fast);
    DECLARE_COMPRESS_FUNCTION_BC7(alpha_basic);
    DECLARE_COMPRESS_FUNCTION_BC7(alpha_slow);

    unsigned char* BCCompression(unsigned char* data, TinyImageFormat format, unsigned width, unsigned height)
    {
        NAU_ASSERT(data);
        NAU_ASSERT(width && height);  // width/height cannot be 0

        const uint32_t blockSize = 4;
        BCCompressionFunc bcCompress = CompressBlocksBC7_alpha_fast;
        uint32_t bytesPerBlock = 16;

        uint32_t inputChannels = TinyImageFormat_ChannelCount(format);
        uint32_t requiredInputChannels = 4;
        uint32_t bitsPerPixel = TinyImageFormat_BitSizeOfBlock(format);

        //-LDR input is 32 bit / pixel(sRGB), HDR is 64 bit / pixel(half float)
        //	- for BC4 input is 8bit / pixel(R8), for BC5 input is 16bit / pixel(RG8)
        //	- dst buffer must be allocated with enough space for the compressed texture

        switch (getDXTCompression(format))
        {
            case DXT_BC1:
                bcCompress = CompressBlocksBC1;
                bytesPerBlock = 8;
                requiredInputChannels = 3;
                break;
            case DXT_BC3:
                bcCompress = CompressBlocksBC3;
                break;
            case DXT_BC4:
                bcCompress = CompressBlocksBC4;
                bytesPerBlock = 8;
                requiredInputChannels = 1;
                break;
            case DXT_BC5:
                bcCompress = CompressBlocksBC5;
                requiredInputChannels = 2;
                break;
            case DXT_BC6:
                bcCompress = CompressBlocksBC6H_fast;
                requiredInputChannels = 4;
                if (bitsPerPixel != 64 || !TinyImageFormat_IsFloat(format))
                {
                    NAU_ASSERT("Unsupported format for BC6 compression");
                    return nullptr;
                }
                break;
            case DXT_BC7:
                bcCompress = inputChannels > 3 ? CompressBlocksBC7_alpha_fast : CompressBlocksBC7_fast;
                break;
            default:
                NAU_ASSERT(false && "Unknown BC compression request");
        }
        NAU_ASSERT(requiredInputChannels <= inputChannels && "Input should always have more data available");

        rgba_surface input;
        input.ptr = data;
        input.stride = width * requiredInputChannels;
        input.width = width;
        input.height = height;

        uint8_t* const resultStorage = EXPR_Block
        {
            const unsigned nbw = std::max<unsigned>(1u, (unsigned(width) + 3u) / 4u);
            const unsigned nbh = std::max<unsigned>(1u, (unsigned(height) + 3u) / 4u);
            const unsigned rowPitch = nbw * bytesPerBlock;
            const unsigned slicePitch = rowPitch * nbh;

            return reinterpret_cast<uint8_t*>(STBI_MALLOC(slicePitch));
        };
        NAU_FATAL(resultStorage);

        bcCompress(&input, resultStorage);
        return resultStorage;
    }

    TextureCompressor::TextureCompressor(TinyImageFormat format, CompressionType compressionType) :
        m_compressionType(compressionType),
        m_sourceFormat(format)
    {
    }

    unsigned char* TextureCompressor::compress(unsigned char* data, unsigned width, unsigned height)
    {
        NAU_ASSERT(data);
        switch (m_compressionType)
        {
            case COMPRESSION_ASTC:
                return ASTCCompression(data, m_sourceFormat, width, height);
            case COMPRESSION_BC:
                return BCCompression(data, m_sourceFormat, width, height);
            default:
                NAU_ASSERT("Unknown compression type");
                return nullptr;
        }

        return nullptr;
    }

    TinyImageFormat TextureCompressor::getOutputTextureFormat(TinyImageFormat format, nau::TextureCompressor::CompressionType compressionType)
    {
        TinyImageFormat outFormat = TinyImageFormat_UNDEFINED;
        switch (compressionType)
        {
            case nau::TextureCompressor::CompressionType::COMPRESSION_ASTC:
                outFormat = TinyImageFormat_ASTC_4x4_UNORM;
                break;
            case nau::TextureCompressor::CompressionType::COMPRESSION_BC:
                outFormat = getBCFormat(format);
                break;
            default:
                outFormat = format;
                break;
        }

        return outFormat;
    }
}  // namespace nau
