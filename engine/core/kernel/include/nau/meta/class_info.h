// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <concepts>
#include <string_view>
#include <type_traits>

#include "nau/meta/function_info.h"
#include "nau/utils/tuple_utility.h"
#include "nau/utils/type_list/append.h"
#include "nau/utils/type_list/concat.h"
#include "nau/utils/type_list/distinct.h"
#include "nau/utils/type_list/filter.h"
#include "nau/utils/type_utility.h"

namespace nau::meta
{

    /**
     */
    template <typename... T>
    struct ReflectClassBase
    {
        using type = TypeList<T...>;
    };

    template <typename, typename...>
    class FieldInfo;

    /**

    */
    template <typename ClassT, typename T, typename... Attribs>
    class FieldInfo<T ClassT::*, Attribs...>
    {
    public:
        using Class = ClassT;
        using ValueType = std::remove_cv_t<T>;
        using FieldPointer = T Class::*;
        using Attributes = std::tuple<Attribs...>;

        template <typename A>
        static constexpr inline bool HasAttribute = TupleUtils::template contains<Attributes, A>();

        FieldInfo(FieldPointer field, const char name[], Attribs... attributes) :
            m_field(field),
            m_name(name),
            m_attributes{std::move(attributes)...}
        {
        }

        constexpr bool isConst() const
        {
            return std::is_const_v<T>;
        }

        decltype(auto) getValue(Class& instance) const
        {
            return (instance.*m_field);
        }

        decltype(auto) getValue(const Class& instance) const
        {
            return (instance.*m_field);
        }

        std::string_view getName() const
        {
            return m_name;
        }

        decltype(auto) getAttributes() const
        {
            return (m_attributes);
        }

        template <typename Attribute>
        constexpr bool fieldHasAttribute() const
        {
            return FieldInfo<FieldPointer, Attribs...>::template HasAttribute<Attribute>;
        }

    private:
        FieldPointer m_field;
        std::string_view m_name;
        Attributes m_attributes;
    };

    template <typename Class, typename T, typename... Attribs>
    FieldInfo(T Class::*, std::string_view, Attribs...) -> FieldInfo<T Class::*, Attribs...>;

    /**
     */
    template <typename... Field>
    class ReflectClassFields
    {
    public:
        using FieldsTuple = std::tuple<Field...>;

        constexpr ReflectClassFields(Field&&... field) :
            m_fields{std::move(field)...}
        {
        }

        const FieldsTuple& getFields() const
        {
            return (m_fields);
        }

    private:
        const FieldsTuple m_fields;
    };

    template <typename... Field>
    ReflectClassFields(Field&&...) -> ReflectClassFields<Field...>;

    /*

     */
    template <auto F, typename... Attributes>
    class MethodInfo
    {
    public:
        using FunctionType = decltype(F);
        using FunctionTypeInfo = meta::GetCallableTypeInfo<FunctionType>;
        using AttributesTuple = std::tuple<Attributes...>;

        inline constexpr static bool IsMemberFunction = std::is_member_function_pointer_v<FunctionType>;
        inline constexpr static FunctionType Func = F;

        constexpr MethodInfo(std::string_view name, Attributes... attributes) :
            m_name(name),
            m_attributes{std::move(attributes)...}
        {
        }

        constexpr auto getFunction() const
        {
            return F;
        }

        std::string_view getName() const
        {
            return m_name;
        }

        const AttributesTuple& getAttributes() const
        {
            return m_attributes;
        }

        template <typename A>
        static consteval bool hasAttribute()
        {
            return TupleUtils::contains<AttributesTuple, A>();
        }

        template <typename A>
        decltype(auto) getAttribute() const
        {
            static_assert(MethodInfo::hasAttribute<A>(), "Method does not contains requested attribute, use if constexpr (MethodInfoType::hasAttribute<A>())");
            return std::get<A>(m_attributes);
        }

    private:
        std::string_view m_name;
        AttributesTuple m_attributes;
    };

    /*

    */
    template <typename... Method>
    class ReflectClassMethods
    {
    public:
        using MethodsTuple = std::tuple<Method...>;

        ReflectClassMethods(Method&&... method) :
            m_methods{std::move(method)...}
        {
        }

        const MethodsTuple& getMethods() const
        {
            return (m_methods);
        }

    private:
        const MethodsTuple m_methods;
    };

    template <typename... Method>
    ReflectClassMethods(Method&&...) -> ReflectClassMethods<Method...>;

}  // namespace nau::meta

namespace nau::meta_detail
{
    /**
     */
    template <typename T>
    concept Concept_ReflectClassBase = requires {
        typename T::Nau_ClassBase;
    } && IsTemplateOf<meta::ReflectClassBase, typename T::Nau_ClassBase>;

    /**
     */
    template <typename T>
    concept Concept_ReflectClassFields = requires(const T& instance) {
        { instance.template nauGetClassFields<T>() } -> TemplateOfConcept<meta::ReflectClassFields>;
    };

    /**
     */
    template <typename T>
    concept Concept_ReflectClassMethods = requires {
        { T::nauGetClassMethods() } -> TemplateOfConcept<meta::ReflectClassMethods>;
    };

    /**
     */
    template <typename T, bool = Concept_ReflectClassBase<T>>
    struct ClassDirectBase
    {
        using type = TypeList<>;
    };

    template <typename T>
    struct ClassDirectBase<T, true>
    {
        using type = typename T::Nau_ClassBase::type;
    };

    /**
     */
    template <typename T, typename Base = typename ClassDirectBase<T>::type>
    struct ClassAllBase;

    template <typename T, typename... Base>
    struct ClassAllBase<T, TypeList<Base...>>
    {
        using type = type_list::Concat<TypeList<Base...>, typename ClassAllBase<Base>::type...>;
    };

