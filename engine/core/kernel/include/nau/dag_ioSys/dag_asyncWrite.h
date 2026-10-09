// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <nau/dag_ioSys/dag_genIo.h>

#include "nau/kernel/kernel_config.h"
namespace nau::iosys
{
    enum class AsyncWriterMode
    {
        TRUNC,
        APPEND
    };

    NAU_KERNEL_EXPORT IGenSave* create_async_writer(const char* fname, int buf_size, AsyncWriterMode mode = AsyncWriterMode::TRUNC);
    NAU_KERNEL_EXPORT IGenSave* create_async_writer_temp(char* in_out_fname, int buf_size);
}  // namespace nau::iosys