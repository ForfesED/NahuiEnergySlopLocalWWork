// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "scene_manager_impl.h"

namespace nau::scene
{
    SceneQuery createSingleObjectQuery(ObjectWeakRef<> object)
    {
        if (!object)
        {
            return SceneQuery{};
        }

        if (object->is<Component>())
        {
            SceneQuery query{QueryObjectCategory::Component, object->getUid()};

            const rtti::TypeInfo& componentType = object->as<const DynamicObject&>().getClassDescriptor()->getClassTypeInfo();
            query.setType(componentType);

            return query;
        }
        else if (object->is<SceneObject>())
        {
            return SceneQuery{QueryObjectCategory::Object, object->getUid()};
        }
        else
        {
            NAU_FAILURE("Building scene query for this object category is not implemented");
        }

        return SceneQuery{};
    }

    ObjectWeakRef<> SceneManagerImpl::querySingleObject(const SceneQuery& query)
    {
        if (query.category)
        {
            return *query.category == QueryObjectCategory::Component ? lookupComponent(query) : lookupSceneObject(query);
        }

        auto result = lookupComponent(query);
        if (!result)
        {
            result = lookupSceneObject(query);
        }
        return result;
    }

    ObjectWeakRef<> SceneManagerImpl::lookupComponent(const SceneQuery& query)
    {
        Component* component = nullptr;

        if (query.uid != NullUid)
        {
            if (auto iter = m_activeComponents.find(query.uid); iter != m_activeComponents.end())
            {
                component = iter->second;
            }
            else
            {
                return nullptr;
            }
        }
        else
        {
            NAU_LOG_WARNING("Current query mechanism is very restricted and can query components only by uid");
        }

        if (component && query.typeHashCode != 0)
        {
            if (!component->is(query.getType()))
            {
                component = nullptr;
            }
        }

        // Initialization ObjectWeakRef<> with null (not std::nullptr_t) is prohibited for security reasons
        return component ? ObjectWeakRef<>{component} : nullptr;
    }

    ObjectWeakRef<> SceneManagerImpl::lookupSceneObject(const SceneQuery& query)
    {
        SceneObject* sceneObject = nullptr;

        if (query.uid != NullUid)
        {
            if (auto iter = m_activeObjects.find(query.uid); iter != m_activeObjects.end())
            {
                sceneObject = iter->second;
            }
        }
        else
        {
            NAU_LOG_WARNING("Current query mechanism is very restricted and can query objects only by uid");
        }

        return sceneObject ? ObjectWeakRef<>{sceneObject} : nullptr;
    }

}  // namespace nau::scene