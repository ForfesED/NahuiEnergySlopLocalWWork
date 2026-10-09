// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#pragma once

#define NAU_RUN_JOB(JobType, Description)                                                                                                 \
    IJob* job = new JobType;                                                                                                              \
    const int exitCode = job->run(args);                                                                                                  \
    exitCode != 0 ? LOG_ERROR("Exit code {}\nFailed to run job ({})", exitCode, job->error()) : LOG_INFO(Description, args->projectPath); \
    delete job;                                                                                                                           \
    return exitCode;

#define NAU_RUN_JOB_WITH_APP(JobType, Description, App)                                                                                   \
    App->startupOnCurrentThread();                                                                                                        \
    IJob* job = new JobType;                                                                                                              \
    const int exitCode = job->run(args);                                                                                                  \
    exitCode != 0 ? LOG_ERROR("Exit code {}\nFailed to run job ({})", exitCode, job->error()) : LOG_INFO(Description, args->projectPath); \
    delete job;                                                                                                                           \
    App->stop();                                                                                                                          \
    while (app->step())                                                                                                                   \
    {                                                                                                                                     \
        std::this_thread::yield();                                                                                                        \
    };                                                                                                                                    \
    return exitCode;

#define NAU_DEFINE_COMPILERS static std::map<std::string, std::shared_ptr<compilers::IAssetCompiler>> g_compilers = {
#define NAU_COMPILER(name, comp) { name, std::make_shared<comp>() }
#define NAU_CLOSE_COMPILERS };