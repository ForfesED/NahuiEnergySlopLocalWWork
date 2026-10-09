// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <tinyimageformat.h>

#include <tuple>

#include "nau/assets/texture_asset_accessor.h"
#include "nau/utils/result.h"

namespace nau
{
    /**
     */
    struct TextureUtils
    {
        /**
        */
        static std::tuple<unsigned, unsigned> getMipSize(unsigned width, unsigned height, uint32_t level);

        /**
         */
        static unsigned roundToPowOf2(unsigned size);

        /**
         */
        static Result<std::tuple<uint64_t, uint64_t>> getImagePitch(TinyImageFormat fmt, size_t width, size_t height);

        /**
         */
        static void copyImageData(DestTextureData& dest, unsigned srcWidth, unsigned srcHeight, TinyImageFormat srcFormat, const std::byte* srcBuffer);
    };
}  // namespace nau
