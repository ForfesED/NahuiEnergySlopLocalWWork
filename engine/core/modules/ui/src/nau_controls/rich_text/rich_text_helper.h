// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <codecvt>
#include "rich_text_models.h"
#include "nau/ui/ui_control.h"
#include "math/CCGeometry.h"
#include "2d/CCSprite.h"
#include "nau/ui/label.h"


class RichTextHelper
{
public:
    static ColorData getRichTextColorData(const std::u32string& colorString);
    static bool tryGetRichTextCustomAlignment(std::vector<RichTextTag>& currentRichTags, nau::ui::HorizontalAlignment& alignValue);
    static void updateCurrentRichTextTags(std::vector<RichTextTag>& currentRichTags, RichTextParseResult& parseResult);
    static nau::ui::SymbolParams getSymbolParams(std::vector<RichTextTag>& currentRichTags);
    static cocos2d::Size getSpriteContentSize(const std::u32string& imagePath);
    static void removeProcessedImageFromRichTags(std::vector<RichTextTag>& currentRichTags);

private:
    static nau::ui::HorizontalAlignment getHorizontalAlignment(const std::u32string& alignment);
    static float u32stringToFloat(const std::u32string& u32str);
};