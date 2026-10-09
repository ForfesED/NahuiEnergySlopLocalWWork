// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <nau/utils/typed_flag.h>
//#include "tracy/Tracy.hpp"

namespace nau
{
	enum class PerfTag : unsigned 
	{ 
		Core = NauFlag(1), 
		Physics = NauFlag(2), 
		Render = NauFlag(3),
		Platform = NauFlag(4)
	}; 
  
	NAU_DEFINE_TYPED_FLAG(PerfTag)
}

static const nau::PerfTagFlag NAU_PERFTAGS = nau::PerfTagFlag{nau::PerfTag::Core, nau::PerfTag::Physics, nau::PerfTag::Render, nau::PerfTag::Platform};

#define NAU_CPU_SCOPED ZoneScoped
#define NAU_CPU_SCOPED_NAME ZoneScopedN

#define NAU_CPU_SCOPED_TAG(TagName) ZoneNamed(__tracy, NAU_PERFTAGS.has(TagName));
#define NAU_CPU_SCOPED_TAG_NAME(Name, TagName) ZoneNamedN(__tracy, Name, NAU_PERFTAGS.has(TagName));

#define NAU_PROFILING_FRAME_END FrameMark
