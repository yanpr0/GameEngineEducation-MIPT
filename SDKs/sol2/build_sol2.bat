setlocal enabledelayedexpansion
for /f "delims=" %%i in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe') do set msbuild_path=%%i

SET CL=/std:c++17 /MDd
SET _CL_=/std:c++17 /MDd

"%msbuild_path%" sol2\.build\sol2.sln /p:Configuration=Debug /p:Platform=x64

SET CL=/std:c++17 /MD
SET _CL_=/std:c++17 /MD

"%msbuild_path%" sol2\.build\sol2.sln /p:Configuration=Release /p:Platform=x64

xcopy sol2\.build\win64_vs2022\bin lib\x64 /Y
