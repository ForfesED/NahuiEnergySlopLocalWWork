// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "tinyimageformat.h"

namespace nau
{
    /*
    */
    class TextureCompressor
    {
        public:
            enum CompressionType
            {
                COMPRESSION_NONE,
                COMPRESSION_ASTC,
                COMPRESSION_BC,
            };

            TextureCompressor(TinyImageFormat format, CompressionType compressionType = CompressionType::COMPRESSION_BC);
            ~TextureCompressor() = default;
            unsigned char* compress(unsigned char* data, unsigned width, unsigned height);

            static TinyImageFormat getOutputTextureFormat(TinyImageFormat format, CompressionType compressionType = CompressionType::COMPRESSION_BC);

        private:
            CompressionType m_compressionType;
            TinyImageFormat m_sourceFormat = TinyImageFormat_UNDEFINED;
    };
}  // namespace nau
