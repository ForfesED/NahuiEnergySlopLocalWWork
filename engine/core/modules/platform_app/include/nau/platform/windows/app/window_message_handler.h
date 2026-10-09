// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.
// nau/platform/windows/app/windows_window.h


#pragma once

#include "nau/platform/windows/windows_headers.h"
#include "nau/rtti/rtti_object.h"

namespace nau
{

    /**
     * Api for per window events handlers.
     */
    struct NAU_ABSTRACT_TYPE IWindowMessageHandler
    {
        NAU_TYPEID(nau::IWindowMessageHandler)

        virtual bool handleMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
    };

    /**
        Api for application main window loop handlers.
     */
    struct NAU_ABSTRACT_TYPE IWindowsApplicationMessageHandler
    {
        NAU_TYPEID(nau::IWindowsApplicationMessageHandler)

        enum PreDispatchMsgResult
        {
            Normal,
            QuitApp,
            SkipMessage
        };

        virtual PreDispatchMsgResult preDispatchMsg(MSG& msg) = 0;

        virtual void postDispatchMsg(const MSG& msg) = 0;
    };

}  // namespace nau
