// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/ui/elements/node.h"
#include "2d/CCClippingNode.h"

namespace nau::ui 
{
    class NAU_UI_EXPORT ClippingNode : public ::nau::ui::Node, protected cocos2d::ClippingNode
    {
    public:
        static ClippingNode* create();

        Node* getStencil() const;
        void setStencil(Node *stencil);
    };
}