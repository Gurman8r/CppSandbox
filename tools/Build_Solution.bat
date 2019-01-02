@echo off

set FileName=%1
set Configuration=%2
set PlatformTarget=%3

set WorkingDir=%cd%\
set SolutionPath=%WorkingDir%%FileName%
set MSBuildPath=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\
set VCVarsPath=%MSBuildPath%VC\Auxiliary\Build\
set VCTargetsPath=%MSBuildPath%Common7\IDE\VC\VCTargets\

cd %VCVarsPath%
call vcvars32.bat

cd %MSBuildPath%
msbuild.exe %SolutionPath% /p:Configuration=%Configuration% /p:PlatformTarget=%PlatformTarget%

pause
exit 0