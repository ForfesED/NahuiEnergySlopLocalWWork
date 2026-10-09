// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/scene/components/internal/missing_component.h"

#include "nau/scene/components/component_attributes.h"
#include "nau/scene/components/scene_component.h"
#include "nau/serialization/json.h"

namespace nau::scene
{
    /**
        @brief MissingComponent internal implementation. Expected be as SceneComponent
     */
    class MissingComponent final : public SceneComponent,
                                   public IMissingComponent
    {
        NAU_COMPONENT(nau::scene::MissingComponent, SceneComponent, IMissingComponent)

        NAU_CLASS_ATTRIBUTES(
            CLASS_ATTRIBUTE(HiddenAttributeAttr, true))
    private:
        void setComponentData(const ComponentAsset& componentData) override
        {
            m_componentData.componentTypeId = componentData.componentTypeId;
            m_componentData.uid = componentData.uid;
            m_componentData.transform = componentData.transform;
            m_componentData.properties = serialization::jsonCreateDictionary();

            RuntimeValue::assign(m_componentData.properties, componentData.properties).ignore();
        }

        void fillComponentData(ComponentAsset& componentData) override
        {
            componentData = m_componentData;
        }

        ComponentAsset m_componentData;
    };

    NAU_IMPLEMENT_COMPONENT(MissingComponent)

    NAU_CORESCENE_EXPORT ObjectUniquePtr<Component> createDefaultMissingComponent()
    {
        return NauObject::classCreateInstance<MissingComponent>();
    }
}  // namespace nau::scene
