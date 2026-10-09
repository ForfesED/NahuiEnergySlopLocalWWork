// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/async/task.h"
#include "nau/io/io_constants.h"
#include "nau/kernel/kernel_config.h"
#include "nau/rtti/ptr.h"
#include "nau/rtti/rtti_object.h"
#include "nau/utils/result.h"

/**
 * @brief Provides base interfaces for stream representations and utility functions for stream operations.
 */

namespace nau::io
{
    /**
     * @struct IStreamBase
     * @brief Base interface for any stream representation.
     *
     * `IStreamBase` defines the basic operations for interacting with a stream, including querying and setting the current position.
     */
    struct NAU_ABSTRACT_TYPE IStreamBase : virtual IRefCounted
    {
        NAU_INTERFACE(nau::io::IStreamBase, IRefCounted)

        using Ptr = nau::Ptr<IStreamBase>; /**< A smart pointer type for `IStreamBase`. */

        /**
         * @brief Returns the current stream position if supported.
         * @return The current position of the stream.
         */
        virtual size_t getPosition() const = 0;

        /**
         * @brief Sets a new stream position as an offset from a specified origin.
         * @param origin The offset origin: current position, beginning, or end of the stream.
         * @param offset The number of bytes to move the stream pointer. Positive values move the pointer forward, and negative values move it backward.
         * @return The new position of the stream if the operation is supported.
         */
        virtual size_t setPosition(OffsetOrigin origin, int64_t offset) = 0;
    };

    /**
     * @struct IStreamReader
     * @brief Interface for reading operations on a stream.
     *
     * `IStreamReader` extends `IStreamBase` and provides methods for reading data from the stream.
     */
    struct NAU_ABSTRACT_TYPE IStreamReader : virtual IStreamBase
    {
        NAU_INTERFACE(nau::io::IStreamReader, IStreamBase)

        using Ptr = nau::Ptr<IStreamReader>; /**< A smart pointer type for `IStreamReader`. */

        /**
         * @brief Reads data from the stream.
         * @param buffer A pointer to the buffer where the read data will be stored.
         * @param count The number of bytes to read.
         * @return A `Result` containing the number of bytes read.
         */
        virtual Result<size_t> read(std::byte* buffer, size_t count) = 0;
    };

    // struct NAU_ABSTRACT_TYPE IAsyncStreamReader : virtual IStreamBase
    // {
    //     NAU_INTERFACE(nau::io::IAsyncStreamReader, IStreamBase)

    //     using Ptr = nau::Ptr<IAsyncStreamReader>;

    //     virtual Result<size_t> read(std::byte*, size_t count) = 0;
    // };

    /**
     * @struct IStreamWriter
     * @brief Interface for writing operations on a stream.
     *
     * `IStreamWriter` extends `IStreamBase` and provides methods for writing data to the stream and flushing the stream.
     */
    struct NAU_ABSTRACT_TYPE IStreamWriter : virtual IStreamBase
    {
        NAU_INTERFACE(nau::io::IStreamWriter, IStreamBase)

        using Ptr = nau::Ptr<IStreamWriter>; /**< A smart pointer type for `IStreamWriter`. */

        /**
         * @brief Writes data to the stream.
         * @param buffer A pointer to the buffer containing the data to be written.
         * @param count The number of bytes to write.
         * @return A `Result` containing the number of bytes written.
         */
        virtual Result<size_t> write(const std::byte* buffer, size_t count) = 0;

        /**
         * @brief Flushes the stream, ensuring all buffered data is written.
         */
        virtual void flush() = 0;
    };

    /**
     * @brief Copies data from a reader to a buffer.
     * @param dst A pointer to the destination buffer.
     * @param size The number of bytes to copy.
     * @param src The `IStreamReader` instance to read from.
     * @return A `Result` containing the number of bytes copied.
     */
    NAU_KERNEL_EXPORT
    Result<size_t> copyFromStream(void* dst, size_t size, IStreamReader& src);

    /**
     * @brief Copies data from a reader to a writer.
     * @param dst The `IStreamWriter` instance to write to.
     * @param size The number of bytes to copy.
     * @param src The `IStreamReader` instance to read from.
     * @return A `Result` containing the number of bytes copied.
     */
    NAU_KERNEL_EXPORT
    Result<size_t> copyFromStream(IStreamWriter& dst, size_t size, IStreamReader& src);

    /**
     * @brief Copies an entire stream from a reader to a writer.
     * @param dst The `IStreamWriter` instance to write to.
     * @param src The `IStreamReader` instance to read from.
     * @return A `Result` containing the number of bytes copied.
     */
    NAU_KERNEL_EXPORT
    Result<size_t> copyStream(IStreamWriter& dst, IStreamReader& src);
}  // namespace nau::io