    template <typename T>
    using ClassAllUniqueBase = type_list::Distinct<typename ClassAllBase<T>::type>;

    /**
     */
    template <typename T>
    inline decltype(auto) getClassDirectFields()
    {
        using Type = std::remove_cv_t<T>;

        if constexpr (Concept_ReflectClassFields<Type>)
        {
            // The hackish trick with access to the class method through null this.
            // It is necessary to distinguish calls through inheritor classes that do not override the method nauGetClassFields().
            const Type* const nullInstance = nullptr;
            return nullInstance->template nauGetClassFields<Type>().getFields();
        }
        else
        {
            return std::tuple{};
        }
    }

    template <typename T>
    inline auto getClassAllFields()
    {
        const auto classFieldsHelper = []<typename... U>(TypeList<U...>)
        {
            return std::tuple_cat(getClassDirectFields<U>()...);
        };

        using Types = type_list::Append<ClassAllUniqueBase<T>, T>;

        return classFieldsHelper(Types{});
    }

    template <typename T>
    inline decltype(auto) getClassDirectMethods()
    {
        using Type = std::remove_cv_t<T>;

        if constexpr (Concept_ReflectClassMethods<Type>)
        {
            return Type::nauGetClassMethods().getMethods();
        }
        else
        {
            return std::tuple{};
        }
    }

    template <typename T>
    inline auto getClassAllMethods()
    {
        const auto classMethodsHelper = []<typename... U>(TypeList<U...>)
        {
            return std::tuple_cat(getClassDirectMethods<U>()...);
        };

        using Types = type_list::Append<ClassAllUniqueBase<T>, T>;

        return classMethodsHelper(Types{});
    }

    template <auto F>
    struct MethodInfoFactory
    {
        const std::string_view name;

        MethodInfoFactory(const char inName[]) :
            name(inName)
        {
        }

        template <typename... Attributes>
        auto operator()(Attributes&&... attribs) const
        {
            return ::nau::meta::MethodInfo<F, Attributes...>{name, std::forward<Attributes>(attribs)...};
        }
    };

    template <typename... T>
    consteval bool anyHasFields(TypeList<T...>)
    {
        return (Concept_ReflectClassFields<T> || ...);
    }

    template <typename... T>
    consteval bool anyHasMethods(TypeList<T...>)
    {
        return (Concept_ReflectClassMethods<T> || ...);
    }

}  // namespace nau::meta_detail

namespace nau::meta
{
    template <typename T>
    using ClassDirectBase = typename ::nau::meta_detail::ClassDirectBase<T>::type;

    template <typename T>
    using ClassAllBase = typename ::nau::meta_detail::ClassAllBase<T>::type;

    template <typename T>
    using ClassAllUniqueBase = ::nau::meta_detail::ClassAllUniqueBase<T>;

    template <typename T>
    inline constexpr bool ClassHasFields = ::nau::meta_detail::Concept_ReflectClassFields<T> || ::nau::meta_detail::anyHasFields(ClassAllUniqueBase<T>{});

    template <typename T>
    inline constexpr bool ClassHasMethods = ::nau::meta_detail::Concept_ReflectClassMethods<T> || ::nau::meta_detail::anyHasMethods(ClassAllUniqueBase<T>{});

    template <typename T>
    inline decltype(auto) getClassDirectFields()
    {
        return ::nau::meta_detail::getClassDirectFields<T>();
    }

    template <typename T>
    inline decltype(auto) getClassAllFields()
    {
        return ::nau::meta_detail::getClassAllFields<T>();
    }

    template <typename T>
    inline decltype(auto) getClassDirectMethods()
    {
        return ::nau::meta_detail::getClassDirectMethods<T>();
    }

    template <typename T>
    inline decltype(auto) getClassAllMethods()
    {
        return ::nau::meta_detail::getClassAllMethods<T>();
    }

}  // namespace nau::meta


#define NAU_CLASS_BASE(...) \
public:                     \
    using Nau_ClassBase = ::nau::meta::ReflectClassBase<__VA_ARGS__>;

#define CLASS_NAMED_FIELD(field, name, ...)    \
    nau::meta::FieldInfo                       \
    {                                          \
        &InstanceT__::field, name, __VA_ARGS__ \
    }

#define CLASS_FIELD(field, ...) CLASS_NAMED_FIELD(field, #field, __VA_ARGS__)

#define NAU_CLASS_FIELDS(...)                                                           \
public:                                                                                 \
    template <typename InstanceT__>                                                     \
    decltype(auto) nauGetClassFields() const                                            \
    {                                                                                   \
        using ThisType = std::remove_const_t<std::remove_reference_t<decltype(*this)>>; \
        if constexpr (std::is_same_v<InstanceT__, ThisType>)                            \
        {                                                                               \
            static const ::nau::meta::ReflectClassFields classFields{                   \
                __VA_ARGS__};                                                           \
                                                                                        \
            return (classFields);                                                       \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            return unsigned{0};                                                         \
        }                                                                               \
    }

#define CLASS_NAMED_METHOD(Class, method, name, ...) ::nau::meta_detail::MethodInfoFactory<&Class::method>(name)(__VA_ARGS__)

#define CLASS_METHOD(Class, method, ...) ::nau::meta_detail::MethodInfoFactory<&Class::method>(#method)(__VA_ARGS__)

#define NAU_CLASS_METHODS(...)                                         \
public:                                                                \
    static decltype(auto) nauGetClassMethods()                         \
    {                                                                  \
        static const ::nau::meta::ReflectClassMethods classMethods = { \
            __VA_ARGS__};                                              \
                                                                       \
        return (classMethods);                                         \
    }
