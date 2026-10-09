// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include <nau/dag_ioSys/dag_ioUtils.h>
#include <nau/osApiWrappers/dag_files.h>

namespace nau::iosys
{
#if _TARGET_PC
    static constexpr int BUF_SZ = 32 << 10;
#else
    static constexpr int BUF_SZ = 16 << 10;
#endif

    void write_zeros(IGenSave& cwr, int size)
    {
        char zero[BUF_SZ];
        memset(zero, 0, BUF_SZ);

        while(size > BUF_SZ)
        {
            cwr.write(zero, BUF_SZ);
            size -= BUF_SZ;
        }

        if(size)
            cwr.write(zero, size);
    }

    void copy_stream_to_stream(IGenLoad& crd, IGenSave& cwr, int size)
    {
        int len;
        char buf[BUF_SZ];
        while(size > 0)
        {
            len = size > BUF_SZ ? BUF_SZ : size;
            size -= len;
            crd.read(buf, len);
            cwr.write(buf, len);
        }
    }
    void copy_file_to_stream(file_ptr_t fp, IGenSave& cwr, int size)
    {
        int len;
        char buf[BUF_SZ];
        while(size > 0)
        {
            len = size > BUF_SZ ? BUF_SZ : size;
            size -= len;
            hal::df_read(fp, buf, len);
            cwr.write(buf, len);
        }
    }
    void copy_file_to_stream(file_ptr_t fp, IGenSave& cwr)
    {
        copy_file_to_stream(fp, cwr, hal::df_length(fp));
    }

    void copy_file_to_stream(const char* fname, IGenSave& cwr)
    {
        file_ptr_t fp = df_open(fname, hal::DF_READ);
        if(!fp)
            NAU_THROW(IGenSave::SaveException("file not found", 0));
        copy_file_to_stream(fp, cwr, hal::df_length(fp));
        hal::df_close(fp);
    }
}  // namespace nau::iosys