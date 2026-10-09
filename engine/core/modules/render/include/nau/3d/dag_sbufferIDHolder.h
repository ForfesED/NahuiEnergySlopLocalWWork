// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "nau/3d/dag_resMgr.h"

class SbufferIDPair
{
protected:
  Sbuffer *buf;
  D3DRESID bufId;

public:
  SbufferIDPair(Sbuffer *t = nullptr, D3DRESID id = BAD_D3DRESID) : buf(t), bufId(id) {}

  D3DRESID getId() const { return bufId; }
  Sbuffer *getBuf() const { return buf; }
};

class SbufferIDHolder : public SbufferIDPair
{
public:
  SbufferIDHolder() = default;
  SbufferIDHolder(SbufferIDHolder &&rhs)
  {
    buf = rhs.buf;
    bufId = rhs.bufId;
    rhs.buf = nullptr;
    rhs.bufId = BAD_D3DRESID;
  }
  ~SbufferIDHolder() { close(); }

  SbufferIDHolder &operator=(SbufferIDHolder &&rhs)
  {
    close();
    buf = rhs.buf;
    bufId = rhs.bufId;
    rhs.buf = nullptr;
    rhs.bufId = BAD_D3DRESID;
    return *this;
  }

  void setRaw(Sbuffer *buf_, D3DRESID texId_)
  {
    buf = buf_;
    bufId = texId_;
  }
  void set(Sbuffer *buf_, D3DRESID texId_)
  {
    close();
    setRaw(buf_, texId_);
  }
  void set(Sbuffer *buf_, const char *name);
  void close();

private:
  SbufferIDHolder(const SbufferIDHolder &) = delete; // Avoid removing the copy of the texture.
};

class SbufferIDHolderWithVar : public SbufferIDPair
{
  int varId = -1;

public:
  SbufferIDHolderWithVar() = default;
  SbufferIDHolderWithVar(SbufferIDHolderWithVar &&rhs)
  {
    buf = rhs.buf;
    bufId = rhs.bufId;
    varId = rhs.varId;
    rhs.buf = nullptr;
    rhs.bufId = BAD_D3DRESID;
    rhs.varId = -1;
  }
  ~SbufferIDHolderWithVar() { close(); }
  void setVarId(int id) { varId = id; }
  int getVarId() const { return varId; }

  SbufferIDHolderWithVar &operator=(SbufferIDHolderWithVar &&rhs)
  {
    close();
    buf = rhs.buf;
    bufId = rhs.bufId;
    varId = rhs.varId;
    rhs.buf = nullptr;
    rhs.bufId = BAD_D3DRESID;
    rhs.varId = -1;
    return *this;
  }

  void setRaw(Sbuffer *buf_, D3DRESID texId_)
  {
    buf = buf_;
    bufId = texId_;
  }
  void set(Sbuffer *buf_, D3DRESID texId_)
  {
    close();
    setRaw(buf_, texId_);
  }
  void set(Sbuffer* buf_, const char* name)
  {
      close();
      if(buf_)
      {
          this->buf = buf_;
          bufId = register_managed_res(name, buf_);
      }

      // TODO shaders will be rewritten in the following iterations
      //varId = get_shader_variable_id(name, true);
  }

  void setVar() const
  {
      // TODO shaders will be rewritten in the following iterations
      //if(varId > 0)
      //    ShaderGlobal::set_buffer(varId, bufId);
  }

  void close()
  {
      // TODO shaders will be rewritten in the following iterations
      //if(bufId != BAD_D3DRESID)
      //    ShaderGlobal::reset_from_vars(bufId);
      release_managed_buf_verified(bufId, buf);
      varId = -1;
  }

private:
  SbufferIDHolderWithVar(const SbufferIDHolderWithVar &) = delete; // Avoid removing the copy of the texture.
};
