// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/meta/runtime_attribute.h"

#include <EASTL/algorithm.h>
#include <EASTL/sort.h>

#include "nau/diag/assertion.h"
#include "nau/meta/attribute.h"

namespace nau::meta
{
    void RuntimeAttributeContainer::setupUniqueKeys()
    {
        if (m_attributes.empty())
        {
            return;
        }

        m_uniqueKeys.reserve(m_attributes.size());
        eastl::transform(m_attributes.begin(), m_attributes.end(), eastl::back_inserter(m_uniqueKeys), [](const AttributeEntry& entry)
        {
            return entry.first;
        });

        eastl::sort(m_uniqueKeys.begin(), m_uniqueKeys.end());
        m_uniqueKeys.erase(eastl::unique(m_uniqueKeys.begin(), m_uniqueKeys.end()), m_uniqueKeys.end());
    }

    size_t RuntimeAttributeContainer::getSize() const
    {
        return m_uniqueKeys.size();
    }

    bool RuntimeAttributeContainer::containsAttribute(eastl::string_view key) const
    {
        return eastl::find(m_uniqueKeys.begin(), m_uniqueKeys.end(), key) != m_uniqueKeys.end();
    }

    eastl::string_view RuntimeAttributeContainer::getKey(size_t index) const
    {
        NAU_FATAL(index < m_uniqueKeys.size());
        return m_uniqueKeys[index];
    }

    RuntimeValue::Ptr RuntimeAttributeContainer::getValue(eastl::string_view attributeKey) const
    {
        auto value = eastl::find_if(m_attributes.begin(), m_attributes.end(), [attributeKey](const AttributeEntry& entry)
        {
            return entry.first == attributeKey;
        });

        return value != m_attributes.end() ? value->second : nullptr;
    }

    Vector<RuntimeValue::Ptr> RuntimeAttributeContainer::getAllValues(eastl::string_view key) const
    {
        Vector<RuntimeValue::Ptr> values;

        for (const auto& [attribKey, attribValue] : m_attributes)
        {
            if (attribKey == key)
            {
                values.push_back(attribValue);
            }
        }

        return values;
    }

}  // namespace nau::meta