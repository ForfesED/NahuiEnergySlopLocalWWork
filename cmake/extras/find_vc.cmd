set VSWHERE_="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
rem  add -prerelease option to allow Preview version
%VSWHERE_% -version [%1,%2) -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
