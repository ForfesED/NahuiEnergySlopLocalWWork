// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/io/file_system.h"
#include "nau/kernel/kernel_config.h"
#include "nau/rtti/ptr.h"

/**
 * @brief Provides the interface for virtual file systems and related functionalities.
 */

namespace nau::io
{
    /**
     * @struct IVirtualFileSystem
     * @brief Interface for a virtual file system that supports mounting and unmounting of other file systems.
     * @details This interface extends both `IMutableFileSystem` and `INativeFileSystem`. It provides functionalities
     *          for managing multiple file systems as a unified virtual file system.
     */
    struct NAU_ABSTRACT_TYPE IVirtualFileSystem : IMutableFileSystem,
                                                  INativeFileSystem
    {
        NAU_INTERFACE(nau::io::IVirtualFileSystem, IMutableFileSystem, INativeFileSystem)

        using Ptr = nau::Ptr<IVirtualFileSystem>;  ///< Type alias for a pointer to an `IVirtualFileSystem`.

        /**
         * @brief Mounts a file system to a specified path within the virtual file system.
         * @param path Path at which the file system will be mounted.
         * @param fileSystem Pointer to the file system to mount.
         * @param priority Priority of the mounted file system.
         * @return Result of the operation.
         * @details If multiple file systems are mounted at the same path, the one with the highest priority will be used.
         */
        virtual Result<> mount(const FsPath&, IFileSystem::Ptr, unsigned priority = 1) = 0;

        /**
         * @brief Unmounts a previously mounted file system.
         * @param fileSystem Pointer to the file system to unmount.
         */
        virtual void unmount(IFileSystem::Ptr) = 0;
    };

    /**
     * @brief Creates a new virtual file system.
     * @return Pointer to the created virtual file system.
     */
    NAU_KERNEL_EXPORT
    IVirtualFileSystem::Ptr createVirtualFileSystem();

}  // namespace nau::io
