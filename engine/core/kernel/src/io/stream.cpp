// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/io/stream.h"

#include "nau/memory/bytes_buffer.h"

namespace nau::io
{

    Result<size_t> copyFromStream(void* dst, size_t size, IStreamReader& src)
    {
        if(size == 0)
        {
            return 0;
        }

        NAU_ASSERT(dst);
        if(!dst)
        {
            return NauMakeError("Invalid dst");
        }

        size_t readOffset = 0;

        for(; readOffset < size;)
        {
            const size_t readCount = size - readOffset;
            const auto readResult = src.read(reinterpret_cast<std::byte*>(dst) + readOffset, readCount);
            NauCheckResult(readResult);

            if(*readResult == 0)
            {
                break;
            }

            readOffset += *readResult;
            NAU_ASSERT(readOffset <= size);
        }

        return readOffset;
    }
    
    Result<size_t> copyFromStream(IStreamWriter& dst, size_t size, IStreamReader& src)
    {
        if(size == 0)
        {
            return 0;
        }

        size_t readOffset = 0;
        BytesBuffer buffer(size);
        for(; readOffset < size;)
        {
            const size_t readCount = size - readOffset;
            const auto readResult = src.read(reinterpret_cast<std::byte*>(buffer.data()) + readOffset, readCount);
            NauCheckResult(readResult);

            if(*readResult == 0)
            {
                break;
            }

            readOffset += *readResult;
            NAU_ASSERT(readOffset <= size);
        }

        return *dst.write(buffer.data(), size);
    }

    Result<size_t> copyStream(IStreamWriter& dst, IStreamReader& src)
    {
        constexpr size_t BlockSize = 4096;

        BytesBuffer buffer(BlockSize);
        size_t totalRead = 0;

        do
        {
            auto readResult = src.read(buffer.data(), BlockSize);
            NauCheckResult(readResult);

            const size_t actualRead = *readResult;
            totalRead += actualRead;

            if(actualRead < BlockSize)
            {
                buffer.resize(actualRead);
                const auto writeResult = dst.write(buffer.data(), buffer.size());
                NauCheckResult(writeResult);
                break;
            }

            const auto writeResult = dst.write(buffer.data(), buffer.size());
            NauCheckResult(writeResult);

        } while(true);

        return totalRead;
    }
}  // namespace nau::io
