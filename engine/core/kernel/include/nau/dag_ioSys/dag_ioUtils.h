// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <nau/dag_ioSys/dag_fileIo.h>
#include <nau/kernel/kernel_config.h>
namespace nau::iosys
{
    NAU_KERNEL_EXPORT void write_zeros(IGenSave& cwr, int byte_num);

    // general file->stream copying
    NAU_KERNEL_EXPORT void copy_file_to_stream(file_ptr_t fp, IGenSave& cwr, int size);
    NAU_KERNEL_EXPORT void copy_file_to_stream(file_ptr_t fp, IGenSave& cwr);
    NAU_KERNEL_EXPORT void copy_file_to_stream(const char* fname, IGenSave& cwr);

    // general stream->stream copying
    NAU_KERNEL_EXPORT void copy_stream_to_stream(IGenLoad& crd, IGenSave& cwr, int size);
}  // namespace nau::iosys