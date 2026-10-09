// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include <string>
#include "nau/shared/api.h"
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
namespace nau
{
    class SHARED_API WindowsProcess
    {
    public:
        int runProcess(const std::string& args, class Logger* logger = nullptr);
        std::string getProcessOutput() const { return processOutput.str(); }
    private:
        std::stringstream processOutput;
    };

    typedef WindowsProcess IProcessWorker;
}
#endif