// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include "nau/math/math.h"

template <typename V, typename T = typename V::value_type>
inline uint32_t data_size(const V& v)
{
    return v.size() * (uint32_t)sizeof(T);
}

template <class V, typename U = typename V::value_type>
inline void mem_set_0(V& v)
{
    memset(v.data(), 0, data_size(v));
}

template <class V, typename U = typename V::value_type>
inline void mem_set_ff(V& v)
{
    memset(v.data(), 0xFF, data_size(v));
}

static inline bool is_viewed_small(const float posRadius, const float distance_2, float markSmallLightsAsFarLimit)
{
    float view_2 = posRadius;
    view_2 = view_2 * view_2 / distance_2;  // good approximation for small values
    return view_2 < markSmallLightsAsFarLimit * markSmallLightsAsFarLimit;
}
