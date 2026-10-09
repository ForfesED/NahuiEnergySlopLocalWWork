// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/dataBlock/dag_dataBlock.h"
#include "pxr/pxr.h"
#include <usd_translator/usd_stage_translator.h>
#include "usd_translator_wrapper_api.h"

extern "C"
{
    USD_TRANSLATOR_WRAPPER_API void translateScene(PXR_NS::UsdStageRefPtr stage, nau::scene::IScene::WeakRef scene);
    USD_TRANSLATOR_WRAPPER_API void translateUIScene(PXR_NS::UsdStageRefPtr stage, nau::DataBlock& blk);
}
