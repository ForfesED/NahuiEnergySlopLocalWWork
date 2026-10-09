// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/rtti/nau_weak_ptr.h


#pragma once

#include "nau/diag/assertion.h"
#include "nau/rtti/ptr.h"

namespace nau
{

    template <typename T = IRefCounted>
    class WeakPtr
    {
    public:
        using type = T;

        WeakPtr() = default;

        WeakPtr(const WeakPtr<T>& weakPtr) :
            m_weakRef(weakPtr.m_weakRef)
        {
            if(m_weakRef)
            {
                m_weakRef->addWeakRef();
            }
        }

        WeakPtr(WeakPtr<T>&& weakPtr) :
            m_weakRef(weakPtr.m_weakRef)
        {
            weakPtr.m_weakRef = nullptr;
        }

        explicit WeakPtr(const Ptr<T>& ptr)
        {
            if(ptr)
            {
                m_weakRef = rtti_detail::asRefCounted(*ptr).getWeakRef();
            }
        }

        ~WeakPtr()
        {
            reset();
        }

        WeakPtr<T>& operator= (const WeakPtr<T>& other)
        {
            reset();

            if(m_weakRef = other.m_weakRef; m_weakRef)
            {
                m_weakRef->addWeakRef();
            }

            return *this;
        }

        WeakPtr<T>& operator= (WeakPtr<T>&& other)
        {
            reset();

            NAU_ASSERT(m_weakRef == nullptr);
            std::swap(m_weakRef, other.m_weakRef);

            return *this;
        }

        WeakPtr<T>& operator= (const Ptr<T>& ptr)
        {
            reset();

            if(ptr)
            {
                m_weakRef = rtti_detail::asRefCounted(*ptr).getWeakRef();
            }

            return *this;
        }

        Ptr<T> acquire()
        {
            IRefCounted* const ptr = (m_weakRef != nullptr) ? m_weakRef->acquire() : nullptr;

            if(!ptr)
            {
                return {};
            }

            T* const targetInstance = ptr->as<T*>();

            NAU_ASSERT(targetInstance, "RefCounted object acquired through weak reference, but instance doesn't provide target interface");

            return rtti::TakeOwnership{targetInstance};
        }

        Ptr<T> lock()
        {
            return acquire();
        }

        explicit operator bool () const
        {
            return m_weakRef != nullptr;
        }

        /**
         * @brief
         * Check that referenced instance is not accessible anymore (i.e. it has been destructed).
         * System can guarantee - that died instance never gone to be alive,
         * but in opposed case alive instance can be a dead immediately right after isAlive returns true;
         */
        bool isDead() const
        {
            return !m_weakRef || m_weakRef->isDead();
        }

        void reset()
        {
            if(m_weakRef)
            {
                IWeakRef* weakRef = nullptr;
                std::swap(weakRef, m_weakRef);
                weakRef->releaseRef();
            }
        }

        IWeakRef* get() const
        {
            return m_weakRef;
        }

        IWeakRef* giveUp()
        {
            IWeakRef* const weakRef = m_weakRef;
            m_weakRef = nullptr;
            return weakRef;
        }

    private:
        IWeakRef* m_weakRef = nullptr;
    };
}  // namespace nau
