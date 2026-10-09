// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/utils/typed_flag.h"

/**
 * @brief Contains constants and enumerations used for file I/O operations.
 */

namespace nau::io
{
    /**
     * @enum AccessMode
     * @brief Specifies the mode in which a file or resource is accessed.
     *
     * The `AccessMode` enum defines various flags for file access, such as reading, writing, and asynchronous operations.
     */
    enum class AccessMode : unsigned
    {
        /**
         * @brief Read access mode.
         *
         * This flag indicates that the file or resource should be opened for reading.
         */
        Read = NauFlag(1),

        /**
         * @brief Write access mode.
         *
         * This flag indicates that the file or resource should be opened for writing.
         */
        Write = NauFlag(2),

        /**
         * @brief Asynchronous access mode.
         *
         * This flag indicates that the file or resource should be accessed asynchronously.
         */
        Async = NauFlag(3)
    };

    /**
     * @brief Define typed flags for the `AccessMode` enum.
     *
     * This macro generates the necessary implementations for using `AccessMode` as a set of flags.
     */
    NAU_DEFINE_TYPED_FLAG(AccessMode)

    /**
     * @enum OpenFileMode
     * @brief Specifies the mode in which a file is opened.
     *
     * The `OpenFileMode` enum defines various options for opening a file, such as creating or opening an existing file.
     */
    enum class OpenFileMode
    {
        /**
         * @brief Always create a new file.
         *
         * If the file does not exist, it will be created. If the file already exists, it will be overwritten.
         */
        CreateAlways,

        /**
         * @brief Create a new file.
         *
         * If the file already exists, an error will be returned.
         */
        CreateNew,

        /**
         * @brief Open the file if it exists, otherwise create it.
         *
         * If the file does not exist, it will be created. If it exists, it will be opened.
         */
        OpenAlways,

        /**
         * @brief Open an existing file.
         *
         * If the file does not exist, an error will be returned.
         */
        OpenExisting
    };

    /**
     * @enum OffsetOrigin
     * @brief Specifies the origin for seeking within a stream of data.
     *
     * The `OffsetOrigin` enum defines the starting point for seeking operations within a stream of data.
     */
    enum class OffsetOrigin
    {
        /**
         * @brief Current position.
         *
         * The offset is relative to the current position within the stream of data.
         */
        Current,

        /**
         * @brief Beginning of the stream of data.
         *
         * The offset is relative to the beginning of the stream of data.
         */
        Begin,

        /**
         * @brief End of the stream of data.
         *
         * The offset is relative to the end of the stream of data.
         */
        End
    };

}  // namespace nau::io
