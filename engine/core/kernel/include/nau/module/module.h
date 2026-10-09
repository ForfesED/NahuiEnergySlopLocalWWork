// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/module/module_entry.h


#pragma once

#if !defined(NAU_MODULE_NAME) && !defined(NAU_KERNEL_BUILD)
    #error "module.h must be used only for module build"
#endif


#include <type_traits>

#include "EASTL/shared_ptr.h"
#include "EASTL/internal/smart_ptr.h"
#include "nau/kernel/kernel_config.h"
#include "nau/rtti/ptr.h"
#include "nau/rtti/rtti_impl.h"
#include "nau/service/service_provider.h"
#include "nau/utils/functor.h"
#include <nau/string/string.h>


namespace nau
{

    struct IModule
    {
        virtual string getModuleName() = 0; // could be usefull for Editor

        virtual void initialize() = 0;

        virtual void deinitialize() = 0;

        virtual void postInit() = 0;

        virtual ~IModule() {}

    };


#if defined(NAU_STATIC_RUNTIME)
    #define IMPLEMENT_MODULE( ModuleClass ) \
        eastl::shared_ptr<nau::IModule> PP_CONCATENATE(createModule_, NAU_MODULE_NAME)() \
        { \
            return eastl::make_shared<ModuleClass>(); \
        } \

#else
    #define IMPLEMENT_MODULE( ModuleClass ) \
        extern "C" __declspec(dllexport) nau::IModule* createModule() \
        { \
            return new ModuleClass(); \
        } \

#endif

    struct DefaultModuleImpl : public IModule
    {
        string getModuleName() override
        {
            return "DefaultModuleImpl";
        }

        void initialize() override
        {
        }

        void deinitialize() override
        {
        }

        void postInit() override
        {
        }
    };


}  // namespace nau


namespace eastl
{
    template<>
    struct default_delete<nau::IModule>
    {
        void operator()(nau::IModule* p) const
           { delete p; }
    };
}


#define NAU_MODULE_EXPORT_CLASS(Type) ::nau::getServiceProvider().addClass<Type>()

#define NAU_MODULE_EXPORT_SERVICE(Type) ::nau::getServiceProvider().addService<Type>()
