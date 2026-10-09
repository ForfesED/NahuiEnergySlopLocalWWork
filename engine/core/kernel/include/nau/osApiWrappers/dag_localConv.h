// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <stdarg.h>

#include "nau/kernel/kernel_config.h"

namespace nau::hal
{
#ifdef __cplusplus
    extern "C"
    {
#endif

        // Initialize tables for operate on symbols of current DOS code page
        NAU_KERNEL_EXPORT void dd_init_local_conv(void);

        // Convert all letters to upper-case
        NAU_KERNEL_EXPORT char* dd_strupr(char* s);

        // Convert all letters to lower-case
        NAU_KERNEL_EXPORT char* dd_strlwr(char* s);

        NAU_KERNEL_EXPORT char dd_charupr(unsigned char c);

        NAU_KERNEL_EXPORT char dd_charlwr(unsigned char c);

        // Compare strings (case-insensitive)
        // stricmp() analogue
        NAU_KERNEL_EXPORT int dd_stricmp(const char* a, const char* b);

        // Compare no more than n first symbols of strings (case-insensitive)
        //  strnicmp() analogue
        NAU_KERNEL_EXPORT int dd_strnicmp(const char* a, const char* b, int n);

        // Compare n symbols (case-insensitive)
        // memicmp() analogue
        NAU_KERNEL_EXPORT int dd_memicmp(const char* a, const char* b, int n);

        //! returns pointer to internal uptab[256]
        extern NAU_KERNEL_EXPORT const unsigned char* dd_local_cmp_uptab;
        //! returns pointer to internal lwtab[256]
        extern NAU_KERNEL_EXPORT const unsigned char* dd_local_cmp_lwtab;

#ifdef __cplusplus
    }
#endif
}  // namespace nau::hal