// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include "usd_proxy/usd_proxy_api.h"

#include <functional>

#include <pxr/base/tf/weakBase.h>
#include <pxr/pxr.h>
#include <pxr/usd/usd/notice.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/sdf/notice.h>
#include <pxr/usd/sdf/layer.h>


namespace UsdProxy
{

    typedef std::function<void(PXR_NS::UsdNotice::ObjectsChanged const& notice)> StageObjectChangedWatcherCallback;
    class USD_PROXY_API StageObjectChangedWatcher : public PXR_NS::TfWeakBase
    {
    public:
        StageObjectChangedWatcher(const PXR_NS::UsdStageRefPtr& stage, StageObjectChangedWatcherCallback cbFunc);
        virtual ~StageObjectChangedWatcher();

        void blockNotifications(bool enable);
    private:
        void onObjectsChanged(PXR_NS::UsdNotice::ObjectsChanged const& notice, PXR_NS::UsdStageWeakPtr const& sender);

        PXR_NS::UsdStageRefPtr m_stage;
        PXR_NS::TfNotice::Key m_objectChangeKey;
        StageObjectChangedWatcherCallback m_cb;
        bool m_blockNotifications = false;
    };

    typedef std::function<void(PXR_NS::UsdNotice::StageEditTargetChanged const& notice)> StageEditTargetChangedWatcherCallback;
    class USD_PROXY_API StageEditTargetChangedWatcher : public PXR_NS::TfWeakBase
    {
    public:
        StageEditTargetChangedWatcher(const PXR_NS::UsdStageRefPtr& stage, StageEditTargetChangedWatcherCallback cbFunc);
        virtual ~StageEditTargetChangedWatcher();

    private:
        void onStageEditTargetChanged(PXR_NS::UsdNotice::StageEditTargetChanged const& notice, PXR_NS::UsdStageWeakPtr const& sender);

        PXR_NS::UsdStageRefPtr m_stage;
        PXR_NS::TfNotice::Key m_editTargetChangeKey;
        StageEditTargetChangedWatcherCallback m_cb;
    };

    typedef std::function<void(PXR_NS::SdfNotice::LayerDirtinessChanged const& notice)> SdfLayerDirtinessChangedWatcherCallback;
    class USD_PROXY_API SdfLayerDirtinessChangedWatcher : public PXR_NS::TfWeakBase
    {
    public:
        SdfLayerDirtinessChangedWatcher(PXR_NS::SdfLayerHandle& layer, SdfLayerDirtinessChangedWatcherCallback cbFunc);
        virtual ~SdfLayerDirtinessChangedWatcher();
    private:
        void onChangeNotice(const PXR_NS::SdfNotice::LayerDirtinessChanged& notice, const PXR_NS::SdfLayerHandle& sender);
        PXR_NS::SdfLayerHandle m_layer;
        PXR_NS::TfNotice::Key m_layerDirtyChangeKey;
        SdfLayerDirtinessChangedWatcherCallback m_cb;
    };
}