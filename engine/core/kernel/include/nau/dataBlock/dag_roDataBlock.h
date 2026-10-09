// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <nau/math/dag_e3dColor.h>
#include <nau/utils/dag_roNameMap.h>
#include <nau/utils/dag_stdint.h>

#include "nau/math/math.h"
namespace nau
{
    /// Read-only data block (with interface similar to DataBlock)
    struct RoDataBlock
    {
    public:
        /// Parameter types enum.
        enum ParamType
        {
            TYPE_NONE,
            TYPE_STRING,    ///< Text string.
            TYPE_INT,       ///< Integer.
            TYPE_REAL,      ///< #real (float).
            TYPE_POINT2,    ///< nau::math::vec2.
            TYPE_POINT3,    ///< nau::math::vec3.
            TYPE_POINT4,    ///< nau::math::vec4.
            TYPE_IPOINT2,   ///< nau::math::ivec2.
            TYPE_IPOINT3,   ///< nau::math::ivec3.
            TYPE_BOOL,      ///< Boolean.
            TYPE_E3DCOLOR,  ///< E3DCOLOR.
            TYPE_MATRIX,    ///< nau::math::mat4.
            TYPE_INT64,     ///< int64_t
        };

        /// empty Read-only data block constructor
        RoDataBlock()
        {
            memset(this, 0, sizeof(*this));
            nameId = -1;
        }

        /// create Read-only data block via loading dump from stream
        NAU_KERNEL_EXPORT static RoDataBlock* load(iosys::IGenLoad& crd, int sz = -1);

        /// patch data once after creating from dump
        NAU_KERNEL_EXPORT void patchData(void* base);

        /// patch data once after creating from dump
        inline void patchNameMap(void* base)
        {
            nameMap->patchData(base);
        }

        /// Returns name id from NameMap, or -1 if there's no such name in the NameMap.
        NAU_KERNEL_EXPORT int getNameId(const char* name) const;
        NAU_KERNEL_EXPORT const char* getName(int name_id) const;

        /// Returns name id of this RoDataBlock.
        int getBlockNameId() const
        {
            return nameId;
        }

        /// Returns name of this RoDataBlock.
        const char* getBlockName() const
        {
            return getName(nameId);
        }

        /// Returns number of sub-blocks in this RoDataBlock.
        /// Use for enumeration.
        int blockCount() const
        {
            return blocks.size();
        }

        /// Returns pointer to i-th sub-block.
        const RoDataBlock* getBlock(uint32_t idx) const
        {
            return (blocks.size() > idx ? blocks.data() + idx : nullptr);
        }
        RoDataBlock* getBlock(uint32_t idx)
        {
            return (blocks.size() > idx ? blocks.data() + idx : nullptr);
        }

        /// Returns pointer to sub-block with specified name id, or NULL if not found.
        NAU_KERNEL_EXPORT RoDataBlock* getBlockByName(int name_id, int start_after = -1);
        NAU_KERNEL_EXPORT RoDataBlock* getBlockByName(int name_id, int start_after = -1) const
        {
            return const_cast<RoDataBlock*>(this)->getBlockByName(name_id, start_after);
        }

        /// Returns pointer to sub-block with specified name, or NULL if not found.
        RoDataBlock* getBlockByName(const char* name, int start_after = -1)
        {
            return getBlockByName(getNameId(name), start_after);
        }
        const RoDataBlock* getBlockByName(const char* name, int start_after = -1) const
        {
            return getBlockByName(getNameId(name), start_after);
        }

        /// Get block by name, returns (always valid) @b emptyBlock, if not found.
        const RoDataBlock* getBlockByNameEx(const char* name) const
        {
            const RoDataBlock* blk = getBlockByName(name);
            return blk ? blk : &emptyBlock;
        }
        RoDataBlock* getBlockByNameEx(const char* name)
        {
            RoDataBlock* blk = getBlockByName(name);
            return blk ? blk : &emptyBlock;
        }

        /// @name Parameters - Getting and Enumeration
        /// @{

        /// Returns number of parameters in this RoDataBlock.
        /// Use for enumeration.
        int paramCount() const
        {
            return params.size();
        }

        /// Returns type of i-th parameter. See ParamType enum.
        int getParamType(int idx) const
        {
            return (idx >= 0 && idx < params.size()) ? params[idx].type : TYPE_NONE;
        }

        /// Returns i-th parameter name id. See getNameId().
        int getParamNameId(int idx) const
        {
            return (idx >= 0 && idx < params.size()) ? params[idx].nameId : -1;
        }

        /// Returns i-th parameter name. Uses getName().
        const char* getParamName(int param_number) const
        {
            return getName(getParamNameId(param_number));
        }

        /// Find parameter by name id.
        /// Returns parameter index or -1 if not found.
        NAU_KERNEL_EXPORT int findParam(int name_id, int start_after = -1) const;

        /// Find parameter by name. Uses getNameId().
        /// Returns parameter index or -1 if not found.
        int findParam(const char* name, int start_after = -1) const
        {
            return findParam(getNameId(name), start_after);
        }

