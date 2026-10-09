
#ifndef GAINPUTINPUTDEVICEMOUSEWIN_H_
#define GAINPUTINPUTDEVICEMOUSEWIN_H_

#include "GainputInputDeviceMouseImpl.h"
#include <gainput/GainputHelpers.h>

#include "../GainputWindows.h"

namespace gainput
{

    class InputDeviceMouseImplWin : public InputDeviceMouseImpl
    {
    public:
        InputDeviceMouseImplWin(InputManager& manager, InputDevice& device, InputState& state, InputState& previousState) :
            manager_(manager),
            device_(device),
            state_(&state),
            previousState_(&previousState),
            nextState_(manager.GetAllocator(), MouseButtonCount + MouseAxisCount),
            delta_(0)
        {
        }

        InputDevice::DeviceVariant GetVariant() const
        {
            return InputDevice::DV_STANDARD;
        }

        void Update(InputDeltaState* delta)
        {
            delta_ = delta;
            *state_ = nextState_;

            // We need only a delta for wheel movement, but GA rely on absolute axis values in button state.
            // So, just modify a previous state on the fly
            if (wheelDelta_ != 0)
            {
                previousState_->Set(MouseAxisWheel, (float)wheelDelta_ / (float)WHEEL_DELTA);
                wheelDelta_ = 0;
            }
        }

        void HandleMessage(const MSG& msg)
        {
            GAINPUT_ASSERT(state_);
            GAINPUT_ASSERT(previousState_);

            DeviceButtonId buttonId;
            bool pressed = false;
            bool moveMessage = false;
            bool wheelMessage = false;
            int ax = -1;
            int ay = -1;
            int wd = 0;
            switch (msg.message)
            {
                case WM_LBUTTONDOWN:
                    buttonId = MouseButtonLeft;
                    pressed = true;
                    break;
                case WM_LBUTTONUP:
                    buttonId = MouseButtonLeft;
                    pressed = false;
                    break;
                case WM_RBUTTONDOWN:
                    buttonId = MouseButtonRight;
                    pressed = true;
                    break;
                case WM_RBUTTONUP:
                    buttonId = MouseButtonRight;
                    pressed = false;
                    break;
                case WM_MBUTTONDOWN:
                    buttonId = MouseButtonMiddle;
                    pressed = true;
                    break;
                case WM_MBUTTONUP:
                    buttonId = MouseButtonMiddle;
                    pressed = false;
                    break;
                case WM_XBUTTONDOWN:
                    buttonId = MouseButton4 + GET_XBUTTON_WPARAM(msg.wParam);
                    pressed = true;
                    break;
                case WM_XBUTTONUP:
                    buttonId = MouseButton4 + GET_XBUTTON_WPARAM(msg.wParam);
                    pressed = false;
                    break;
                case WM_MOUSEMOVE:
                    moveMessage = true;
                    ax = GET_X_LPARAM(msg.lParam);
                    ay = GET_Y_LPARAM(msg.lParam);
                    break;
                case WM_MOUSEWHEEL:
                {
                    wheelMessage = true;
                    wd = GET_WHEEL_DELTA_WPARAM(msg.wParam);
                    break;
                }
                default:  // Non-mouse message
                    return;
            }

            if (moveMessage)
            {
                float x = float(ax) / float(manager_.GetDisplayWidth());
                float y = float(ay) / float(manager_.GetDisplayHeight());
                HandleAxis(device_, nextState_, delta_, MouseAxisX, x);
                HandleAxis(device_, nextState_, delta_, MouseAxisY, y);
            }
            else if (wheelMessage)
            {
                wheelDelta_ += wd;
            }
            else
            {
                HandleButton(device_, nextState_, delta_, buttonId, pressed);
            }
        }

    private:
        InputManager& manager_;
        InputDevice& device_;
        InputState* state_;
        InputState* previousState_;
        InputState nextState_;
        InputDeltaState* delta_;
        int wheelDelta_ = 0;
    };

}  // namespace gainput

#endif
