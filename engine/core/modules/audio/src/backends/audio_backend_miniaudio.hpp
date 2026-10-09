// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once 

#include "nau/audio/audio_engine.hpp"

#include <memory>


NAU_AUDIO_BEGIN

// ** AudioEngineMiniaudio

class AudioEngineMiniaudio : public IAudioEngine
{
public:
	AudioEngineMiniaudio();
	~AudioEngineMiniaudio();

	void initialize() override;
	void deinitialize() override;
	void update() override;

	AudioAssetPtr loadSound(const eastl::string& path) override;
	AudioAssetPtr loadStream(const eastl::string& path) override;
	
	AudioAssetList audioAssets() override;

private:
	class Impl;
	std::unique_ptr<Impl> m_pimpl;
};

NAU_AUDIO_END
