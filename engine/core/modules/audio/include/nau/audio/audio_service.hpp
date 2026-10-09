// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "nau/audio/audio_common.hpp"
#include "nau/service/service.h"

#include "nau/audio/audio_engine.hpp"


NAU_AUDIO_BEGIN

class NAU_AUDIO_EXPORT AudioService final
    : public IServiceInitialization
    , public IServiceShutdown
{
    NAU_RTTI_CLASS(AudioService, IServiceInitialization, IServiceShutdown)

public:
    async::Task<> initService() override;
    async::Task<> shutdownService() override;

    IAudioEngine& engine();

private:
    AudioEnginePtr m_engine;
};

NAU_AUDIO_END
