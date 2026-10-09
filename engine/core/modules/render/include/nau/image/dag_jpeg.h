// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <EASTL/string.h>


// forward declarations for external classes
class IGenLoad;
class IGenSave;
struct TexImage32;
struct TexImage8;
struct TexImage8a;
struct TexPixel32;
struct IAllocImg;


// load JPG file
TexImage32 *load_jpeg32(IGenLoad &crd, eastl::allocator *mem, eastl::string *comments = NULL);
void *load_jpeg32(IGenLoad &crd, IAllocImg &a, eastl::string *comments = NULL);
TexImage8a *load_jpeg8a(IGenLoad &crd, eastl::allocator *);
TexImage8 *load_jpeg8(IGenLoad &crd, eastl::allocator *);

TexImage32 *load_jpeg32(const char *fn, eastl::allocator *);
TexImage8a *load_jpeg8a(const char *fn, eastl::allocator *);
TexImage8 *load_jpeg8(const char *fn, eastl::allocator *);

// save JPG file
bool save_jpeg32(unsigned char *ptr, int width, int height, int stride, int quality, IGenSave &cwr, const char *comments = NULL);
bool save_jpeg32(struct TexPixel32 *pix, int w, int h, int stride, int quality, const char *fn, const char *comments = NULL);
bool save_jpeg32(TexImage32 *im, int quality, const char *fn, const char *comments = NULL);
