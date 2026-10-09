// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/scene/nau_object.h"

#include "nau/scene/scene_manager.h"

namespace nau::scene
{
#ifdef NAU_ASSERT_ENABLED
    void NauObject::operator delete(void*, size_t)
    {
        NAU_FATAL_FAILURE("explicit delete operation is not applicable for NauObject instances");
    }
#endif

    NauObject::~NauObject()
    {
        NAU_ASSERT(m_references.empty());
    }

    void NauObject::destroy()
    {
        deleteObjectNow();
    }

    Uid NauObject::getUid() const
    {
        return m_uid;
    }

    void NauObject::setUid(Uid uid)
    {
        m_uid = uid;
    }

    void NauObject::addReference(scene_internal::ObjectWeakRefBase& reference)
    {
        NAU_FATAL(!m_references.contains(reference));
        m_references.push_back(reference);
    }

    void NauObject::removeReference(scene_internal::ObjectWeakRefBase& reference)
    {
        NAU_FATAL(m_references.contains(reference), "Has no reference");
        m_references.remove(reference);
    }

    void NauObject::clearAllWeakReferences()
    {
        for (scene_internal::ObjectWeakRefBase& reference : m_references)
        {
            reference.notifyReferencedObjectDestroyed();
        }

        m_references.clear();
    }

    void NauObject::onBeforeDeleteObject()
    {
    }

    void NauObject::deleteObjectNow()
    {
        NAU_FATAL(m_allocator);
        NAU_FATAL(m_destructorFunc);
        NAU_FATAL(!m_hasPtrOwner);

        clearAllWeakReferences();

        IMemAllocator* const allocator = std::exchange(m_allocator, nullptr);
        DestructorFunc destructor = std::exchange(m_destructorFunc, nullptr);

        NAU_FATAL(allocator);

        void* const ptr = destructor(this);
        allocator->deallocateAligned(ptr);
    }
}  // namespace nau::scene

namespace nau::scene_internal
{
    ObjectWeakRefBase::ObjectWeakRefBase() = default;

    ObjectWeakRefBase::~ObjectWeakRefBase()
    {
        reset();
    }

    ObjectWeakRefBase::ObjectWeakRefBase(scene::NauObject* object) :
        m_object(object)
    {
        if (m_object)
        {
            m_object->addReference(*this);
        }
    }

    ObjectWeakRefBase::ObjectWeakRefBase(const ObjectWeakRefBase& other) :
        ObjectWeakRefBase(other.m_object)
    {
    }

    ObjectWeakRefBase& ObjectWeakRefBase::operator=(const ObjectWeakRefBase& other)
    {
        reset(other.m_object);
        return *this;
    }

    bool ObjectWeakRefBase::equals(const ObjectWeakRefBase& other) const
    {
        return m_object == other.m_object;
    }

    void ObjectWeakRefBase::reset(scene::NauObject* newObject)
    {
        m_objectQuery.reset();

        auto* const oldObject = std::exchange(m_object, newObject);
        if (oldObject)
        {
            oldObject->removeReference(*this);
        }

        if (m_object)
        {
            m_object->addReference(*this);
        }
    }

    bool ObjectWeakRefBase::refIsValid() const
    {
        return m_object != nullptr;
    }

    ObjectWeakRefBase* RuntimeObjectWeakRefValueImpl::allocateInternalStorage(WeakRefStorage& storage, const scene_internal::ObjectWeakRefBase& weakRef)
    {
        return new(&storage) scene_internal::ObjectWeakRefBase(weakRef);
    }

    RuntimeObjectWeakRefValueImpl::~RuntimeObjectWeakRefValueImpl()
    {
        using namespace nau::scene_internal;
        if (m_weakRef && m_weakRef == reinterpret_cast<ObjectWeakRefBase*>(&m_weakRefStorage))
        {
            m_weakRef->~ObjectWeakRefBase();
        }
    }

    RuntimeObjectWeakRefValueImpl::RuntimeObjectWeakRefValueImpl() :
        m_isMutable(true),
        m_weakRef(allocateInternalStorage(m_weakRefStorage, scene::ObjectWeakRef<>{}))
    {
        NAU_FATAL(m_weakRef);
    }

    RuntimeObjectWeakRefValueImpl::RuntimeObjectWeakRefValueImpl(const scene_internal::ObjectWeakRefBase& weakRef, CopyCtorTag) :
        m_isMutable(true),
        m_weakRef(allocateInternalStorage(m_weakRefStorage, weakRef))
    {
        NAU_FATAL(m_weakRef);
    }

    RuntimeObjectWeakRefValueImpl::RuntimeObjectWeakRefValueImpl(const scene_internal::ObjectWeakRefBase& weakRef) :
        m_isMutable(false),
        m_weakRef(&const_cast<scene_internal::ObjectWeakRefBase&>(weakRef))
    {
        NAU_FATAL(m_weakRef);
    }

    RuntimeObjectWeakRefValueImpl::RuntimeObjectWeakRefValueImpl(scene_internal::ObjectWeakRefBase& weakRef) :
        m_isMutable(true),
        m_weakRef(&weakRef)
    {
        NAU_FATAL(m_weakRef);
    }

    bool RuntimeObjectWeakRefValueImpl::isMutable() const
    {
        return m_isMutable;
    }

    Result<> RuntimeObjectWeakRefValueImpl::setString(std::string_view str)
    {
        NAU_ASSERT(m_isMutable);
        if (!m_isMutable)
        {
            return NauMakeError("Attempt to modify an immutable value.");
        }

        m_weakRef->m_object = nullptr;
        m_weakRef->m_objectQuery.emplace();
        if (!str.empty())
        {
            return parse(str, *m_weakRef->m_objectQuery);
        }

        // accept empty string as valid value (i.e. null reference);

        return ResultSuccess;
    }

    std::string RuntimeObjectWeakRefValueImpl::getString() const
    {
        using namespace nau::scene;

        if (m_weakRef->m_object != nullptr)
        {
            const SceneQuery query = scene::createSingleObjectQuery(ObjectWeakRef{m_weakRef->getMutableNauObjectPtr()});
            return toString(query);
        }
        else if (m_weakRef->m_objectQuery)
        {
            return toString(*m_weakRef->m_objectQuery);
        }

        return std::string{};
    }

    scene::SceneQuery RuntimeObjectWeakRefValueImpl::getObjectQuery() const
    {
        return m_weakRef->m_objectQuery.value_or(scene::SceneQuery{});
    }

    bool RuntimeObjectWeakRefValueImpl::isAssignable([[maybe_unused]] const scene::NauObject& object) const
    {
        return true;
    }

    scene::ObjectWeakRef<> RuntimeObjectWeakRefValueImpl::getObjectWeakRefInternal()
    {
        scene::NauObject* const objectPtr = m_weakRef->getMutableNauObjectPtr();

        // Constructing ObjectWeakRef from nullptr is prohbited for security reasons:
        return objectPtr == nullptr ? scene::ObjectWeakRef<>{} : scene::ObjectWeakRef<>{*objectPtr};
    }

    void RuntimeObjectWeakRefValueImpl::setObjectWeakRefInternal(scene::ObjectWeakRef<> weakRef)
    {
        if (weakRef && !isAssignable(*weakRef.get()))
        {
            return;
        }

        m_weakRef->reset(weakRef.get());
    }

}  // namespace nau::scene_internal
