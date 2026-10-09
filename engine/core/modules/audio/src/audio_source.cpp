// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/audio/audio_source.hpp"

NAU_AUDIO_BEGIN

void IAudioSource::playNext(AudioSourcePtr next)
{
    setEndCallback([_next = std::weak_ptr<IAudioSource>(next)] {
        if (auto next = _next.lock(); next) {
            next->play();
        }
    });
}

void IAudioSource::rewind()
{
    seek(std::chrono::milliseconds(0));
}


NAU_AUDIO_END
