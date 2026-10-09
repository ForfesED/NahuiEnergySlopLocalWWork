// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once
#include <string>
#include "nau/shared/api.h"

namespace nau
{
    class IJob
    {
    public:
        virtual int run(const struct CommonArguments* arguments) = 0;
        virtual std::string error() = 0;
        virtual int exitCode() = 0;
    };

    class Job : public IJob
    {
    public:
        virtual std::string error() final override
        {
            return m_error;
        };
        virtual int exitCode() final override
        {
            return m_exitCode;
        };
    protected:
        int result(const std::string& error, int exitCode)
        {
            m_error = error;
            m_exitCode = exitCode;
            return exitCode;
        }
    private:
        std::string m_error;
        int m_exitCode = 0;
    };
}  // namespace nau