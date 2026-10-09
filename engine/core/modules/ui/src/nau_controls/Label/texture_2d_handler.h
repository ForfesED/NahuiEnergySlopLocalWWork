// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once


namespace cocos2d 
{ 
    class Texture2D; 
}

namespace cocos2d 
{ 
    class Rect;
}

namespace nau::ui
{

struct Texture2DHandler
{
    Texture2DHandler(cocos2d::Texture2D* texture, cocos2d::Rect& rect) : 
        m_texture(texture), 
        m_rect(rect)
    {}

    cocos2d::Texture2D* m_texture{nullptr};
    cocos2d::Rect& m_rect;
};

}