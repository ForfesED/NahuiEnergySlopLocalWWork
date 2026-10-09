// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once
namespace nau::iosys
{
    namespace btag_compr
    {
        //! block bits for IGenLoad::beginBlock(&tag) and IGenSave::endBlock(tag) that denote compression type
        //! these values are written to many data files (part of format) and must not be changed
        static constexpr const unsigned NONE = 0;
        static constexpr const unsigned ZSTD = 1;
        static constexpr const unsigned OODLE = 2;

        static constexpr const unsigned UNSPECIFIED = 0;  //< compression is code dependent, usually obsolete LZMA
    }  // namespace btag_compr
}  // namespace nau::iosys