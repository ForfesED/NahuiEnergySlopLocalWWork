// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once
#include <EASTL/string.h>
#include "nau/app/application.h"
#include "nau/app/application_services.h"
#include "nau/app/global_properties.h"
#include "nau/service/service_provider.h"

namespace nau::test
{
    class AppGuard : public ApplicationInitDelegate
    {
    public:
#if !defined(NAU_MODULES_LIST)
        AppGuard(eastl::string modulesList);
#else
        AppGuard(eastl::string modulesList = eastl::string{NAU_MODULES_LIST});
#endif

        ~AppGuard();

        void start();

        void stop();

    public:
        template <typename... T>
        static void registerClasses()
        {
            auto& serviceProvider = getServiceProvider();
            (serviceProvider.addClass<T>(), ...);
        }

        template <typename... T>
        static void registerServices()
        {
            auto& serviceProvider = getServiceProvider();
            (serviceProvider.addService<T>(), ...);
        }

    protected:
        virtual void setupTestServices()
        {
        }

    private:
        Result<> configureApplication() final;
        Result<> initializeApplication() final;

        eastl::unique_ptr<Application> m_app;
        eastl::string m_modulesList;
    };
}  // namespace nau::test
