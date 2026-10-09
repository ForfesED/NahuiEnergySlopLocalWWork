// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include <filesystem>
#include <iostream>
#include <string>

#include "argparse/argparse.hpp"
#include "nau/asset_tools/asset_api.h"
#include "nau/shared/args.h"
#include "nau/shared/error_codes.h"
#include "nau/shared/file_system.h"
#include "nau/shared/logger.h"

namespace nau
{
    int parse(int argc, char** const& argv)
    {
        argparse::ArgumentParser programArgs("asset tool", NAU_VERSION);
        programArgs.add_argument("--log")
            .help("Log output path or current exe directory");
        programArgs.add_argument("--verbose")
            .default_value(false)
            .implicit_value(true)
            .help("Log level");

        argparse::ArgumentParser import("import");
        import.add_description("Compiles and built assets into assets database.");
        import.add_argument("--project")
            .required()
            .help("Project path");
        import.add_argument("--file")
            .default_value("")
            .help("Path to the specific file to import (if none, all asset will be scanned).");
        import.add_argument("--files_mask")
            .nargs(argparse::nargs_pattern::any)
            .help("Optional value to scan only specific files");

        programArgs.add_subparser(import);

        try
        {
            programArgs.parse_args(argc, argv);
        }
        catch (const std::exception& err)
        {
            std::cout << "Fatal! Could not parse arguments!" << std::endl;
            std::cerr << err.what() << std::endl;
            std::cerr << programArgs;

            return ErrorCode::invalidArgumentsError;
        }

        const std::string logPath = programArgs.is_used("--log") ? programArgs.get<std::string>("--log") : std::filesystem::current_path().string();
        const bool verbose = programArgs.is_used("--verbose");
        logger::init(logPath, verbose);

        // For console output
        logger::addConsoleOutput(verbose);

        try
        {
            if (programArgs.is_subcommand_used("import"))
            {
                auto args = std::make_unique<nau::ImportAssetsArguments>();

                args->projectPath = import.get<std::string>("--project");
                args->assetPath = import.get<std::string>("--file");
                args->filesExtensions = import.get<std::vector<std::string>>("--files_mask");

                LOG_INFO("Importing project assets at path {}...", args->projectPath);

                return importAssets(args.get());
            }
            else
            {
                LOG_ERROR("Unknown command.");
                return ErrorCode::invalidArgumentsError;
            }
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("Failed processing project: {}", e.what());

            return ErrorCode::invalidArgumentsError;
        }

        return ErrorCode::success;
    }
}  // namespace nau

int main(int argc, char** argv)
{
    return nau::parse(argc, argv);
}
