// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "nau/scene/scene_object.h"

namespace nau::scene
{
    /*
    void dumpScene(SceneObject* obj, eastl::u8string offset)
    {
        NAU_LOG_DEBUG(offset + obj->getName().c_str());
        auto components = obj->getChildComponents();
        for (size_t i = 0; i < components.size(); ++i)
        {
            NAU_LOG_DEBUG(components[i]->NauRtti_TypeId.getTypeName());
        }

        auto childs = obj->getChildObjects();
        offset += u8" ";
        for (size_t i = 0; i < childs.size(); ++i)
        {
            dumpScene(childs[i], offset);
        }
    }
    */
}  // namespace nau::scene