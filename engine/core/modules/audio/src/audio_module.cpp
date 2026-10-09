// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/audio/audio_common.hpp"
#include "nau/audio/audio_component_emitter.hpp"
#include "nau/audio/audio_component_listener.hpp"
#include "nau/audio/audio_service.hpp"

#include "nau/module/module.h"


NAU_AUDIO_BEGIN

// ** AudioModule

class AudioModule : public IModule
{
    string getModuleName() override;

    void initialize() override;
    void deinitialize() override;
    void postInit() override;
};


string AudioModule::getModuleName()
{
    return "nau.audio";
}

void AudioModule::initialize()
{
    NAU_MODULE_EXPORT_SERVICE(AudioService);
    NAU_MODULE_EXPORT_CLASS(AudioComponentEmitter);
    NAU_MODULE_EXPORT_CLASS(AudioComponentListener);
}

void AudioModule::deinitialize()
{

}

void AudioModule::postInit()
{

}

NAU_AUDIO_END

IMPLEMENT_MODULE(nau::audio::AudioModule)
