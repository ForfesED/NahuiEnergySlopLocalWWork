// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/kernel/kernel_config.h"
#include "nau/dag_ioSys/dag_baseIo.h"
#include "nau/memory/mem_allocator.h"

namespace nau::iosys
{
    // Chained memory segments to store data
    struct NAU_KERNEL_EXPORT MemoryChainedData
    {
    public:
        MemoryChainedData* next;  // link to next data segment in chain
        ptrdiff_t size;           // size of data
        ptrdiff_t offset;         // absolute offset from chain beginning
        ptrdiff_t used;           // used size of data (<=size)
        char data[16];            // pointer to segment's data

        inline ptrdiff_t calcTotalSize() const
        {
            return calcTotalSize(this);
        }
        inline ptrdiff_t calcTotalUsedSize() const
        {
            return calcTotalUsedSize(this);
        }
        inline bool cmpEq(const MemoryChainedData* m) const
        {
            return cmpEq(this, m);
        }

    public:
        // creates new chain segment; if parent==NULL, main segment is created
        static MemoryChainedData* create(ptrdiff_t sz, MemoryChainedData* parent = NULL)
        {
            if(parent && parent->next)
                return NULL;  // error, p already has next chained data

            // MemoryChainedData *mcd = (MemoryChainedData *)::midmem->alloc(sz + sizeof(MemoryChainedData) - 16);
            MemoryChainedData* mcd = (MemoryChainedData*)getDefaultAllocator()->allocate(sz + sizeof(MemoryChainedData) - 16);
            mcd->next = NULL;
            // midmem
            mcd->size = getDefaultAllocator()->getSize(mcd) - sizeof(MemoryChainedData) + 16;
            mcd->used = 0;
            if(parent)
            {
                parent->next = mcd;
                mcd->offset = parent->offset + parent->size;
            }
            else
                mcd->offset = 0;

            return mcd;
        }

        // deletes all segments of chain starting from 'main'
        static void deleteChain(MemoryChainedData* main)
        {
            while(main)
            {
                MemoryChainedData* next = main->next;
                delete[] main;
                getDefaultAllocator()->deallocate(main);
                main = next;
            }
        }

        // calculate total size of all segments of chain starting from 'main'
        static ptrdiff_t calcTotalSize(const MemoryChainedData* main)
        {
            ptrdiff_t size = 0;
            while(main)
            {
                size += main->size;
                main = main->next;
            }
            return size;
        }

        // calculate total used size of all segments of chain starting from 'main'
        static ptrdiff_t calcTotalUsedSize(const MemoryChainedData* main)
        {
            ptrdiff_t size = 0;
            while(main && main->used > 0)
            {
                size += main->used;
                main = main->next;
            }
            return size;
        }

        static bool cmpEq(const MemoryChainedData* m1, const MemoryChainedData* m2)
        {
            ptrdiff_t len = calcTotalUsedSize(m1);
            if(len != calcTotalUsedSize(m2))
                return false;
            const char* data1 = m1->data;
            const char* data2 = m2->data;
            ptrdiff_t sz1 = m1->used, sz2 = m2->used;

            while(len > 0)
            {
                ptrdiff_t sz = sz1 < sz2 ? sz1 : sz2;
                if(sz > len)
                    sz = len;
                if(memcmp(data1, data2, sz) != 0)
                    return false;
                len -= sz;
                if(!len)
                    return true;
                sz1 -= sz;
                if(sz1)
                    data1 += sz;
                else
                {
                    m1 = m1->next;
                    if(!m1)
                        return false;
                    data1 = m1->data;
                    sz1 = m1->used;
                }
                sz2 -= sz;
                if(sz2)
                    data2 += sz;
                else
                {
                    m2 = m2->next;
                    if(!m2)
                        return false;
                    data2 = m2->data;
                    sz2 = m2->used;
                }
            }
            return false;
        }

    private:
        MemoryChainedData(const MemoryChainedData&);
        MemoryChainedData& operator=(const MemoryChainedData&);
    };

    // Memory save callback interface
    class NAU_KERNEL_EXPORT MemorySaveCB : public IBaseSave
    {
    public:
        MemorySaveCB();
        MemorySaveCB(MemoryChainedData* _ch, bool auto_delete);
        MemorySaveCB(int init_sz);
        virtual ~MemorySaveCB();

        MemoryChainedData* takeMem();
        MemoryChainedData* getMem()
        {
            return mcd;
        }

        void deleteMem();
        void setMem(MemoryChainedData* _ch, bool auto_delete);
        void setRange(int min_pos, int max_pos);
        void setMcdMinMax(int min_size, int max_size);

        int getTotalReservedSize() const
        {
            return (int)totalSize;
        }
        int getSize() const
        {
            return (int)MemoryChainedData::calcTotalUsedSize(mcd);
        }

        virtual void write(const void* ptr, int size);
        virtual int tell()
        {
            return (int)pos;
        }
        virtual void seekto(int pos);
        virtual void seektoend(int ofs = 0)
        {
            MemorySaveCB::seekto(int(MemoryChainedData::calcTotalUsedSize(mcd) + ofs));
        }
        virtual const char* getTargetName()
        {
            return "(mem)";
        }
        virtual void flush()
        { /*noop*/
        }

        void copyDataTo(IGenSave& dest_cwr)
        {
            MemoryChainedData* mem = mcd;
            while(mem)
            {
                if(!mem->used)
                    break;
                dest_cwr.write(mem->data, (int)mem->used);
                mem = mem->next;
            }
        }

    protected:
        MemoryChainedData *mcd, *cMcd;
        ptrdiff_t minPos, maxPos;
        ptrdiff_t pos, totalSize;
        int minMcdSz, maxMcdSz;
        bool autoDelete;

        void initDefs();
        void extendMemory(ptrdiff_t sz);
    };

    // Memory load callback interface
    class NAU_KERNEL_EXPORT MemoryLoadCB : public IBaseLoad
    {
    public:
        MemoryLoadCB();
        MemoryLoadCB(const MemoryChainedData* _ch, bool auto_delete);
        virtual ~MemoryLoadCB();

        MemoryChainedData* takeMem();
        const MemoryChainedData* getMem()
        {
            return mcd;
        }

        void deleteMem();
        void setMem(const MemoryChainedData* _ch, bool auto_delete);

        bool isReady()
        {
            return mcd != NULL;
        }

        // rest of IGenLoad interface to implement
        virtual void read(void* ptr, int size) override
        {
            if(MemoryLoadCB::tryRead(ptr, size) != size)
                throw(LoadException("read error", (int)pos));
        }

        virtual int tryRead(void* ptr, int size) override;
        virtual void seekrel(int rel_ofs) override;
        virtual int tell() override
        {
            return (int)pos;
        }
        virtual void seekto(int pos) override;
        virtual const char* getTargetName() override
        {
            return "(mem)";
        }
        int64_t getTargetDataSize() override
        {
            return targetDataSz;
        }

    protected:
        const MemoryChainedData *mcd, *cMcd;
        ptrdiff_t pos;
        int64_t targetDataSz = -1;
        bool autoDelete;

        void initDefs();
    };
}  // namespace nau::iosys