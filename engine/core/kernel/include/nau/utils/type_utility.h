// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include <type_traits>

namespace nau
{

    namespace rt_detail
    {

        /**
         */
        template <template <typename... X> class TemplatedClass, typename T>
        struct IsTemplateOfHelper
        {
            static std::false_type helper(...);

            template <typename... U>
            static std::true_type helper(const TemplatedClass<U...>&);

            constexpr static bool value = decltype(helper(std::declval<T>()))::value;
        };

        /**
         */
        template <template <typename...> class TemplatedClass>
        struct IsTemplateOfHelper<TemplatedClass, void>
        {
            constexpr static bool value = false;
        };

    }  // namespace rt_detail

    /**
     */
    struct ConstIndex
    {
        static constexpr inline int NotIndex = -1;

        const int value;

        constexpr ConstIndex() :
            value(NotIndex)
        {
        }

        constexpr ConstIndex(int i) :
            value(i)
        {
        }

        constexpr operator size_t() const
        {
            return static_cast<size_t>(value);
        }

        constexpr operator bool() const
        {
            return value >= 0;
        }

        constexpr ConstIndex operator||(ConstIndex other) const
        {
            return value >= 0 ? *this : other;
        }
    };

    /**
     * @brief
     * Tell that type is an instantiation of base template class.
     */
    template <template <typename...> class TemplateClass, typename T>
    inline constexpr bool IsTemplateOf = rt_detail::IsTemplateOfHelper<TemplateClass, std::remove_const_t<std::remove_reference_t<T>>>::value;

    template <typename T, template <typename...> class TemplateClass>
    concept TemplateOfConcept = IsTemplateOf<TemplateClass, T>;

    template <typename T>
    std::add_lvalue_reference_t<T> lValueRef();

    template <typename T>
    std::add_rvalue_reference_t<T> rValueRef();

    template <typename T>
    std::add_const_t<std::add_lvalue_reference_t<T>> constLValueRef();

    template <typename T, typename... U>
    inline constexpr bool AnyOf = (std::is_same_v<T, U> || ...);

    // In (msvc) std can not using std::aligned_storage<> with align great than alignof(max_align_t)
    template <size_t Size, size_t Align>
    struct AlignedStorage
    {
        alignas(Align) char space[Size];
    };

}  // namespace nau
