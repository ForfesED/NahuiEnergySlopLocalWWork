// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/rtti/rtti_utils.h


#pragma once

#include "nau/meta/class_info.h"
#include "nau/rtti/rtti_object.h"
#include "nau/utils/type_list/append.h"

namespace nau::rtti_detail
{

    template <typename Target, typename T, typename... Base>
    inline bool staticCastHelper(T& instance, Target*& target, TypeList<Base...>)
    {
        if constexpr(std::is_convertible_v<T*, Target*>)
        {
            target = &static_cast<Target&>(instance);
            return true;
        }
        else
        {
            return (staticCastHelper(static_cast<Base&>(instance), target, meta::ClassDirectBase<Base>{}) || ...);
        }
    }

    template <typename T, typename... Base>
    inline bool runtimeCastHelper(T& instance, const rtti::TypeInfo& targetTypeId, void** target, TypeList<Base...>)
    {
        const auto& instanceTypeId = rtti::getTypeInfo<T>();
        if(instanceTypeId == targetTypeId)
        {
            *target = reinterpret_cast<void*>(&instance);
            return true;
        }

        return (runtimeCastHelper(static_cast<Base&>(instance), targetTypeId, target, meta::ClassDirectBase<Base>{}) || ...);
    }

    template <typename T, typename... Base>
    consteval bool isConvertibleHelper(TypeList<Base...>)
    {
        return (std::is_convertible_v<Base*, T*> || ...);
    }

}  // namespace nau::rtti_detail

namespace nau::rtti
{
    template <typename Target, typename T>
    requires(std::is_pointer_v<Target>)
    inline Target staticCast(T* const instance)
    {
        using namespace nau::rtti_detail;

        using Type = std::remove_const_t<T>;
        using TargetType = std::remove_const_t<std::remove_pointer_t<Target>>;

        if constexpr(std::is_same_v<TargetType, Type>)
        {
            return instance;
        }
        else
        {
            Target target = nullptr;
            return staticCastHelper(const_cast<Type&>(*instance), target, meta::ClassDirectBase<Type>{}) ? target : nullptr;
        }
    }

    template <typename T>
    void* runtimeCast(T& instance, const rtti::TypeInfo& targetType)
    {
        using namespace nau::rtti_detail;

        using Type = std::remove_reference_t<std::remove_const_t<T>>;

        if(targetType == rtti::getTypeInfo<IRttiObject>())
        {
            IRttiObject* const rttiObject = rtti::staticCast<IRttiObject*>(&instance);
            return reinterpret_cast<void*>(rttiObject);
        }
        else if(targetType == rtti::getTypeInfo<IRefCounted>())
        {
            IRefCounted* const refCounted = rtti::staticCast<IRefCounted*>(&instance);
            return reinterpret_cast<void*>(refCounted);
        }

        void* target = nullptr;
        return runtimeCastHelper(const_cast<Type&>(instance), targetType, &target, meta::ClassDirectBase<Type>{}) ? target : nullptr;
    }

    template <typename T>
    bool runtimeIs(const rtti::TypeInfo& targetType)
    {
        using Type = std::remove_reference_t<std::remove_const_t<T>>;
        using UniqueBases = type_list::AppendHead<meta::ClassAllUniqueBase<Type>, Type>;

        if(targetType == rtti::getTypeInfo<IRttiObject>())
        {
            return rtti_detail::isConvertibleHelper<IRttiObject>(UniqueBases{});
        }
        else if(targetType == rtti::getTypeInfo<IRefCounted>())
        {
            return rtti_detail::isConvertibleHelper<IRefCounted>(UniqueBases{});
        }

        constexpr auto runtimeIsHelper = []<typename... Base>(const rtti::TypeInfo& typeId, TypeList<Base...>)
        {
            return ((typeId == rtti::getTypeInfo<Base>()) || ...);
        };

        return runtimeIsHelper(targetType, UniqueBases{});
    }

}  // namespace nau::rtti
