// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "nau/ui/elements/clipping_node.h"


namespace nau::ui
{
    ClippingNode* ClippingNode::create()
    {
        return Node::create<ClippingNode>();
    }

    Node* ClippingNode::getStencil() const
    {
        return  as<Node>(cocos2d::ClippingNode::getStencil());
    }

    void ClippingNode::setStencil(Node *stencil)
    {
        cocos2d::ClippingNode::setStencil(stencil);
    }
}