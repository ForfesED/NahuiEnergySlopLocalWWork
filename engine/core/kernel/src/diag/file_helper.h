// Copyright 2024 N-GINN LLC. All rights reserved.
// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <cerrno>
#include <chrono>
#include <cstdio>
#include <string>
#include <thread>
#include <tuple>

#include "EASTL/functional.h"
#include "EASTL/string.h"
#include "nau/io/file_system.h"
#include "nau/service/service_provider.h"

namespace spdlog
{
    using memory_buf_t = eastl::string;
    using stream_t = nau::io::IStreamWriter::Ptr;
    // TODO: use nau::io::IFile::Ptr when it support file writing.
    using file_t = stream_t;

    struct file_event_handlers
    {
        file_event_handlers() :
            before_open(nullptr),
            after_open(nullptr),
            before_close(nullptr),
            after_close(nullptr)
        {
        }

        eastl::function<void(eastl::string_view filename)> before_open;
        eastl::function<void(eastl::string_view filename, file_t file_stream)> after_open;
        eastl::function<void(eastl::string_view filename, file_t file_stream)> before_close;
        eastl::function<void(eastl::string_view filename)> after_close;
    };

    namespace details
    {

        // Helper class for file sinks.
        // When failing to open a file, retry several times(5) with a delay interval(10 ms).
        // Throw spdlog_ex exception on errors.

        NAU_FORCE_INLINE eastl::string spdlog_ex(const eastl::string& msg, int last_errno)
        {
            fmt::basic_memory_buffer<char, 250> outbuf;
            fmt::format_system_error(outbuf, last_errno, msg.c_str());
            return eastl::string(outbuf.data(), outbuf.size());
        }

        NAU_FORCE_INLINE void throw_spdlog_ex(const eastl::string& msg, int last_errno)
        {
            NAU_FATAL_FAILURE(spdlog_ex(msg, last_errno));
        }

        NAU_FORCE_INLINE void throw_spdlog_ex(const eastl::string& msg)
        {
            NAU_FATAL_FAILURE(msg);
        }

        class NAU_KERNEL_EXPORT file_helper
        {
        public:
            explicit file_helper(eastl::string_view fname, const file_event_handlers& event_handlers = {});

            file_helper(const file_helper&) = delete;
            file_helper& operator=(const file_helper&) = delete;
            ~file_helper();

            bool isOpen();
            void open();
            void flush();
            void sync();
            void close();
            void write(const memory_buf_t& buf);
            size_t size() const;
            eastl::string_view filename() const;

            //
            // return file path and its extension:
            //
            // "mylog.txt" => ("mylog", ".txt")
            // "mylog" => ("mylog", "")
            // "mylog." => ("mylog.", "")
            // "/dir1/dir2/mylog.txt" => ("/dir1/dir2/mylog", ".txt")
            //
            // the starting dot in filenames is ignored (hidden files):
            //
            // ".mylog" => (".mylog". "")
            // "my_folder/.mylog" => ("my_folder/.mylog", "")
            // "my_folder/.mylog.txt" => ("my_folder/.mylog", ".txt")
            static std::tuple<eastl::string, eastl::string> split_by_extension(eastl::string_view fname);

        private:
            const int open_tries_ = 1;
            // TODO: use file_t and VFS when they support file writing.
            //file_t fd_ = nullptr;
            stream_t sd_ = nullptr;
            eastl::string filename_;
            file_event_handlers event_handlers_;
            std::atomic<bool> filename_is_broken_ = false;
        };

    }  // namespace details
}  // namespace spdlog
