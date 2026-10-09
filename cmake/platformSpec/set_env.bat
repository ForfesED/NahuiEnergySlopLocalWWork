
@echo Setup enviroment variables for Nau Engine

@echo off
@rem set variables
for %%I in ("%~dp0.") do for %%J in ("%%~dpI.") do set SourcesFolder=%%~dpJ
set "SourcesFolder=%SourcesFolder:\=/%"
>NUL SETX NAU_ENGINE_SOURCE_DIR %SourcesFolder%


@echo on

@rem print variables
@echo NAU_ENGINE_SOURCE_DIR = %SourcesFolder%