set ToolsDir=%~dp0
set EnvVCCmd="%ToolsDir%..\cmake\extras\env_vc.cmd"
set VSVer=17
set Config=Release

call restore_packages.cmd
call %EnvVCCmd% %VSVer%

MSBuild.exe %ToolsDir%build_tool\NauTools.sln -t:Rebuild -p:Configuration=%Config%
MSBuild.exe %ToolsDir%project_tool\ProjectTools.sln -t:Rebuild -p:Configuration=%Config%