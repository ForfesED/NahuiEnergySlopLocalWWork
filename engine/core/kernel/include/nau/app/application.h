// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/async/executor.h"
#include "nau/kernel/kernel_config.h"
#include "nau/rtti/rtti_object.h"
#include "nau/utils/functor.h"
#include "nau/utils/result.h"


namespace nau
{
    /**
     */
    struct NAU_ABSTRACT_TYPE NAU_KERNEL_EXPORT Application : IRttiObject
    {
        NAU_INTERFACE(nau::Application, IRttiObject)

        virtual void startupOnCurrentThread() = 0;
        virtual bool isMainThread() = 0;

        virtual bool step() = 0;

        virtual void stop() = 0;

        virtual bool isClosing() const = 0;

        virtual async::Executor::Ptr getExecutor() = 0;
        virtual bool hasExecutor() = 0;
    };

    NAU_KERNEL_EXPORT
    void setApplication(Application*);

    NAU_KERNEL_EXPORT
    Application& getApplication();

    NAU_KERNEL_EXPORT
    bool applicationExists();

}  // namespace nau
