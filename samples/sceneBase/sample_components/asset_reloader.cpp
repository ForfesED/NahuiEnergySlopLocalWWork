// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "./asset_reloader.h"

#include "nau/app/global_properties.h"
#include "nau/assets/asset_manager.h"
#include "nau/service/service_provider.h"

namespace nau::sample
{
    NAU_IMPLEMENT_DYNAMIC_OBJECT(AssetReloader)

    void AssetReloader::updateComponent(float dt)
    {
        using namespace nau::scene;

        ImGui::Begin("Demo");
        ImGui::SetWindowSize(ImVec2(150, 160), ImGuiCond_Once);

        if (m_assetPaths.empty())
        {
            return;
        }

        ImGui::Text("Asset re-load:");

        if (ImGui::BeginCombo("Asset:", m_assetPaths[m_currentSelection].c_str()))
        {
            for (size_t i = 0; i < m_assetPaths.size(); ++i)
            {
                if (ImGui::Selectable(m_assetPaths[i].c_str(), m_currentSelection == i))
                {
                    m_currentSelection = i;
                }
            }

            ImGui::EndCombo();
        }

        const auto& assetPath = m_assetPaths[m_currentSelection];
        IAssetDescriptor::Ptr asset = getServiceProvider().get<IAssetManager>().openAsset(AssetPath{assetPath});
        const IAssetDescriptor::LoadState state = asset->getLoadState();

        if (state == IAssetDescriptor::LoadState::None)
        {
            if (ImGui::Button("Do Load"))
            {
                NAU_LOG("Will load asset:({})", assetPath);
                asset->load();
            }
        }
        else if (state == IAssetDescriptor::LoadState::Ready)
        {
            if (ImGui::Button("Do Unload"))
            {
                NAU_LOG("Will unload asset:({})", assetPath);
                asset->unload();
            }
        }
        else
        {
            ImGui::Text("Asset is not ready");
        }

        ImGui::End();
    }

    void AssetReloader::onComponentActivated()
    {
        GlobalProperties& globalProps = getServiceProvider().get<GlobalProperties>();
        auto paths = globalProps.getValue<eastl::vector<eastl::string>>("sample/assetsToReload");
        if (paths.has_value())
        {
            std::move(paths->begin(), paths->end(), std::back_inserter(m_assetPaths));
        }
    }

    void AssetReloader::setAssetPath(eastl::string_view assetPath)
    {
        value_changes_scope;

        m_assetPaths.emplace_back(assetPath);
    }

}  // namespace nau::sample
