/*
 * Copyright (c) 2008 Radu Gruian
 * Copyright (c) 2011 Vit Valentin
 * Copyright (c) 2012 cocos2d-x.org
 * Copyright (c) 2013-2016 Chukong Technologies Inc.
 * Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 *
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


#ifndef __CCACTION_CATMULLROM_H__
#define __CCACTION_CATMULLROM_H__

#include <vector>

#include "math/CCGeometry.h"

NS_CC_BEGIN;

class Node;

/**
 * @addtogroup actions
 * @{
 */

/** An Array that contain control points.
 * Used by CardinalSplineTo and (By) and CatmullRomTo (and By) actions.
 * @ingroup Actions
 * @js NA
 */
class CC_DLL PointArray : public Ref, public Clonable
{
public:

    /** Creates and initializes a Points array with capacity.
     * @js NA
     * @param capacity The size of the array.
     */
    static PointArray* create(ssize_t capacity);

    /**
     * @js NA
     * @lua NA
     */
    virtual ~PointArray();
    /**
     * @js NA
     * @lua NA
     */
    PointArray();

    /** Initializes a Catmull Rom config with a capacity hint.
     *
     * @js NA
     * @param capacity The size of the array.
     * @return True.
     */
    bool initWithCapacity(ssize_t capacity);

    /** Appends a control point.
     *
     * @js NA
     * @param controlPoint A control point.
     */
    void addControlPoint(const Vec2& controlPoint);

    /** Inserts a controlPoint at index.
     *
     * @js NA
     * @param controlPoint A control point.
     * @param index Insert the point to array in index.
     */
    void insertControlPoint(const Vec2& controlPoint, ssize_t index);

    /** Replaces an existing controlPoint at index.
     *
     * @js NA
     * @param controlPoint A control point.
     * @param index Replace the point to array in index.
     */
    void replaceControlPoint(const Vec2& controlPoint, ssize_t index);

    /** Get the value of a controlPoint at a given index.
     *
     * @js NA
     * @param index Get the point in index.
     * @return A Vec2.
     */
    const Vec2& getControlPointAtIndex(ssize_t index) const;

    /** Deletes a control point at a given index
     *
     * @js NA
     * @param index Remove the point in index.
     */
    void removeControlPointAtIndex(ssize_t index);

    /** Returns the number of objects of the control point array.
     *
     * @js NA
     * @return The number of objects of the control point array.
     */
    ssize_t count() const;

    /** Returns a new copy of the array reversed. User is responsible for releasing this copy.
     *
     * @js NA
     * @return A new copy of the array reversed.
     */
    PointArray* reverse() const;

    /** Reverse the current control point array inline, without generating a new one.
     * @js NA
     */
    void reverseInline();
    /**
     * @js NA
     * @lua NA
     */
    virtual PointArray* clone() const;
    /**
     * @js NA
     */
    const std::vector<Vec2>& getControlPoints() const;
    /**
     * @js NA
     */
    void setControlPoints(std::vector<Vec2> controlPoints);
private:
    /** Array that contains the control points. */
    std::vector<Vec2> _controlPoints;
};

/** Returns the Cardinal Spline position for a given set of control points, tension and time */
extern CC_DLL Vec2 ccCardinalSplineAt(const Vec2 &p0, const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, float tension, float t);



NS_CC_END;

#endif // __CCACTION_CATMULLROM_H__
