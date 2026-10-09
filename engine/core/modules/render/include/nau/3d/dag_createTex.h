// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

// forward declarations for external classes
class BaseTexture;
struct TextureMetaData;


class ICreateTexFactory
{
public:
  virtual BaseTexture *createTex(const char *fn, int flg, int levels, const char *fn_ext, const TextureMetaData &tmd) = 0;
};

void add_create_tex_factory(ICreateTexFactory *ctf);
void del_create_tex_factory(ICreateTexFactory *ctf);

BaseTexture *create_texture(const char *fn, int flg, int levels, bool fatal_on_err, const char *fnext = nullptr);
BaseTexture *create_texture_via_factories(const char *fn, int flg, int levels, const char *fn_ext, const TextureMetaData &tmd,
  ICreateTexFactory *excl_factory);
