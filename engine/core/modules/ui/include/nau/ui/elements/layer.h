// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/ui/elements/node.h"

#include <cocos/2d/CCLayer.h>

namespace nau::ui
{


class NAU_UI_EXPORT Layer : public ::nau::ui::Node, protected cocos2d::Layer
{
public:
    static Layer *create();

protected:
    using cocos2d::Layer::init;

public:
    using cocos2d::Layer::onTouchBegan;
    using cocos2d::Layer::onTouchMoved;
    using cocos2d::Layer::onTouchEnded;
    using cocos2d::Layer::onTouchCancelled;
    using cocos2d::Layer::onTouchesBegan;
    using cocos2d::Layer::onTouchesMoved;
    using cocos2d::Layer::onTouchesEnded;
    using cocos2d::Layer::onTouchesCancelled;
    using cocos2d::Layer::onAcceleration;
    using cocos2d::Layer::onKeyPressed;
    using cocos2d::Layer::onKeyReleased;
    using cocos2d::Layer::getDescription;

};

} //namespace nau::ui
