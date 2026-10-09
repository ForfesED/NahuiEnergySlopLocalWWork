// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "usd_ui_translator.h"
#include "../usd_prim_translator.h"
#include "nau/diag/logging.h"
#include <queue>

using namespace nau;
using namespace nau::scene;
using namespace PXR_NS;

namespace UsdTranslator
{
    UITranslator& UITranslator::setTarget(nau::ui::Canvas* scene)
    {
        m_scene = scene;
        
        return *this;
    }

    nau::ui::Canvas* UITranslator::getTarget() const
    {
        return m_scene;
    }

    UITranslator& UITranslator::setSource(PXR_NS::UsdStagePtr usdStage, const PXR_NS::SdfPath& rootPath)
    {
        m_usdStage = usdStage;
        m_rootPath = rootPath;

        return *this;
    }

    PXR_NS::UsdStagePtr UITranslator::getSource() const
    {
        return m_usdStage;
    }

    const PXR_NS::SdfPath& UITranslator::getRootPath() const
    {
        return m_rootPath;
    }

    IUIPrimAdapter::Ptr UITranslator::getRootAdapter() const
    {
        return m_rootAdapter;
    }

    UITranslator& UITranslator::follow()
    {
        m_watcher = std::make_shared<UsdProxy::StageObjectChangedWatcher>(m_usdStage, [this](PXR_NS::UsdNotice::ObjectsChanged const& notice)
            {
                std::set<IUIPrimAdapter::Ptr> updated;
                for (auto it : notice.GetResyncedPaths())
                {
                    auto targetAdapter = m_rootAdapter;
                    auto needUpdate = true;
                    auto destroy = false;
                    for (auto element : it.GetPrefixes())
                    {
                        if (element.ContainsPropertyElements())
                            break;

                        auto prim = m_usdStage->GetPrimAtPath(element);
                        if (!prim)
                            destroy = true;

                        if (auto target = targetAdapter->getChild(element.GetNameToken()))
                        {
                            if (destroy)
                            {
                                targetAdapter->destroyChild(element.GetNameToken());
                                break;
                            }
                            else
                                targetAdapter = target;
                            
                        }
                        else if(!destroy)
                        {

                            initSceneObjects(prim, targetAdapter->getNode(), targetAdapter);
                            needUpdate = false;
                            break;
                        }
                    }

                    if(!targetAdapter || !needUpdate || destroy)
                        continue;

                    std::queue<IUIPrimAdapter::Ptr> next;
                    next.push(targetAdapter);
                    while (!next.empty())
                    {
                        auto adapter = next.front();
                        next.pop();
                        if(updated.find(adapter) == updated.end())
                        {
                            adapter->update();
                            updated.emplace(adapter);
                        }
                        for (auto child : adapter->getChildren())
                            next.push(child.second);
                    }
                }

                for (auto it : notice.GetChangedInfoOnlyPaths())
                {
                    auto targetAdapter = m_rootAdapter;
                    for (auto element : it.GetPrefixes())
                    {
                        if (element.ContainsPropertyElements())
                            break;

                        targetAdapter = targetAdapter->getChild(element.GetNameToken());
                        if (!targetAdapter)
                            break;                                               
                    }

                    if (!targetAdapter)
                        continue;

                    std::queue<IUIPrimAdapter::Ptr> next;
                    next.push(targetAdapter);
                    while (!next.empty())
                    {
                        auto adapter = next.front();
                        next.pop();
                        if (updated.find(adapter) == updated.end())
                        {
                            adapter->update();
                            updated.emplace(adapter);
                        }
                        for (auto child : adapter->getChildren())
                            next.push(child.second);
                    }
                }
            });
        return *this;
    }

    void UITranslator::initSceneObjects(PXR_NS::UsdPrim prim, nau::ui::Node* dest, IUIPrimAdapter::Ptr& parent)
    {
        auto adapter = UsdTranslator::PrimTranslator::instance().createUIAdapter(prim);
        if(!adapter)
        {
            NAU_LOG_ERROR("Failed to create USD UI adapter for primitive {}", prim.GetPath().GetString().c_str());
            return;
        }

        auto next = adapter->initializeNode();
        if (!parent)
        {
            parent = adapter;
            dest->addChild(next);
        }
        else
        {
            parent->addChild(prim.GetName(), adapter);
            parent->addChildInternal(next);
        }

        for (auto child : prim.GetAllChildren())
            initSceneObjects(child, next ? next : dest, adapter);
    }

    void UITranslator::initScene()
    {
        initSceneObjects(getSceneRoot(), m_scene, m_rootAdapter);
    }

    void UITranslator::loadSceneTree(PXR_NS::UsdPrim prim, IUIPrimAdapter::Ptr& parent)
    {
        auto adapter = UsdTranslator::PrimTranslator::instance().createUIAdapter(prim);
        if(!adapter)
        {
            NAU_LOG_ERROR("Failed to create USD UI adapter for primitive {}", prim.GetPath().GetString().c_str());
            return;
        }

        if (!parent)
        {
            parent = adapter;
        }
        else
        {
            parent->addChild(prim.GetName(), adapter);
        }

        for (auto child : prim.GetAllChildren())
        {
            loadSceneTree(child, adapter);
        }
    }

    void UITranslator::initSceneDataOnly()
    {
        loadSceneTree(getSceneRoot(), m_rootAdapter);
    }

    PXR_NS::UsdPrim UITranslator::getSceneRoot()
    {
        return m_rootPath.IsEmpty()
            ? m_usdStage->GetPseudoRoot()
            : m_usdStage->GetPrimAtPath(m_rootPath);
    }
}
