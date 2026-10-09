// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "usd_stage_translator.h"

#include <queue>

#include "usd_prim_translator.h"

using namespace nau;
using namespace nau::scene;
using namespace PXR_NS;

namespace UsdTranslator
{
    StageTranslator::StageTranslator()
    {
    }

    StageTranslator::~StageTranslator()
    {
    }

    StageTranslator& StageTranslator::setTarget(nau::scene::IScene::WeakRef scene)
    {
        m_scene = scene;

        return *this;
    }

    nau::scene::IScene::WeakRef StageTranslator::getTarget() const
    {
        return m_scene;
    }

    StageTranslator& StageTranslator::setSource(PXR_NS::UsdStagePtr usdStage, const PXR_NS::SdfPath& rootPath)
    {
        m_usdStage = usdStage;
        m_rootPath = rootPath;

        return *this;
    }

    PXR_NS::UsdStagePtr StageTranslator::getSource() const
    {
        return m_usdStage;
    }

    const PXR_NS::SdfPath& StageTranslator::getRootPath() const
    {
        return m_rootPath;
    }

    IPrimAdapter::Ptr StageTranslator::getRootAdapter() const
    {
        return m_rootAdapter;
    }

    nau::async::Task<> StageTranslator::forceUpdate(PXR_NS::UsdPrim prim)
    {
        auto targetAdapter = m_rootAdapter;

        for (auto element : prim.GetPath().GetPrefixes())
        {
            if (element.ContainsPropertyElements())
            {
                break;
            }
            targetAdapter = targetAdapter->getChild(element.GetNameToken());
            if (!targetAdapter)
            {
                co_return;
            }
        }

        if (!targetAdapter)
        {
            co_return;
        }

        std::queue<IPrimAdapter::Ptr> next;
        next.push(targetAdapter);
        while (!next.empty())
        {
            auto adapter = next.front();
            next.pop();
            co_await adapter->update();
            for (auto child : adapter->getChildren())
            {
                next.push(child.second);
            }
        }

        co_return;
    }

