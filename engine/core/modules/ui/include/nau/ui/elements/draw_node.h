// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include "nau/math/dag_color.h"
#include "nau/math/math.h"
#include "nau/ui/elements/node.h"
#include <cocos/2d/CCDrawNode.h>

namespace nau::ui 
{

class NAU_UI_EXPORT DrawNode : public Node,  protected cocos2d::DrawNode
{

public:
    static DrawNode* create();
    void drawPoint(const math::vec2& point, const float pointSize, const math::Color4 &color);
    void drawPoints(const math::vec2 *position, unsigned int numberOfPoints, const math::Color4 &color);
    void drawPoints(const math::vec2 *position, unsigned int numberOfPoints, const float pointSize, const math::Color4 &color);
    void drawLine(const math::vec2 &origin, const math::vec2 &destination, const math::Color4 &color);
    void drawRect(const math::vec2 &origin, const math::vec2 &destination, const math::Color4 &color);
    void drawPoly(const math::vec2 *poli, unsigned int numberOfPoints, bool closePolygon, const math::Color4 &color);
    void drawCircle( const math::vec2& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY, const math::Color4 &color);
    void drawCircle(const math::vec2 &center, float radius, float angle, unsigned int segments, bool drawLineToCenter, const math::Color4 &color);
    void drawQuadBezier(const math::vec2 &origin, const math::vec2 &control, const math::vec2 &destination, unsigned int segments, const math::Color4 &color);
    void drawCubicBezier(const math::vec2 &origin, const math::vec2 &control1, const math::vec2 &control2, const math::vec2 &destination, unsigned int segments, const math::Color4 &color);
    void drawDot(const math::vec2 &pos, float radius, const math::Color4 &color);
    void drawRect(const math::vec2 &p1, const math::vec2 &p2, const math::vec2 &p3, const math::vec2& p4, const math::Color4 &color);
    void drawSolidRect(const math::vec2 &origin, const math::vec2 &destination, const math::Color4 &color);
    void drawSolidPoly(const math::vec2 *poli, unsigned int numberOfPoints, const math::Color4 &color);

    void drawSolidCircle(const math::vec2& center, float radius, float angle, unsigned int segments, float scaleX, float scaleY, const math::Color4 &color);
    void drawSolidCircle(const math::vec2& center, float radius, float angle, unsigned int segments, const math::Color4& color);
    void drawSegment(const math::vec2 &from, const math::vec2 &to, float radius, const math::Color4 &color);
    void drawPolygon(const math::vec2 *verts, int count, const math::Color4 &fillColor, float borderWidth, const math::Color4 &borderColor);
    void drawTriangle(const math::vec2 &p1, const math::vec2 &p2, const math::vec2 &p3, const math::Color4 &color);

    void clearDrawNode();
    using cocos2d::DrawNode::getBlendFunc;
    using cocos2d::DrawNode::setBlendFunc;
    using cocos2d::DrawNode::setLineWidth;
    using cocos2d::DrawNode::getLineWidth;
    using cocos2d::DrawNode::setIsolated;
    using cocos2d::DrawNode::isIsolated;
};


}
