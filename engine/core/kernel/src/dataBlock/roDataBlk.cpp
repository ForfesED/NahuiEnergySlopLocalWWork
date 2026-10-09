// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#include <nau/dag_ioSys/dag_genIo.h>
#include <nau/dataBlock/dag_roDataBlock.h>

#include "nau/memory/mem_allocator.h"

namespace nau
{

    RoDataBlock RoDataBlock::emptyBlock;

    RoDataBlock* RoDataBlock::load(iosys::IGenLoad& crd, int sz)
    {
        if(sz == -1)
            crd.readInt(sz);

        void* mem = nau::getDefaultAllocator()->allocate(sz);  // memalloc(sz, tmpmem);
        RoDataBlock* blk = new (mem) RoDataBlock;

        crd.read(blk, sz);
        blk->patchData(mem);
        blk->patchNameMap(mem);
        return blk;
    }

    void RoDataBlock::patchData(void* base)
    {
        nameMap.patch(base);
        blocks.patch(base);
        params.patch(base);
        for(int i = 0; i < blocks.size(); ++i)
            blocks[i].patchData(base);
    }

    int RoDataBlock::getNameId(const char* name) const
    {
        return nameMap ? nameMap->getNameId(reinterpret_cast<const char*>(name)) : -1;
    }
    const char* RoDataBlock::getName(int name_id) const
    {
        return nameMap ? reinterpret_cast<const char*>(nameMap->map[name_id].get()) : nullptr;
    }

    RoDataBlock* RoDataBlock::getBlockByName(int nid, int after)
    {
        for(int i = after + 1; i < blocks.size(); ++i)
            if(blocks[i].nameId == nid)
                return &blocks[i];
        return NULL;
    }

    int RoDataBlock::findParam(int nid, int after) const
    {
        for(int i = after + 1; i < params.size(); ++i)
            if(params[i].nameId == nid)
                return i;
        return -1;
    }
}  // namespace nau