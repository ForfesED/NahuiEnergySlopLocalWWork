// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#include "rich_text_helper.h"
#include <CCPlatformMacros.h>
#include "base/ccUTF8.h"


nau::ui::HorizontalAlignment RichTextHelper::getHorizontalAlignment(const std::u32string& alignment)
{
    static const std::map<std::u32string, nau::ui::HorizontalAlignment> alignmentMap =
    {
        {U"left", nau::ui::HorizontalAlignment::left},
        {U"center", nau::ui::HorizontalAlignment::center},
        {U"right", nau::ui::HorizontalAlignment::right}
    };

    auto it = alignmentMap.find(alignment);
    if (it != alignmentMap.end())
    {
        return it->second;
    }
    else
    {
        NAU_LOG_ERROR("Invalid alignment value:{}", std::string(alignment.begin(), alignment.end()));
    }

    return nau::ui::HorizontalAlignment::left;
}

ColorData RichTextHelper::getRichTextColorData(const std::u32string& colorString)
{
    if (colorString.size() != 10 || colorString.substr(0, 2) != U"0x")
    {
        return { cocos2d::Color3B(0, 0, 0), 255 };
    }

    unsigned int colorValue = std::stoul(std::string(colorString.begin(), colorString.end()), nullptr, 16);

    unsigned char alpha = (colorValue >> 24) & 0xFF;
    unsigned char red = (colorValue >> 16) & 0xFF;
    unsigned char green = (colorValue >> 8) & 0xFF;
    unsigned char blue = colorValue & 0xFF;

    return { cocos2d::Color3B(red, green, blue), alpha };
}

bool RichTextHelper::tryGetRichTextCustomAlignment(std::vector<RichTextTag>& currentRichTags, nau::ui::HorizontalAlignment& alignValue)
{
    for (auto tagIt = currentRichTags.rbegin(); tagIt != currentRichTags.rend(); ++tagIt)
    {
        if (tagIt->Name == U"align")
        {
            auto attrIt = tagIt->Attributes.find(U"align");
            if (attrIt != tagIt->Attributes.end())
            {
                alignValue = getHorizontalAlignment(attrIt->second);
                return true;
            }
            else 
            {
                NAU_LOG_ERROR("Align tag has no value!");
            }
        }
    }

    return false;
}

void RichTextHelper::updateCurrentRichTextTags(std::vector<RichTextTag>& currentRichTags, RichTextParseResult& parseResult)
{
    for (RichTextTag& tag : parseResult.Tags)
    {
        if (tag.Type == RichTextTag::RichTextTagType::Open)
        {
            currentRichTags.emplace_back(tag);
        }
        else if (tag.Type == RichTextTag::RichTextTagType::Close)
        {
            if (!currentRichTags.empty())
            {
                RichTextTag lastElement = currentRichTags.back();

                if (lastElement.Name == tag.Name)
                {
                    currentRichTags.pop_back();
                }
                else
                {
                    NAU_LOG_ERROR("Incorrect closing rich text tag name");
                }
            }
            else
            {
                NAU_LOG_ERROR("Incorrect closing rich text tag behavior");
            }
        }
    }
}

nau::ui::SymbolParams RichTextHelper::getSymbolParams(std::vector<RichTextTag>& currentRichTags)
{
    nau::ui::SymbolParams params;

    for (RichTextTag& tag : currentRichTags)
    {
        if (tag.Name == U"color")
        {
            auto it = tag.Attributes.find(U"color");
            if (it != tag.Attributes.end())
            {
                params.Color = it->second;
            }
            else
            {
                NAU_LOG_ERROR("Color tag has no value!");
            }
        }

        if (tag.Name == U"font")
        {
            auto it = tag.Attributes.find(U"font");
            if (it != tag.Attributes.end())
            {
                params.Font = it->second;
            }
            else
            {
                NAU_LOG_ERROR("Font tag has no value!");
            }
        }

        if (tag.Name == U"image")
        {
            auto srcIt = tag.Attributes.find(U"src");
            if (srcIt != tag.Attributes.end())
            {
                params.Image = srcIt->second;
            }
            else
            {
                NAU_LOG_ERROR("Image tag has no path value!");
            }

            auto scaleIt = tag.Attributes.find(U"scale");
            if (scaleIt != tag.Attributes.end())
            {
                params.ImageScale = u32stringToFloat(scaleIt->second);
            }
            else
            {
                params.ImageScale = 1.0f;
            }

            auto rotIt = tag.Attributes.find(U"rotation");
            if (rotIt != tag.Attributes.end())
            {
                params.ImageRotation = u32stringToFloat(rotIt->second);
            }
            else
            {
                params.ImageRotation = 0.0f;
            }
        }
    }

    return params;
}

cocos2d::Size RichTextHelper::getSpriteContentSize(const std::u32string& imagePath)
{
    std::string utf8PathString;
    if (!cocos2d::StringUtils::UTF32ToUTF8(imagePath, utf8PathString))
    {
        NAU_LOG_ERROR("UTF8ToUTF32 convert error");
        return cocos2d::Size(0, 0);
    }

    auto sprite = cocos2d::Sprite::create(utf8PathString);

    if (sprite) 
    {
        cocos2d::Size spriteSize = sprite->_getContentSize();
        return spriteSize;
    }
    else 
    {
        return cocos2d::Size(0, 0);
    }
}

void RichTextHelper::removeProcessedImageFromRichTags(std::vector<RichTextTag>& currentRichTags)
{
    currentRichTags.erase(std::remove_if(
        currentRichTags.begin(),
        currentRichTags.end(),
        [](const RichTextTag& tag) { return tag.Name == U"image"; }),
        currentRichTags.end());
}

float RichTextHelper::u32stringToFloat(const std::u32string& u32str)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::string utf8str = converter.to_bytes(u32str);

    try 
    {
        return std::stof(utf8str);
    }
    catch (const std::invalid_argument& e) 
    {
        NAU_LOG_ERROR("[RichTextHelper][u32stringToFloat] Invalid argument");
        return 0.0f;
    }
    catch (const std::out_of_range& e) 
    {
        NAU_LOG_ERROR("[RichTextHelper][u32stringToFloat] Out of range");
        return 0.0f;
    }
}
