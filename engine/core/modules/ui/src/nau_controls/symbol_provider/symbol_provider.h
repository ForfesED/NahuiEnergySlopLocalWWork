// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once

#include <nau/ui/label.h>
#include <string>

#include <string>
#include <EASTL/optional.h>
#include <EASTL/string.h>
#include <EASTL/unordered_map.h>


namespace cocos2d
{
    class Texture2D;
}

class ISymbolProvider
{
public:
    virtual ~ISymbolProvider() = default;

    virtual int* getHorizontalKerning(const eastl::u32string& text, int& outNumLetters) const = 0;
    virtual cocos2d::Texture2D* getSymbolTexture(int textureId) const = 0;
    virtual bool getSymbol(char32_t utf32Code, nau::ui::FontLetterDefinition& symbolDefinition) = 0;
    virtual bool hasSymbol(char32_t utf32Code) const = 0;
    virtual bool warmUpSymbosCache(const eastl::u32string& text) const = 0;

    FORCEINLINE const std::u32string& getName() { return m_providerName; };
    FORCEINLINE void setName(const std::u32string& providerName) { m_providerName = providerName; };

    void addDefinition(char32_t symbol, const nau::ui::FontLetterDefinition& definition);
    eastl::optional<nau::ui::FontLetterDefinition> getDefinition(char32_t symbol) const;
    bool containsDefinition(char32_t symbol) const;

private:
    std::u32string m_providerName {};
    eastl::unordered_map<char32_t, nau::ui::FontLetterDefinition> m_symbolsCache{};
};