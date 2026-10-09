// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once
#include "nau/input_system.h"
#include "nau/service/service_provider.h"
#include "nau/scene/components/component_life_cycle.h"
#include "nau/scene/components/scene_component.h"
#include "nau/scene/scene_object.h"

namespace nau::sample
{
    class InputRotateComponent final : public scene::SceneComponent,
                                        public scene::IComponentUpdate,
                                        public scene::IComponentEvents
    {
        NAU_OBJECT(InputRotateComponent, scene::SceneComponent, scene::IComponentUpdate, scene::IComponentEvents)
        NAU_DECLARE_DYNAMIC_OBJECT

        NAU_CLASS_FIELDS(
            CLASS_NAMED_FIELD(m_factor, "Factor"))

    public:
        InputRotateComponent() = default;
        ~InputRotateComponent() = default;

        void setInputContexts(const eastl::string& context)
        {
            m_inputContext = context;
        }

    private:
        void updateComponent(float dt) override
        {
            m_timeDelta = dt;
        }

        void onComponentActivated() override
        {
            auto& insys = nau::getServiceProvider().get<nau::IInputSystem>();
            auto action = insys.addAction("WASD_Rotation", IInputAction::Type::Continuous,
            insys.createSignal("or", "gate", [&insys](IInputSignal* signalWasd)
            {
                signalWasd->addInput(insys.createSignal("key_axis", "keyboard", [](IInputSignal* signal)
                {
                    signal->Properties().set("key", eastl::string("w"));
                    signal->Properties().set("axis", 1);
                    signal->Properties().set("coeff", 1.f);
                }));
                signalWasd->addInput(insys.createSignal("key_axis", "keyboard", [](IInputSignal* signal)
                {
                    signal->Properties().set("key", eastl::string("s"));
                    signal->Properties().set("axis", 1);
                    signal->Properties().set("coeff", -1.f);
                }));
                signalWasd->addInput(insys.createSignal("key_axis", "keyboard", [](IInputSignal* signal)
                {
                    signal->Properties().set("key", eastl::string("a"));
                    signal->Properties().set("axis", 0);
                    signal->Properties().set("coeff", -1.f);
                }));
                signalWasd->addInput(insys.createSignal("key_axis", "keyboard", [](IInputSignal* signal)
                {
                    signal->Properties().set("key", eastl::string("d"));
                    signal->Properties().set("axis", 0);
                    signal->Properties().set("coeff", 1.f);
                }));
            }),
            [this](IInputSignal* onAction)
            {
                doStep(onAction->getVector3());
            });

            if (!m_inputContext.empty())
            {
                action->addContextTag(m_inputContext);
            }

            getParentObject().setRotation(math::quat::identity());
            getParentObject().setTranslation(math::vec3::zAxis() * -5.f);
        }

        void doStep(const math::vec3& offset)
        {
            math::Transform transform = getParentObject().getTransform();
            const float angle = m_timeDelta;

            if (offset.getX() != 0.f)
            {
                transform.addRotation(math::quat::rotationX(offset.getX() * m_timeDelta * m_factor));
            }
            if (offset.getY() != 0.f)
            {
                transform.addRotation(math::quat::rotationY(offset.getY() * m_timeDelta * m_factor));
            }
            if (offset.getZ() != 0.f)
            {
                transform.addRotation(math::quat::rotationZ(offset.getZ() * m_timeDelta * m_factor));
            }
            getParentObject().setTransform(transform);
        }

        float m_factor = 2.5f;
        float m_timeDelta = 0.f;
        eastl::string m_inputContext;
    };

}  // namespace nau::sample