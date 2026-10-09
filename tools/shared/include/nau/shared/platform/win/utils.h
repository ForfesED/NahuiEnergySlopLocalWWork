// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include <string>

#include "nau/shared/api.h"

#if defined(_WIN32) || defined(_WIN64)
namespace nau
{
    class SHARED_API WindowsUtils
    {
    public:
        static void openFolder(const std::string& path);
        static bool createLink(const std::string& from, const std::string& to);
        static void appendPathEnv(const std::string& path);
        static int setenv(const char* name, const char* value, int overwrite);
    };

    typedef WindowsUtils IPlatformUtils;
}  // namespace nau
#endif