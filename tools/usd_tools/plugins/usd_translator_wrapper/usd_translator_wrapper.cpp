// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "pxr/pxr.h"
#include "usd_translator_wrapper.h"
#include <usd_translator/ui_translator/usd_ui_translator.h>

void translateScene(PXR_NS::UsdStageRefPtr stage, nau::scene::IScene::WeakRef scene)
{
    UsdTranslator::StageTranslator translator;

    translator.setSource(stage);
    translator.setTarget(scene);

    auto task = translator.initScene();
    nau::async::wait(task);
}


void translateUIScene(PXR_NS::UsdStageRefPtr stage, nau::DataBlock& blk)
{
    UsdTranslator::UITranslator translator{};
    translator.setSource(stage);
    translator.initSceneDataOnly();
    auto adapter = translator.getRootAdapter();
    adapter->serializeToBlk(blk);
}