        /// Returns true if there is parameter with specified name id in this RoDataBlock.
        bool paramExists(int name_id, int start_after = -1) const
        {
            return findParam(name_id, start_after) >= 0;
        }

        /// Returns true if there is parameter with specified name in this RoDataBlock.
        bool paramExists(const char* name, int start_after = -1) const
        {
            return findParam(name, start_after) >= 0;
        }

        const char* getStr(int idx) const
        {
            return (char*)(ptrdiff_t(nameMap.get()) + params[idx].i);
        }
        bool getBool(int idx) const
        {
            return params[idx].b;
        }
        int getInt(int idx) const
        {
            return params[idx].i;
        }
        float getReal(int idx) const
        {
            return params[idx].r;
        }
        nau::math::E3DCOLOR getE3dcolor(int idx) const
        {
            return static_cast<nau::math::E3DCOLOR>(params[idx].i);
        }
        const nau::math::vec2& getPoint2(int idx) const
        {
            return castParam<nau::math::vec2>(idx);
        }
        const nau::math::vec3& getPoint3(int idx) const
        {
            return castParam<nau::math::vec3>(idx);
        }
        const nau::math::vec4& getPoint4(int idx) const
        {
            return castParam<nau::math::vec4>(idx);
        }
        const nau::math::ivec2& getIPoint2(int idx) const
        {
            return castParam<nau::math::ivec2>(idx);
        }
        const nau::math::ivec3& getIPoint3(int idx) const
        {
            return castParam<nau::math::ivec3>(idx);
        }
        const nau::math::mat4& getTm(int idx) const
        {
            return castParam<nau::math::mat4>(idx);
        }
        int64_t getInt64(int idx) const
        {
            return *(int64_t*)(ptrdiff_t(nameMap.get()) + params[idx].i);
        }

        const char* getStr(const char* name, const char* def) const
        {
            int i = findParam(name);
            if(i >= 0 && params[i].type == TYPE_STRING)
                return getStr(i);
            return def;
        }

        bool getBool(const char* name, bool def) const
        {
            int i = findParam(name);
            if(i >= 0 && params[i].type == TYPE_BOOL)
                return getBool(i);
            return def;
        }

        int getInt(const char* name, int def) const
        {
            int i = findParam(name);
            if(i >= 0 && params[i].type == TYPE_INT)
                return getInt(i);
            return def;
        }

        float getReal(const char* name, float def) const
        {
            int i = findParam(name);
            if(i >= 0 && params[i].type == TYPE_REAL)
                return getReal(i);
            return def;
        }

        nau::math::E3DCOLOR getE3dcolor(const char* name, nau::math::E3DCOLOR def) const
        {
            int i = findParam(name);
            if(i >= 0 && params[i].type == TYPE_E3DCOLOR)
                return getE3dcolor(i);
            return def;
        }

        const nau::math::vec2& getPoint2(const char* name, const nau::math::vec2& def) const
        {
            int i = findParam(name);
            if(i >= 0 && params[i].type == TYPE_POINT2)
                return getPoint2(i);
            return def;
        }

        const nau::math::vec3& getPoint3(const char* name, const nau::math::vec3& def) const
        {
            int i = findParam(name);
            if(i >= 0 && params[i].type == TYPE_POINT3)
                return getPoint3(i);
            return def;
        }

        const nau::math::vec4& getPoint4(const char* name, const nau::math::vec4& def) const
        {
            int i = findParam(name);
            if(i >= 0 && params[i].type == TYPE_POINT4)
                return getPoint4(i);
            return def;
        }

        const nau::math::ivec2& getIPoint2(const char* name, const nau::math::ivec2& def) const
        {
            int i = findParam(name);
            if(i >= 0 && params[i].type == TYPE_IPOINT3)
                return getIPoint2(i);
            return def;
        }

        const nau::math::ivec3& getIPoint3(const char* name, const nau::math::ivec3& def) const
        {
            int i = findParam(name);
            if(i >= 0 && params[i].type == TYPE_IPOINT3)
                return getIPoint3(i);
            return def;
        }

        const nau::math::mat4& getTm(const char* name, const nau::math::mat4& def) const
        {
            int i = findParam(name);
            if(i >= 0 && params[i].type == TYPE_MATRIX)
                return getTm(i);
            return def;
        }

        int64_t getInt64(const char* name, int64_t def) const
        {
            int i = findParam(name);
            if(i >= 0 && params[i].type == TYPE_INT64)
                return getInt64(i);
            return def;
        }

        inline bool isValid() const
        {
            return true;
        }

    protected:
        struct Param
        {
            union
            {
                int i;
                bool b;
                float r;
            };
            uint16_t nameId, type;
        };

        PatchableTab<Param> params;
        PatchableTab<RoDataBlock> blocks;
        PatchablePtr<dag::RoNameMap> nameMap;
        int nameId;
        int _resv;

        NAU_KERNEL_EXPORT static RoDataBlock emptyBlock;
        template <class T>
        const T& castParam(int idx) const
        {
            return *(T*)(ptrdiff_t(nameMap.get()) + params[idx].i);
        }
    };
}  // namespace nau