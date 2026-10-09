// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/rtti/type_info.h"

#include "nau/diag/assertion.h"

namespace nau::rtti
{
    TypeIndex::TypeIndex(const TypeInfo& typeInfo) noexcept :
        m_typeInfo(&typeInfo)
    {
    }

    TypeIndex::TypeIndex(const TypeIndex&) noexcept = default;
    TypeIndex& TypeIndex::operator=(const TypeIndex&) noexcept = default;

    bool TypeIndex::operator==(const TypeIndex& other) const
    {
        NAU_ASSERT(m_typeInfo && other.m_typeInfo);

        return *m_typeInfo == *other.m_typeInfo;
    }

    std::strong_ordering TypeIndex::operator<=>(const TypeIndex& other) const
    {
        NAU_ASSERT(m_typeInfo && other.m_typeInfo);

        if(*m_typeInfo == *other.m_typeInfo)
        {
            return std::strong_ordering::equal;
        }

        return *m_typeInfo < *other.m_typeInfo ? std::strong_ordering::less : std::strong_ordering::greater;
    }

    const TypeInfo& TypeIndex::getType() const
    {
        NAU_FATAL(m_typeInfo);

        return *m_typeInfo;
    }

}  // namespace nau::rtti
