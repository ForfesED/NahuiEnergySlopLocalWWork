// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "graphics_assets/material_asset.h"
#include "graphics_assets/shader_asset.h"
#include <imgui.h>
#include "nau/assets/asset_ref.h"

#include "nau/3d/dag_drv3d.h"

struct ImGuiIO;
struct ImDrawData;

class DagImGuiRenderer
{
public:
  DagImGuiRenderer();
  void setBackendFlags(ImGuiIO &io);
  void createAndSetFontTexture(ImGuiIO &io);
  void render(ImDrawData *draw_data);

private:
  d3d::SamplerHandle sampler;
  BaseTexture* fontTex = nullptr;

  nau::MaterialAssetRef m_imguiDefaultMaterialRef {nau::AssetPath{"file:/res/materials/imgui.nmat_json"}};
  nau::MaterialAssetView::Ptr m_imguiDefaultMaterial;


  int vStride = sizeof(ImDrawVert);
  Sbuffer* vb = nullptr;
  Sbuffer* ib = nullptr;
  uint32_t vbSize = 0;
  uint32_t ibSize = 0;
};