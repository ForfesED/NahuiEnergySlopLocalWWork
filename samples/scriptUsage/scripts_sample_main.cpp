// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/app/run_application.h"
#include "nau/input.h"
#include "nau/samples/sample_app_delegate.h"
#include "nau/scene/scene_manager.h"
#include "nau/scripts/scripts.h"

namespace nau::sample
{
    class MyNativeBinding : public IRefCounted
    {
        NAU_CLASS_(MyNativeBinding, IRefCounted)
        NAU_CLASS_METHODS(
            CLASS_METHOD(MyNativeBinding, getKeyboardButtonPressed),
            CLASS_METHOD(MyNativeBinding, spawn))

    public:
        bool getKeyboardButtonPressed(input::Key key) const
        {
            return input::isKeyboardButtonPressed(0, key);
        }

        void spawn(float x, float y, float z) const
        {
            NAU_LOG("DO SPAWN at ({},{},{})", x, y, z);
        }
    };

    class MyComponent : public scene::Component,
                        public scene::IComponentEvents,
                        public scene::IComponentUpdate
    {
        NAU_COMPONENT(MyComponent, scene::Component, scene::IComponentEvents, scene::IComponentUpdate)

        void onComponentActivated() override
        {
        }

        void updateComponent(float dt) override
        {
            std::string res = *m_globalFunction(dt);
        }

    private:
        scripts::GlobalFunction<std::string(float)> m_globalFunction{"globalFunction"};
    };

    NAU_IMPLEMENT_COMPONENT(MyComponent)

    /**
     */
    class ScriptsSampleDelegate final : public SampleAppDelegate
    {
    public:
        ScriptsSampleDelegate() :
            SampleAppDelegate("scriptUsage")
        {
        }

    private:
        Result<> initializeServices() override
        {
            getServiceProvider().addClass<MyComponent>();
            return ResultSuccess;
        }

        async::Task<> startupApplication() override
        {
            using namespace nau::scripts;

            ScriptManager& scriptManager = getServiceProvider().get<ScriptManager>();
            scriptManager.registerNativeClass<MyNativeBinding>();
            scriptManager.executeScriptFromFile("MyScript1").ignore();

            auto scene = getServiceProvider().get<scene::ISceneFactory>().createEmptyScene();
            scene->getRoot().addComponent<MyComponent>();

            co_await getServiceProvider().get<scene::ISceneManager>().activateScene(std::move(scene));
        }
    };

}  // namespace nau::sample

int main(int argc, char** argv)
{
    using namespace nau;
    return runApplication(eastl::make_unique<sample::ScriptsSampleDelegate>());
}
