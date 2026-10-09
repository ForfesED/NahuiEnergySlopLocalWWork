@echo off
pushd "%~dp0"

set FindVCCmd="%~dp0find_vc.cmd"

where /Q cl
IF %ERRORLEVEL% EQU 0 goto done

popd

set VS_VER=%1
if "%VS_VER%" == "" (
  set VS_VER=17
)

set /A VS_VER_NEXT=VS_VER+1
set TargetVSInstallDir=empty

for /f "usebackq skip=4 tokens=*" %%i in (`call %FindVCCmd% %VS_VER%.0 %VS_VER_NEXT%.0`) do (
  set TargetVSInstallDir=%%i
)

if "%TargetVSInstallDir%" == "empty" (
	goto noVS
)

pushd "%~dp0"

set DevCmdDir=Common7\Tools
set DevCmd=VsDevCmd.bat
set DevCmdFullPath="%TargetVSInstallDir%\%DevCmdDir%\%DevCmd%"

if not exist %DevCmdFullPath% (
	goto noVS
)

echo Visual Studio Installation: [%TargetVSInstallDir%].
echo Visual Studio dev tools: [%DevCmdFullPath%].

call %DevCmdFullPath% -arch=x64 -host_arch=x64

goto done

:noVS
echo No any Visual Studio (%VS_VER%) Installation found

:done
popd
