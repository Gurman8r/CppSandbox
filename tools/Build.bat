rem Build.bat
@echo off
cls

rem Store Working Directory
set WorkingDir=%cd%\

rem Set Solution File
:setSolution
if "%1"=="" (
	set /P SolutionFile="Enter Solution: "
) else (
	set SolutionFile=%1
)

set SolutionPath=%WorkingDir%%SolutionFile%
if exist "%SolutionPath%" (
	echo Solution = %SolutionPath%
	echo OK
) else (
	echo %SolutionPath% does not exist.
	goto :setSolution
)


rem Set Configuration
:setConfiguration
if "%2"=="" (
	set /P Configuration="Enter Configuration (Debug): "
	if "%Configuration%"=="" (
		set Configuration="Debug"
	)
) else (
	set Configuration=%2
)
echo Configuration = %Configuration%


rem Set Platform Target
:setPlatformTarget
if "%3"=="" (
	set /P PlatformTarget="Enter Platform Target (x86): "
	if "%PlatformTarget%"=="" (
		set PlatformTarget="x86"
	)
) else (
	set PlatformTarget=%3
)
echo PlatformTarget = %PlatformTarget%


rem Setup Environment
set MSBuildPath=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\
set VCVarsPath=%MSBuildPath%VC\Auxiliary\Build\
set VCTargetsPath=%MSBuildPath%Common7\IDE\VC\VCTargets\

cd %VCVarsPath%
if %PlatformTarget%=="x86" (
	echo Running vcvars32.bat...
	call vcvars32.bat
) else (
	echo Running vcvars64.bat...
	call vcvars64.bat
)


rem Build
cd %MSBuildPath%
echo Running msbuild.exe...
msbuild.exe %SolutionPath% /p:Configuration=%Configuration% /p:PlatformTarget=%PlatformTarget%

echo Status: %ERRORLEVEL%
pause
exit %ERRORLEVEL%