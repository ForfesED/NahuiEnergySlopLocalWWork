// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

// global render states
extern bool grs_draw_wire;

// compute and set gamma correction
void set_gamma(float p);
void set_gamma_shadervar(float p);

// returns current gamma
float get_current_gamma();
