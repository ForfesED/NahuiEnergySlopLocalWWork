// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <optional>

#include "nau/dispatch/dispatch_args.h"
#include "nau/meta/runtime_attribute.h"
#include "nau/rtti/ptr.h"
#include "nau/rtti/rtti_object.h"
#include "nau/serialization/runtime_value.h"
#include "nau/string/string_utils.h"
#include "nau/utils/result.h"

namespace nau
{
    /**
     */
    enum class MethodCategory
    {
        Instance,
        Class
    };

    /**
        @brief Dynamic method description
     */
    struct NAU_ABSTRACT_TYPE IMethodInfo
    {
        virtual ~IMethodInfo() = default;

        virtual std::string getName() const = 0;

        virtual MethodCategory getCategory() const = 0;

        virtual std::optional<unsigned> getParametersCount() const = 0;

        /**
            @brief Dynamically (without type knowing) invoking the method.
                  The caller must take care to properly destroy the returned object according to its type !

            @return The invocation result. Currently can represent:
                - any value through RuntimeValue api
                - invocable object (i.e. functor/lambda) through IDispatch interface
                - any other IRttiOject based object. Be aware

            TODO: IRttiObject*  MUST be replaced by some kind of 'Universal' pointer !
         */
        virtual Result<IRttiObject*> invoke(IRttiObject* instance, DispatchArguments args) const = 0;

        /**
            @brief temporary method.
                MUST be removed after invoke will be refatored to returns Universal ptr
         */
        nau::Ptr<> invokeToPtr(IRttiObject* instance, DispatchArguments args) const
        {
            Result<IRttiObject*> result = this->invoke(instance, std::move(args));
            IRttiObject* const obj = *result;
            if (!obj)
            {
                return {};
            }

            IRefCounted* const refCounted = obj->as<IRefCounted*>();
            NAU_FATAL(refCounted);

            return rtti::TakeOwnership{refCounted};
        }
    };

    /**
        @brief Dynamic interface/API description
     */
    struct NAU_ABSTRACT_TYPE IInterfaceInfo
    {
        virtual std::string getName() const = 0;

        virtual const rtti::TypeInfo* getTypeInfo() const = 0;

        virtual size_t getMethodsCount() const = 0;

        virtual const IMethodInfo& getMethod(size_t) const = 0;
    };

    /**
        @brief Dynamic type description
     */
    struct NAU_ABSTRACT_TYPE IClassDescriptor : virtual IRefCounted
    {
        NAU_INTERFACE(nau::IClassDescriptor, IRefCounted)

        using Ptr = nau::Ptr<IClassDescriptor>;

        virtual const rtti::TypeInfo& getClassTypeInfo() const = 0;

        virtual std::string getClassName() const = 0;

        virtual const meta::IRuntimeAttributeContainer* getClassAttributes() const = 0;

        virtual size_t getInterfaceCount() const = 0;

        virtual const IInterfaceInfo& getInterface(size_t) const = 0;

        /**
            @brief Class's instance construction factory

            @return Constructor method if instance construction is available, nullptr otherwise
         */
        virtual const IMethodInfo* getConstructor() const = 0;

        const IInterfaceInfo* findInterface(const rtti::TypeInfo&) const;

        template <rtti::WithTypeInfo>
        const IInterfaceInfo* findInterface() const;

        bool hasInterface(const rtti::TypeInfo&) const;

        template <rtti::WithTypeInfo>
        bool hasInterface() const;

        const IMethodInfo* findMethod(std::string_view) const;
    };

    /**
     */
    inline const IInterfaceInfo* IClassDescriptor::findInterface(const rtti::TypeInfo& typeInfo) const
    {
        for (size_t i = 0, iCount = getInterfaceCount(); i < iCount; ++i)
        {
            const IInterfaceInfo& api = getInterface(i);
            const rtti::TypeInfo* const apiType = api.getTypeInfo();
            if (apiType && (*apiType == typeInfo))
            {
                return &api;
            }
        }

        return nullptr;
    }

    template <rtti::WithTypeInfo T>
    inline const IInterfaceInfo* IClassDescriptor::findInterface() const
    {
        return findInterface(rtti::getTypeInfo<T>());
    }

    inline bool IClassDescriptor::hasInterface(const rtti::TypeInfo& type) const
    {
        return findInterface(type) != nullptr;
    }

    template <rtti::WithTypeInfo T>
    inline bool IClassDescriptor::hasInterface() const
    {
        return findInterface(rtti::getTypeInfo<T>()) != nullptr;
    }

    /**
     */
    inline const IMethodInfo* IClassDescriptor::findMethod(std::string_view methodName) const
    {
        for (size_t i = 0, iCount = getInterfaceCount(); i < iCount; ++i)
        {
            const IInterfaceInfo& api = getInterface(i);
            for (size_t j = 0, mCount = api.getMethodsCount(); j < mCount; ++j)
            {
                const auto& method = api.getMethod(j);
                if (strings::icaseEqual(methodName, method.getName()))
                {
                    return &method;
                }
            }
        }

        return nullptr;
    }
}  // namespace nau
