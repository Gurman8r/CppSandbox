@echo off

set Solution=..\CppSandbox.sln
set Configuration="Debug"
set PlatformTarget="x86"

set WorkingDir=%cd%\
set SLNPath=%WorkingDir%%Solution%
set MSBuildPath=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\
set VCVarsPath=%MSBuildPath%VC\Auxiliary\Build\
set VCTargetsPath=%MSBuildPath%Common7\IDE\VC\VCTargets\

cd %VCVarsPath%
call vcvars32.bat

cd %MSBuildPath%
msbuild.exe %SLNPath% /p:Configuration=%Configuration% /p:PlatformTarget=%PlatformTarget%

pause
exit 0