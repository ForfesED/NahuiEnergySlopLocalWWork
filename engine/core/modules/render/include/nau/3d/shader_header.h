// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

// some headers define DOMAIN as macro which collides with enum names
#if defined(DOMAIN)
    #undef DOMAIN
#endif

namespace dxil
{
    // DX12 Baseline limit
    // 14 is the max until Tier 3 HW
    constexpr uint32_t MAX_B_REGISTERS = 14;
    constexpr uint32_t MAX_T_REGISTERS = 32;
    constexpr uint32_t MAX_S_REGISTERS = MAX_T_REGISTERS;
    constexpr uint32_t MAX_U_REGISTERS = 13;

    enum class ShaderStage : uint32_t
    {
        VERTEX,
        PIXEL,
        GEOMETRY,
        DOMAIN,
        HULL,
        COMPUTE,
        MESH,
        AMPLIFICATION,
    };



    struct ShaderResourceUsageTable
    {
        uint32_t tRegisterUseMask = 0;
        uint32_t sRegisterUseMask = 0;
        uint32_t bindlessUsageMask = 0;
        uint16_t bRegisterUseMask = 0;
        uint16_t uRegisterUseMask = 0;
        uint8_t rootConstantDwords = 0;
        uint8_t specialConstantsMask = 0;
        uint16_t _resv = 0;
    };


    inline bool operator==(const ShaderResourceUsageTable &l, const ShaderResourceUsageTable &r)
    {
        return l.tRegisterUseMask == r.tRegisterUseMask && l.sRegisterUseMask == r.sRegisterUseMask &&
               l.bindlessUsageMask == r.bindlessUsageMask && l.bRegisterUseMask == r.bRegisterUseMask &&
               l.rootConstantDwords == r.rootConstantDwords && l.uRegisterUseMask == r.uRegisterUseMask &&
               l.specialConstantsMask == r.specialConstantsMask;
    }

    inline bool operator!=(const ShaderResourceUsageTable &l, const ShaderResourceUsageTable &r) { return !(l == r); }


    struct ShaderHeader
    {
        uint16_t shaderType;
        // input primitive for tesselation stages
        uint16_t inputPrimitive;
        uint32_t maxConstantCount;
        uint32_t bonesConstantsUsed;
        ShaderResourceUsageTable resourceUsageTable;

        uint32_t sRegisterCompareUseMask;
        // for VS each bit indicates use of semantic name lookup table
        // for PS it is a RGBA mask for each of the 8 render targets
        uint32_t inOutSemanticMask;
        // Needed for null fallback, need to know which kind to use
        uint8_t tRegisterTypes[MAX_T_REGISTERS];
        uint8_t uRegisterTypes[MAX_U_REGISTERS];

        friend bool operator==(const ShaderHeader &l, const ShaderHeader &r) { return 0 == (memcmp(&l, &r, sizeof(ShaderHeader))); }

        friend bool operator!=(const ShaderHeader &l, const ShaderHeader &r) { return !(l == r); }
    };

}