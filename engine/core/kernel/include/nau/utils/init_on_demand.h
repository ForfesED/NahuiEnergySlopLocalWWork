// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include "nau/diag/assertion.h"

template <typename T, bool ptr = true>
struct InitOnDemand
{
public:
    inline InitOnDemand() :
        obj(NULL)
    {
    }
    ~InitOnDemand()
    {
        demandDestroy();
    }

    inline operator T*() const
    {
        return obj;
    }
    inline T& operator*() const
    {
        return *obj;
    }
    inline T* operator->() const
    {
        return obj;
    }
    inline T* get() const
    {
        return obj;
    }
    explicit operator bool() const
    {
        return obj != NULL;
    }

    template <typename... Args>
    inline T* demandInit(const Args&... args)
    {
        if(!obj)
            obj = new T(args...);
        return obj;
    }
    inline void demandDestroy()
    {
        obj ? (delete obj, obj = NULL) : (0);
    }

private:
    T* obj;
};

template <typename T>
struct InitOnDemand<T, false>
{
public:
    InitOnDemand() :
        inited(false)
    {
    }
    ~InitOnDemand()
    {
        demandDestroy();
    }

    // Note: access before initialization or after deinitialization is not alloved because of type-safety
    // If you need explicit check use get() for that
#if NAU_DEBUG
    operator T*() const
    {
        NAU_FAST_ASSERT(inited);
        return getObj();
    }
    T& operator*() const
    {
        NAU_FAST_ASSERT(inited);
        return *getObj();
    }
    T* operator->() const
    {
        NAU_FAST_ASSERT(inited);
        return getObj();
    }
#else
    operator T*() const
    {
        return getObj();
    }
    T& operator*() const
    {
        return *getObj();
    }
    T* operator->() const
    {
        return getObj();
    }
#endif
    T* get() const
    {
        return inited ? getObj() : NULL;
    }
    explicit operator bool() const
    {
        return inited;
    }

    template <typename... Args>
    T* demandInit(const Args&... args)
    {
        if(!inited)
        {
            new (objBuf /*, _NEW_INPLACE*/) T(args...);
            inited = true;
        }
        return getObj();
    }
    void demandDestroy()
    {
        if(inited)
        {
            getObj()->~T();
            inited = false;
        }
    }

private:
    T* getObj() const
    {
        return (T*)objBuf;
    }

private:
    alignas(T) char objBuf[sizeof(T)];
    bool inited;
};