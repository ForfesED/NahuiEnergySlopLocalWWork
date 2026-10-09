// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "base/ccTypes.h"


class RichTextTag
{
public:
    enum class RichTextTagType
    {
        Open,
        Close
    };

    RichTextTagType Type;
    std::u32string Name;
    std::unordered_map<std::u32string, std::u32string> Attributes;
};

struct RichTextParseResult
{
    std::vector<RichTextTag> Tags;
    size_t MoveToIndex;
};

struct ColorData
{
    cocos2d::Color3B Color;
    uint8_t Opacity;
};