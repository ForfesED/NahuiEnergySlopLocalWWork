// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#include <string>
#include <functional>

#include "nau/ui/elements/node.h"
#include "nau/math/math.h"
#include "nau/rtti/ptr.h"
#include "nau/scene/nau_object.h"
#include "nau/scene/scene_object.h"


namespace nau::ui
{
    
    /**
     * @brief Enumerates GUI element input event types.
     */
    enum class EventType 
    {
        press,  ///< Indicates that the element is being pressed on with the cursor.
        release,///< Indicates that the element has just stopped being pressed on with the cursor.
        hover,  ///< Indicates that the cursor is currently hovered over it.
        leave   ///< Indicates that the cursor has just exited the area of the element and it is no more hovered over.
    };

    /**
     * @brief Enumerates GUI element states.
     */
    enum class UIState 
    {
        normal,     ///< Default (enabled) state of the element when it is not receiving any cursor input.
        hovered,    ///< The state the element transitions to when the cursor is hovered over it.
        pressed,    ///< The state the element transitions to when is is being pressed on with the cursor.
        disabled    ///< The state the element transitions to when it gets disabled.
    };

    /**
     * @brief Encapsulates horizontal alignment types.
     */
    enum class HorizontalAlignment
    {
        left,
        center,
        right
    };

    /**
     * @brief Encapsulates vertical alignment types.
     */
    enum class VerticalAlignment
    {
        top,
        center,
        bottom
    };

/**
 * @brief Manages GUI element state and input events.
 */
class NAU_UI_EXPORT UIControl : public Node
{
public:
    /**
     * A functor type that is called upon EventType::press event triggering.
     */
    using OnPressedCallback = std::function<void(math::vec2 mousePositionLocal)>;

    /**
     * @brief A functor type that is called upon EventType::release event triggering.
     */
    using OnReleasedCallback = std::function<void()>;

    /**
     * @brief A functor type that is called upon EventType::hover event triggering.
     */
    using OnHoverCallback = std::function<void(math::vec2 mousePositionLocal)>;

    /**
     * @brief A functor type that is called upon EventType::leave event triggering.
     */
    using OnLeaveCallback = std::function<void()>;

    /**
     * @brief A functor type that is called when the cursor is held and moved within the element borders.
     */
    using TouchMovedCallback = std::function<void(math::vec2 mousePositionLocal, math::vec2 delta)>;

    /**
     * @brief Default constructor.
     */
    UIControl();

    /**
     * @brief Destructor.
     */
    virtual ~UIControl();

    /**
     * @brief Changes the callback for EventType::press event.
     * 
     * @param [in] cb Callback to set.
     */
    FORCEINLINE void setOnPressedCallback(OnPressedCallback cb) { m_onPressed = cb; };

    /**
     * @brief Changes the callback for EventType::release event.
     * 
     * @param [in] cb Callback to set.
     */
    FORCEINLINE void setOnReleasedCallback(OnReleasedCallback cb) { m_onReleased = cb; };

    /**
     * @brief Changes the callback for EventType::hover event.
     * 
     * @param [in] cb Callback to set.
     */
    FORCEINLINE void setOnHoverCallback(OnHoverCallback cb) { m_onHover = cb; };

    /**
     * @brief Changes the callback for EventType::leave event.
     * 
     * @param [in] cb Callback to set.
     */
    FORCEINLINE void setOnLeaveCallback(OnLeaveCallback cb) { m_onLeave = cb; };

    /**
     * @brief Changes the callback that is dispatched when the cursor is moved within the element borders.
     * 
     * @param [in] cb Callback to set.
     */
    FORCEINLINE void setOnTouchMovedCallback(TouchMovedCallback cb) { m_touchMovedCallback = cb; };

    /**
     * @brief Changes whether child interactable area should be restricted to the element area.
     * 
     * @param [in] isRestrict `true` if children should be interactable only within the element borders, `false` otherwise.
     * 
     * If `false` is passed, then cursor events can be triggered over the entire child area.
     * If `true` is passed, then cursor events can be triggered only over the part of child area that is inside the parent (this) element area.
     */
    FORCEINLINE void setInputRestrictForChild(bool isRestrict) { m_inNeedRestrictInputForChildWidgets = isRestrict; };

    /**
     * @brief Changes whether the GUI element can be interacted with (i.e. whether it can trigger cursor events).
     * 
     * @param [in] interactable  Indicates whether the GUI element should be interactable.
     */
    virtual void setInteractable(bool interactable);

    /**
     * @brief Checks whether the element can be interacted with (i.e. whether it can trigger cursor events).
     * 
     * @return `true` if the element is interactable, `false` otherwise.
     */
    virtual bool isInteractable() const;

    /**
     * @brief Checks whether the element is currently being pressed on (given that it is interactable at the cursor location point).
     * 
     * @return `true` if the EventType::press is currently being registered, `false` otherwise.
     */
    virtual bool isTouchCaptured () const;

    /**
     * @brief Checks whether the element is currently being hovered on (given that it is interactable at the cursor location point).
     *
     * @return `true` if the EventType::hover is currently being registered, `false` otherwise.
     */
    virtual bool isMouseCaptured() const;

    /**
     * @brief This function is called when any cursor event is triggered.
     * 
     * @param [in] eventType Type of the triggered event.
     * 
     * @note This is a callback with default no-op implementation. Users can provide their own overloads in the custom elemnts implementing UIControl.
     * @note Event-specific callbacks (like `OnPressed`) are called outside this function, so user do not need to call them manually.
     */
    virtual void handleEvent(nau::ui::EventType eventType);

protected:
    OnPressedCallback m_onPressed;
    OnReleasedCallback m_onReleased;
    OnHoverCallback m_onHover;
    OnLeaveCallback m_onLeave;
    TouchMovedCallback m_touchMovedCallback;

    bool m_interactable { true };
    bool m_inNeedRestrictInputForChildWidgets { false };

    virtual bool initialize() override;
    virtual bool isInputEventInElementBorder(math::vec2 inputPosition);

private:
    void addTouchListener();
    void releaseTouchListener();
    UIControl* getAncestorWidget(Node* node);
    bool isInteractableAndVisible(math::vec2 localInputPosition);
   
    bool m_touchCaptured {false};
    bool m_mouseCaptured {false};
};
    
}