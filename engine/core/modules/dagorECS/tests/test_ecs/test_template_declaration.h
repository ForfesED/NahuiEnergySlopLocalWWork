// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved
#pragma once

#include "daECS/core/template_declaration.h"



struct TestTemplateDeclarationStruct
{
    uint32_t tex_id = 0;
};

ECS_DECLARE_RELOCATABLE_TYPE(TestTemplateDeclarationStruct);

uint32_t call_t1();
uint32_t call_t2();
uint32_t call_t3();