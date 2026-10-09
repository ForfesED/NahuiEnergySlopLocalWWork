// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/app/application.h"
#include "nau/io/virtual_file_system.h"

namespace nau
{
    namespace
    {
        Application*& getApplicationSingletonPtrRef()
        {
            static Application* appSingletonPtr = nullptr;
            return appSingletonPtr;
        }
    }  // namespace

    void setApplication(Application* app)
    {
        getApplicationSingletonPtrRef() = app;
    }

    Application& getApplication()
    {
        NAU_ASSERT(applicationExists());
        return *getApplicationSingletonPtrRef();
    }

    bool applicationExists()
    {
        return getApplicationSingletonPtrRef() != nullptr;
    }

}  // namespace nau
