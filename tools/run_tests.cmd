setlocal
set ToolsDir=%~dp0
set projectPath=%ToolsDir%\project_tool
set sdkPath=%ToolsDir%..\
set VersionToUpgrade=2.0
cd 	%projectPath%
dotnet restore
dotnet test tests\tests.csproj
cmd /k