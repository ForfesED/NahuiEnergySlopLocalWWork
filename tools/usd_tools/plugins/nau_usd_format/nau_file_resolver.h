// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <pxr/usd/ar/defaultResolver.h>

class NauFileResolver : public PXR_NS::ArDefaultResolver
{
protected:
    std::string _CreateIdentifier(const std::string& assetPath, const PXR_NS::ArResolvedPath& anchorAssetPath) const override;
    PXR_NS::ArResolvedPath _Resolve(const std::string& assetPath) const override;
  
};