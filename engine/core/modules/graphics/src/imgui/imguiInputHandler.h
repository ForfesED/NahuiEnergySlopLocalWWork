// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include <gainput/gainput.h>
#include <gainput/GainputInputListener.h>

struct DearImGuiInputHandler : public gainput::InputListener
{
  DearImGuiInputHandler();
  virtual bool OnDeviceButtonBool(gainput::DeviceId device, gainput::DeviceButtonId deviceButton, bool oldValue, bool newValue) override;
  virtual bool OnDeviceButtonFloat(gainput::DeviceId device, gainput::DeviceButtonId deviceButton, float oldValue, float newValue) override;
  virtual int GetPriority() const override;

  bool hybridInput = true;
  bool drawMouseCursor = false;
  int viewPortOffsetX = 0;
  int viewPortOffsetY = 0;

  gainput::ListenerId listenerId = -1;
};
