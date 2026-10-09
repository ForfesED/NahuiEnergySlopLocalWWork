/*
 * Copyright (c) 2008 Radu Gruian
 * Copyright (c) 2011 Vit Valentin
 * Copyright (c) 2012 cocos2d-x.org
 * Copyright (c) 2013-2016 Chukong Technologies Inc.
 * Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *
 * Original code by Radu Gruian: http://www.codeproject.com/Articles/30838/Overhauser-Catmull-Rom-Splines-for-Camera-Animatio.So
 *
 * Adapted to cocos2d-x by Vit Valentin
 *
 * Adapted from cocos2d-x to cocos2d-iphone by Ricardo Quesada
 */
#include "base/ccMacros.h"
#include "2d/CCPointArray.h"
#include "2d/CCNode.h"

#include <iterator>

using namespace std;

NS_CC_BEGIN;

/*
 *  Implementation of PointArray
 */

PointArray* PointArray::create(ssize_t capacity)
{
    PointArray* pointArray = new (std::nothrow) PointArray();
    if (pointArray && pointArray->initWithCapacity(capacity))
    {
        pointArray->autorelease();
        return pointArray;
    }

    delete pointArray;
    return nullptr;
}

bool PointArray::initWithCapacity(ssize_t capacity)
{
    _controlPoints.reserve(capacity);
    
    return true;
}

PointArray* PointArray::clone() const
{
    vector<Vec2> newArray = _controlPoints;
    
    PointArray *points = new (std::nothrow) PointArray();
    points->initWithCapacity(10);
    points->setControlPoints(std::move(newArray));

    points->autorelease();
    return points;
}

PointArray::~PointArray()
{
    NAU_LOG_INFO("deallocing PointArray: {:#x}", reinterpret_cast<uintptr_t>(this));
}

PointArray::PointArray() {}

const std::vector<Vec2>& PointArray::getControlPoints() const
{
    return _controlPoints;
}

void PointArray::setControlPoints(vector<Vec2> controlPoints)
{
    _controlPoints = std::move(controlPoints);
}

void PointArray::addControlPoint(const Vec2& controlPoint)
{    
    _controlPoints.push_back(controlPoint);
}

void PointArray::insertControlPoint(const Vec2& controlPoint, ssize_t index)
{
    _controlPoints.insert(std::next(_controlPoints.begin(), index), controlPoint);
}

const Vec2& PointArray::getControlPointAtIndex(ssize_t index) const
{
    index = MIN(static_cast<ssize_t>(_controlPoints.size())-1, MAX(index, 0));
    return _controlPoints.at(index);
}

void PointArray::replaceControlPoint(const Vec2& controlPoint, ssize_t index)
{
    _controlPoints.at(index) = controlPoint;
}

void PointArray::removeControlPointAtIndex(ssize_t index)
{
    vector<Vec2>::iterator iter = std::next(_controlPoints.begin(), index);
    _controlPoints.erase(iter);
}

ssize_t PointArray::count() const
{
    return _controlPoints.size();
}

PointArray* PointArray::reverse() const
{
    vector<Vec2> newArray;
    newArray.reserve(_controlPoints.size());
    for (auto iter = _controlPoints.rbegin(), iterRend = _controlPoints.rend(); iter != iterRend; ++iter)
    {
        newArray.push_back(*iter);
    }
    PointArray *config = PointArray::create(0);
    config->setControlPoints(std::move(newArray));
    
    return config;
}

void PointArray::reverseInline()
{
    const size_t l = _controlPoints.size();
    Vec2 *p1 = nullptr;
    Vec2 *p2 = nullptr;
    float x, y;
    for (size_t i = 0; i < l/2; ++i)
    {
        p1 = &_controlPoints.at(i);
        p2 = &_controlPoints.at(l-i-1);
        
        x = p1->x;
        y = p1->y;
        
        p1->x = p2->x;
        p1->y = p2->y;
        
        p2->x = x;
        p2->y = y;
    }
}

// CatmullRom Spline formula:
Vec2 ccCardinalSplineAt(const Vec2 &p0, const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, float tension, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;
    
    /*
     * Formula: s(-ttt + 2tt - t)P1 + s(-ttt + tt)P2 + (2ttt - 3tt + 1)P2 + s(ttt - 2tt + t)P3 + (-2ttt + 3tt)P3 + s(ttt - tt)P4
     */
    float s = (1 - tension) / 2;

    float b1 = s * ((-t3 + (2 * t2)) - t);                      // s(-t3 + 2 t2 - t)P1
    float b2 = s * (-t3 + t2) + (2 * t3 - 3 * t2 + 1);          // s(-t3 + t2)P2 + (2 t3 - 3 t2 + 1)P2
    float b3 = s * (t3 - 2 * t2 + t) + (-2 * t3 + 3 * t2);      // s(t3 - 2 t2 + t)P3 + (-2 t3 + 3 t2)P3
    float b4 = s * (t3 - t2);                                   // s(t3 - t2)P4
    
    float x = (p0.x*b1 + p1.x*b2 + p2.x*b3 + p3.x*b4);
    float y = (p0.y*b1 + p1.y*b2 + p2.y*b3 + p3.y*b4);

    return Vec2(x,y);
}

NS_CC_END;
