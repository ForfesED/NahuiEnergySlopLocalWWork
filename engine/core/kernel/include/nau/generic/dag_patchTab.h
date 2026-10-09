// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved 
#pragma once

#include <EASTL/span.h>
#include <cstddef>

/// @addtogroup containers
/// @{

/// Template array that has method for patching pointer address.
/// Based on eastl::span. Intended for patching arrays in serialized dump of data.
template <class T>
class PatchableTab : protected eastl::span<T>
{
public:
  using typename eastl::span<T>::value_type;
  using typename eastl::span<T>::index_type;
  using typename eastl::span<T>::iterator;
  using typename eastl::span<T>::const_iterator;
  using typename eastl::span<T>::reference;
  using typename eastl::span<T>::const_reference;
  using eastl::span<T>::operator[];
  using eastl::span<T>::begin;
  using eastl::span<T>::cbegin;
  using eastl::span<T>::end;
  using eastl::span<T>::cend;
  using eastl::span<T>::front;
  using eastl::span<T>::back;
  using eastl::span<T>::data;
  using eastl::span<T>::size;
  using eastl::span<T>::size_bytes;
  using eastl::span<T>::empty;

  PatchableTab() {}
  ~PatchableTab() {}

  PatchableTab(PatchableTab &&ft) = default;
  PatchableTab &operator=(PatchableTab &&ft) = default;



  /// Add specified base address to data address (pointer).
  void patch(void *base)
  {
#if NAU_64BIT
#if _TARGET_BE
    eastl::span<T>::mnSize = intptr_t(eastl::span<T>::mpData) & 0xFFFFFFFF;
    eastl::span<T>::mpData = eastl::span<T>::mnSize ? (T *)((ptrdiff_t(eastl::span<T>::mpData) >> 32) + ptrdiff_t(base)) : NULL;
#else

    eastl::span<T>::mnSize = intptr_t(eastl::span<T>::mpData) >> 32;
    eastl::span<T>::mpData = eastl::span<T>::mnSize ? (T *)((ptrdiff_t(eastl::span<T>::mpData) & 0xFFFFFFFF) + ptrdiff_t(base)) : NULL;
#endif
#else
    if (eastl::span<T>::mnSize)
      eastl::span<T>::mpData = (T *)(ptrdiff_t(eastl::span<T>::mpData) + ptrdiff_t(base));
#endif
  }

  /// Rebases already patched data address (useful for fast cloning)
  void rebase(void *newbase, const void *oldbase)
  {
    if (eastl::span<T>::mnSize)
      eastl::span<T>::mpData = (T *)(ptrdiff_t(eastl::span<T>::mpData) + ptrdiff_t(newbase) - ptrdiff_t(oldbase));
  }

  /// Explicitly init eastl::span (useful when when constructing PatchableTab not from load only)
  void init(void *base, intptr_t cnt)
  {
    eastl::span<T>::mpData = (T *)base;
    eastl::span<T>::mnSize = cnt;
  }

private:
  PatchableTab(const PatchableTab &ft);
  PatchableTab &operator=(const PatchableTab &ft);

#if !NAU_64BIT
  int _resv[2];
#endif
};
/// @}

#pragma pack(push, 4) // this pointers are not aligned at least in dag::RoNameMap::map
template <class T>
class PatchablePtr
{
public:
  PatchablePtr() {} //-V730
  ~PatchablePtr() {}

  T *get() const { return p; }
  operator T *() const { return p; }

  const T *operator->() const { return p; }
  T *operator->() { return p; }

  /// reinterprets initial value as int (must be used only before patch!)
  int toInt() const { return int(intptr_t(p)); }

  /// Add specified base address to data address (pointer).
  void patch(void *base) { p = int(ptrdiff_t(p) & 0xFFFFFFFF) >= 0 ? (T *)((ptrdiff_t(p) & 0xFFFFFFFF) + ptrdiff_t(base)) : NULL; }

  /// Add specified base address to data address (pointer). When offset is 0, pointer is treated as NULL
  void patchNonNull(void *base)
  {
    p = int(ptrdiff_t(p) & 0xFFFFFFFF) > 0 ? (T *)((ptrdiff_t(p) & 0xFFFFFFFF) + ptrdiff_t(base)) : NULL;
  }

  /// Rebases already patched data address (useful for fast cloning)
  void rebase(void *newbase, const void *oldbase) { p = p ? (T *)(ptrdiff_t(p) + ptrdiff_t(newbase) - ptrdiff_t(oldbase)) : NULL; }

  /// Expicitly assigns new pointer
  void operator=(const T *new_p) { p = (T *)new_p; }
  void setPtr(const void *new_p) { p = (T *)new_p; }

#if NAU_64BIT
  void clearUpperBits() {}
#else
  void clearUpperBits() { _resv = 0; }
#endif

private:
  T *p;

#if !NAU_64BIT
  int _resv;
#endif
};
#pragma pack(pop)

#if NAU_64BIT
#define PATCHABLE_64BIT_PAD32(NM) int NM
#define PATCHABLE_32BIT_PAD32(NM)
#define PATCHABLE_DATA64(T, NM) T NM
enum
{
  PATCHABLE_64BIT_PAD32_SZ = 4,
  PATCHABLE_32BIT_PAD32_SZ = 0,
};
#else
#define PATCHABLE_64BIT_PAD32(NM)
#define PATCHABLE_32BIT_PAD32(NM) int NM
#define PATCHABLE_DATA64(T, NM) \
  T NM;                         \
  int _##NM##HDW;
enum
{
  PATCHABLE_64BIT_PAD32_SZ = 0,
  PATCHABLE_32BIT_PAD32_SZ = 4,
};
#endif
