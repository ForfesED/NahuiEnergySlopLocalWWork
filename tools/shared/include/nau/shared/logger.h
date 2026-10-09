// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include <nau/diag/log_subscribers.h>
#include <nau/diag/logging.h>

#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <string>

#include "nau/diag/assertion.h"
#include "nau/diag/device_error.h"
#include "nau/shared/api.h"

namespace nau
{
    const struct LoggerTag
    {
        const char* m_tag;
    };

    // Temp before replaceing with internal engine logger
    #define LOG_INFO(msg, ...) logger::log(LoggerTag{NAU_TARGET_NAME}, NAU_INLINED_SOURCE_INFO, nau::diag::LogLevel::Info, msg __VA_OPT__(, ) __VA_ARGS__)
    #define LOG_WARN(msg, ...) logger::log(LoggerTag{NAU_TARGET_NAME}, NAU_INLINED_SOURCE_INFO, nau::diag::LogLevel::Warning, msg __VA_OPT__(, ) __VA_ARGS__)
    #define LOG_ERROR(msg, ...) logger::log(LoggerTag{NAU_TARGET_NAME}, NAU_INLINED_SOURCE_INFO, nau::diag::LogLevel::Error, msg __VA_OPT__(, ) __VA_ARGS__)
    #define LOG_FASSERT(cond, msg, ...) logger::fassert(LoggerTag{NAU_TARGET_NAME}, NAU_INLINED_SOURCE_INFO, cond, msg __VA_OPT__(, ) __VA_ARGS__)
    #define LOG_COND(condition, msg, ...) logger::cond(LoggerTag{NAU_TARGET_NAME}, NAU_INLINED_SOURCE_INFO, condition, msg __VA_OPT__(, ) __VA_ARGS__)

    namespace logger
    {
        SHARED_API void init(std::string_view output = "", bool verbosity = true);
        SHARED_API void writeOutput(nau::diag::LogLevel level, const std::string& fmt, const LoggerTag& tag, const nau::diag::SourceInfo& info);
        SHARED_API void addConsoleOutput(bool verbosity = true);
        SHARED_API nau::diag::Logger::Ptr getLogger();

        template <typename... args>
        void log(const LoggerTag& tag, const nau::diag::SourceInfo& info, nau::diag::LogLevel level, const std::string_view& fmt, const args&... params)
        {
            std::string out;

            constexpr size_t num = sizeof...(params);

            if constexpr (num == 0)
            {
                out = fmt;
            }
            else
            {
                out = std::vformat(fmt, std::make_format_args(params...));
            }

            writeOutput(level, out, tag, info);
        }

        template <typename... args>
        void fassert(const LoggerTag& tag, const nau::diag::SourceInfo& info, bool condition, const std::string_view& fmt, args&&... params)
        {
            if (condition)
            {
                log(tag, info, nau::diag::LogLevel::Critical, fmt, std::forward<args>(params)...);
                exit(1);
            }
        }

        template <typename... args>
        void cond(const LoggerTag& tag, const nau::diag::SourceInfo& info, bool condition, const std::string_view& fmt, args&&... params)
        {
            if (condition)
            {
                log(tag, info, nau::diag::LogLevel::Info, fmt, std::forward<args>(params)...);
            }
        }
    }  // namespace logger
};  // namespace nau