    StageTranslator& StageTranslator::follow()
    {
        // NOTE: Since the scene operation is asynchronous, we process changes in two stages.
        // First stage, we collect all changes received from the notice.
        // The second stage, we start all asynchronous tasks one by one.
        using namespace nau;
        using namespace nau::async;

        auto taskWrapper = [this](PXR_NS::UsdNotice::ObjectsChanged const& n)
        {
            m_watchTask = [](StageTranslator& self, PXR_NS::UsdNotice::ObjectsChanged const& notice, Task<> previousTask) -> Task<>
            {
                std::set<IPrimAdapter::Ptr> updated;
                std::vector<SdfPath> resyncedPaths;
                std::vector<SdfPath> changedInfoOnlyPaths;

                auto createFunc = [&self](auto prim, IPrimAdapter::Ptr& targetAdapter, bool createChildren) -> Task<>
                {
                    if (!createChildren)
                    {
                        co_await self.initSceneObjects(prim, *targetAdapter->getSceneObject(), targetAdapter);
                        co_return;
                    }

                    std::queue<std::pair<UsdPrim, IPrimAdapter::Ptr>> prims;
                    prims.push({prim, targetAdapter});

                    while (!prims.empty())
                    {
                        auto [topPrim, targetAdapter] = prims.front();
                        prims.pop();

                        for (auto prim : topPrim.GetAllChildren())
                        {
                            if (auto target = targetAdapter->getChild(prim.GetName()))
                            {
                                prims.push({prim, target});
                                continue;
                            }

                            co_await self.initSceneObjects(prim, *targetAdapter->getSceneObject(), targetAdapter);
                        }
                    }
                };

                auto updateFunc = [&updated](auto targetAdapter) -> Task<>
                {
                    std::queue<IPrimAdapter::Ptr> next;
                    next.push(targetAdapter);
                    while (!next.empty())
                    {
                        auto adapter = next.front();
                        next.pop();
                        if (updated.find(adapter) == updated.end())
                        {
                            co_await adapter->update();
                            updated.emplace(adapter);
                        }
                        for (auto child : adapter->getChildren())
                        {
                            next.push(child.second);
                        }
                    }
                };

                for (auto it : notice.GetResyncedPaths())
                {
                    resyncedPaths.emplace_back(it);
                }
                for (auto it : notice.GetChangedInfoOnlyPaths())
                {
                    changedInfoOnlyPaths.emplace_back(it);
                }

                if (previousTask)
                {
                    co_await previousTask;
                }

                for (auto it : resyncedPaths)
                {
                    auto targetAdapter = self.m_rootAdapter;
                    bool needUpdate = false;
                    for (auto element : it.GetPrefixes())
                    {
                        if (element.ContainsPropertyElements())
                        {
                            break;
                        }

                        auto prim = self.m_usdStage->GetPrimAtPath(element);
                        auto target = targetAdapter->getChild(element.GetNameToken());
                        if (prim && !target)
                        {
                            co_await createFunc(prim, targetAdapter, false);
                            needUpdate = false;
                            break;
                        }
                        else if (!prim && target)
                        {
                            targetAdapter->destroyChild(element.GetNameToken());
                            needUpdate = false;
                            break;
                        }
                        else
                        {
                            TfToken newAdapterType = PrimTranslator::instance().findAdapterType(prim);
                            if (target->getType() != newAdapterType.GetString())
                            {
                                auto newAdapter = PrimTranslator::instance().createAdapter(prim);
                                targetAdapter->destroyChild(prim.GetName());
                                targetAdapter->addChild(prim.GetName(), newAdapter);

                                auto next = co_await newAdapter->initializeSceneObject(*targetAdapter->getSceneObject());
                                for (auto it : prim.GetAllChildren())
                                {
                                    co_await self.initSceneObjects(it, next ? *next : *targetAdapter->getSceneObject(), targetAdapter);
                                }
                                needUpdate = false;
                                break;
                            }

                            targetAdapter = target;
                            needUpdate = true;
                        }
                    }

                    if (!needUpdate)
                    {
                        continue;
                    }

                    NAU_ASSERT(targetAdapter);
                    co_await updateFunc(targetAdapter);
                    co_await createFunc(self.m_usdStage->GetPrimAtPath(it.GetPrimPath()), targetAdapter, true);
                }

                for (auto it : changedInfoOnlyPaths)
                {
                    auto targetAdapter = self.m_rootAdapter;
                    for (auto element : it.GetPrefixes())
                    {
                        if (element.ContainsPropertyElements())
                        {
                            break;
                        }

                        targetAdapter = targetAdapter->getChild(element.GetNameToken());
                        if (!targetAdapter)
                        {
                            // TODO: Is that possible?
                            NAU_ASSERT("Changes to this adapter are not possible because this adapter does not exist");
                            break;
                        }
                    }

                    if (!targetAdapter)
                    {
                        continue;
                    }

                    co_await updateFunc(targetAdapter);
                }
            }(*this, n, std::move(m_watchTask));
        };

        NAU_ASSERT(!m_watcher);
        m_watcher = std::make_shared<UsdProxy::StageObjectChangedWatcher>(m_usdStage, taskWrapper);
        return *this;
    }

    nau::async::Task<> StageTranslator::initSceneObjects(UsdPrim prim, nau::scene::ObjectWeakRef<nau::scene::SceneObject> dest, IPrimAdapter::Ptr& parent)
    {
        auto adapter = PrimTranslator::instance().createAdapter(prim);
        if (!parent)
        {
            parent = adapter;
        }
        else
        {
            parent->addChild(prim.GetName(), adapter);
        }

        auto next = co_await adapter->initializeSceneObject(dest);
        for (auto child : prim.GetAllChildren())
        {
            co_await initSceneObjects(child, next ? *next : dest, adapter);
        }
    }

    nau::async::Task<> StageTranslator::initScene()
    {
        auto& sceneRoot = m_scene->getRoot();
        auto rootPrim = m_rootPath.IsEmpty() ? m_usdStage->GetPseudoRoot() : m_usdStage->GetPrimAtPath(m_rootPath);
        co_await initSceneObjects(rootPrim, sceneRoot, m_rootAdapter);
    }

}  // namespace UsdTranslator
