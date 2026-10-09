#/****************************************************************************
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

if(WINDOWS)
    set(COCOS_PLATFORM_SPECIFIC_HEADER
        platform/win32/compat/stdint.h
        platform/win32/inet_pton_mingw.h
        platform/win32/CCFileUtils-win32.h
        platform/win32/CCStdC-win32.h
        platform/win32/CCPlatformDefine-win32.h
        platform/win32/CCUtils-win32.h
        )
    set(COCOS_PLATFORM_SPECIFIC_SRC
        platform/win32/CCStdC-win32.cpp
        platform/win32/CCUtils-win32.cpp
        platform/win32/CCFileUtils-win32.cpp
        platform/win32/CCDevice-win32.cpp
        platform/win32/inet_pton_mingw.cpp
        )
endif()

set(COCOS_PLATFORM_HEADER
    ${COCOS_PLATFORM_SPECIFIC_HEADER}
    platform/CCApplicationProtocol.h
    platform/CCCommon.h
    platform/CCDevice.h
    platform/CCFileUtils.h
    platform/CCRenderView.h
    platform/CCImage.h
    platform/CCPlatformConfig.h
    platform/CCPlatformDefine.h
    platform/CCPlatformMacros.h
    platform/CCSAXParser.h
    platform/CCStdC.h
    )

set(COCOS_PLATFORM_SRC
    ${COCOS_PLATFORM_SPECIFIC_SRC}
    platform/CCSAXParser.cpp
    platform/CCRenderView.cpp
    platform/CCFileUtils.cpp
    platform/CCImage.cpp
    )
