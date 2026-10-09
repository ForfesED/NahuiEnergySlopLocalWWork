// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <nau/dag_ioSys/dag_genIo.h>
#include <nau/kernel/kernel_config.h>
#include <nau/threading/critical_section.h>
#include <nau/threading/dag_atomic.h>
namespace nau::iosys
{
    // simple circular buffer writer with block support (no mem allocations)
    class NAU_KERNEL_EXPORT SimpleBlockSave : public IGenSave
    {
    public:
        SimpleBlockSave();
        virtual ~SimpleBlockSave()
        {
        }

        void setCircularBuffer(void* mem, int len);
        void setLimits(int start_pos, int end_pos);

        int getEndPos() const
        {
            return limEnd;
        }
        int getLimSize() const
        {
            return limSize;
        }
        int getRootBlockCount() const
        {
            return rootBlkNum;
        }

        virtual void beginBlock();
        virtual void endBlock(unsigned block_flags_2bits = 0);
        virtual int getBlockLevel();

        virtual void write(const void* ptr, int size);
        virtual int tell();
        virtual void seekto(int abs_ofs);
        virtual void seektoend(int rel_ofs = 0);
        virtual const char* getTargetName()
        {
            return "(msg)";
        }
        virtual void flush()
        { /*noop*/
        }

    protected:
        static constexpr int BLOCK_MAX = 32;
        int blkOfs[BLOCK_MAX];
        int blkUsed, rootBlkNum;

        char* buffer;
        int bufferSize;
        int limStart, limEnd, limSize, curPos;
    };

    // simple circular buffer reader with block support (no mem allocations)
    class NAU_KERNEL_EXPORT SimpleBlockLoad : public IGenLoad
    {
    public:
        SimpleBlockLoad();
        virtual ~SimpleBlockLoad()
        {
        }

        void setCircularBuffer(const void* mem, int len);
        void setLimits(int start_pos, int end_pos);

        int getEndPos() const
        {
            return limEnd;
        }
        int getLimSize() const
        {
            return limSize;
        }

        virtual int beginBlock(unsigned* out_blk_flg = NULL);
        virtual void endBlock();
        virtual int getBlockLength();
        virtual int getBlockRest();
        virtual int getBlockLevel();

        virtual void read(void* ptr, int size);
        virtual int tryRead(void* ptr, int size);
        virtual int tell();
        virtual void seekto(int abs_ofs);
        virtual void seekrel(int rel_ofs);
        virtual const char* getTargetName()
        {
            return "(msg)";
        }

    protected:
        static constexpr int BLOCK_MAX = 32;
        int blkOfs[BLOCK_MAX];
        int blkLen[BLOCK_MAX];
        int blkUsed;

        const char* buffer;
        int bufferSize;
        int limStart, limEnd, limSize, curPos;
    };

    //
    // Thread-safe message IO (supports write/read form different threads with fast locks and without memory allocation)
    //
    class NAU_KERNEL_EXPORT ThreadSafeMsgIo
    {
    public:
        ThreadSafeMsgIo(int buf_sz = (16 << 10));
        virtual ~ThreadSafeMsgIo();

        //! returns pointer to reader interface (or NULL, if msg_count==0)
        IGenLoad* startRead(int& msg_count);
        //! finished reading and removes read content from buffer
        void endRead();

        //! returns pointer to writer interface
        IGenSave* startWrite();
        //! finished writing and updates buffer pointers
        void endWrite();

        inline int getWriteAvailableSize() const
        {
            return interlocked_acquire_load(availWrSize);
        }

    protected:
        dag::CriticalSection cc;
        void* buffer;
        int bufferSize;
        int wrPos, rdPos;
        int msgCount, rdMsgCount;
        volatile int availWrSize;

        SimpleBlockSave cwr;
        SimpleBlockLoad crd;
    };

    //
    // Thread-safe message IO with multiple-write-single-read ability
    //
    class NAU_KERNEL_EXPORT ThreadSafeMsgIoEx : public ThreadSafeMsgIo
    {
    public:
        ThreadSafeMsgIoEx(int buf_sz = (16 << 10)) :
            ThreadSafeMsgIo(buf_sz)
        {
        }

        inline IGenSave* startWrite()
        {
            ccWrite.lock();
            return ThreadSafeMsgIo::startWrite();
        }
        inline void endWrite()
        {
            ThreadSafeMsgIo::endWrite();
            ccWrite.unlock();
        }

    protected:
        dag::CriticalSection ccWrite;
    };
}  // namespace nau::iosys