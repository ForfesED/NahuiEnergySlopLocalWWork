// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/scene/scene_object.h"

namespace nau::scene
{
    Vector<SceneObject*> SceneObject::getDirectChildObjects()
    {
        return getChildObjects(false);
    }

    Vector<SceneObject*> SceneObject::getAllChildObjects()
    {
        return getChildObjects(true);
    }

    const math::Transform& SceneObject::getWorldTransform() const
    {
        return getRootComponentInternal().getWorldTransform();
    }

    void SceneObject::setWorldTransform(const math::Transform& transform)
    {
        return getRootComponentInternal().setWorldTransform(transform);
    }

    const math::Transform& SceneObject::getTransform() const
    {
        return getRootComponentInternal().getTransform();
    }

    void SceneObject::setTransform(const math::Transform& transform)
    {
        return getRootComponentInternal().setTransform(transform);
    }

    void SceneObject::setRotation(math::quat rotation)
    {
        getRootComponentInternal().setRotation(rotation);
    }

    void SceneObject::setTranslation(math::vec3 pos)
    {
        getRootComponentInternal().setTranslation(pos);
    }

    void SceneObject::setScale(math::vec3 scale)
    {
        getRootComponentInternal().setScale(scale);
    }

    math::quat SceneObject::getRotation() const
    {
        return getRootComponentInternal().getRotation();
    }

    math::vec3 SceneObject::getTranslation() const
    {
        return getRootComponentInternal().getTranslation();
    }

    math::vec3 SceneObject::getScale() const
    {
        return getRootComponentInternal().getScale();
    }

}  // namespace nau::scene
