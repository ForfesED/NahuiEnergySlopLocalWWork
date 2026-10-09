// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/audio/audio_service.hpp"


NAU_AUDIO_BEGIN


async::Task<> AudioService::initService()
{
    m_engine = nau::audio::IAudioEngine::create(nau::audio::IAudioEngine::Backend::Miniaudio);
    m_engine->initialize();
    return async::Task<>::makeResolved();
}

async::Task<> AudioService::shutdownService()
{
    m_engine->deinitialize();
    return async::Task<>::makeResolved();
}

IAudioEngine& AudioService::engine()
{
    return *m_engine.get();
}

NAU_AUDIO_END