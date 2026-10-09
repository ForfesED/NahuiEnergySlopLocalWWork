// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/utils/hasg.h


#pragma once

namespace nau::strings
{
    constexpr inline size_t constHash(const char* input)
    {
        size_t hash = sizeof(size_t) == 8 ? 0xcbf29ce484222325 : 0x811c9dc5;
        const size_t prime = sizeof(size_t) == 8 ? 0x00000100000001b3 : 0x01000193;

        while(*input)
        {
            hash ^= static_cast<size_t>(*input);
            hash *= prime;
            ++input;
        }

        return hash;
    }
}  // namespace nau::strings
