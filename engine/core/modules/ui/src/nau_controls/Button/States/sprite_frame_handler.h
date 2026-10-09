// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

namespace cocos2d 
{ 
    class SpriteFrame; 
}

namespace nau::ui
{

struct SpriteFrameHandler
{
    cocos2d::SpriteFrame* m_spriteFrame{nullptr};
};

}