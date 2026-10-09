// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include <nau/dag_ioSys/dag_baseIo.h>
#include <nau/debug/dag_except.h>
namespace nau::iosys
{
    IBaseSave::IBaseSave() = default;

    IBaseSave::~IBaseSave() = default;

    void IBaseSave::beginBlock()
    {
        int ofs = 0;
        write(&ofs, sizeof(int));

        int o = tell();
        blocks.emplace_back(Block{o});
    }

    void IBaseSave::endBlock(unsigned block_flags)
    {
        NAU_ASSERT(block_flags <= 0x3, "block_flags={:08x}", block_flags);  // 2 bits max
        if(blocks.size() <= 0)
            NAU_THROW(SaveException("block not begun", tell()));

        Block& b = blocks.back();
        int o = tell();
        seekto(b.ofs - sizeof(int));
        int l = o - b.ofs;
        NAU_ASSERT(l >= 0 && !(l & 0xC0000000), "o={:08x} b.ofs={:08x} l={:08x}", o, b.ofs, l);
        l |= (block_flags << 30);
        write(&l, sizeof(int));
        seekto(o);

        blocks.pop_back();
    }

    int IBaseSave::getBlockLevel()
    {
        return blocks.size();
    }

    IBaseLoad::IBaseLoad() = default;
    IBaseLoad::~IBaseLoad() = default;

    int IBaseLoad::beginBlock(unsigned* out_block_flags)
    {
        int l = 0;
        read(&l, sizeof(int));
        if(out_block_flags)
            *out_block_flags = (l >> 30) & 0x3u;
        l &= 0x3FFFFFFF;

        int o = tell();
        blocks.emplace_back(Block{o, l});
        return l;
    }

    void IBaseLoad::endBlock()
    {
        if(blocks.size() <= 0)
            NAU_THROW(LoadException("endBlock without beginBlock", tell()));

        Block& b = blocks.back();
        seekto(b.ofs + b.len);

        blocks.pop_back();
    }

    int IBaseLoad::getBlockLength()
    {
        if(blocks.size() <= 0)
            NAU_THROW(LoadException("block not begun", tell()));

        Block& b = blocks.back();
        return b.len;
    }

    int IBaseLoad::getBlockRest()
    {
        if(blocks.size() <= 0)
            NAU_THROW(LoadException("block not begun", tell()));

        Block& b = blocks.back();
        return b.ofs + b.len - tell();
    }

    int IBaseLoad::getBlockLevel()
    {
        return blocks.size();
    }
}  // namespace nau::iosys