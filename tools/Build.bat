rem Build.bat
@echo off
cls

rem Visual Studio .NET 2002 (Platform Toolset = 'v70')
rem Visual Studio .NET 2003 (Platform Toolset = 'v71')
rem Visual Studio 2005      (Platform Toolset = 'v80')
rem Visual Studio 2008      (Platform Toolset = 'v90')
rem Visual Studio 2010      (Platform Toolset = 'v100')
rem Visual Studio 2012      (Platform Toolset = 'v110')
rem Visual Studio 2013      (Platform Toolset = 'v120')
rem Visual Studio 2015      (Platform Toolset = 'v140')
rem Visual Studio 2017      (Platform Toolset = 'v141')

rem Initialize
:init
set WorkingDir=%cd%\
set MSBuildPath=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\
set VCTargetsPath=%MSBuildPath%Common7\IDE\VC\VCTargets\
set VCVarsPath=%MSBuildPath%VC\Auxiliary\Build\
set VCVars32=vcvars32.bat
set VCVars64=vcvars64.bat

set DefaultConfiguration=Debug
set DefaultPlatform=x86
set DefaultToolset=v141

rem Set Solution File
:setSolution
if "%1"=="" (
	set /P SolutionFile="Solution: "
) else (
	set SolutionFile=%1
)
set SolutionPath=%WorkingDir%%SolutionFile%
if exist "%SolutionPath%" (
	echo Set Solution %SolutionPath%
	echo OK
) else (
	echo %SolutionPath% does not exist.
	goto :setSolution
)


rem Set Configuration
:setConfiguration
if "%2"=="" (
	set /P Configuration="Configuration [%DefaultConfiguration%]: "
	if "%Configuration%"=="" (
		set Configuration=%DefaultConfiguration%
	)
) else (
	set Configuration=%2
)
echo Set Configuration %Configuration%


rem Set Platform Target
:setPlatformTarget
if "%3"=="" (
	set /P PlatformTarget="Platform Target [%DefaultPlatform%]: "
	if "%PlatformTarget%"=="" (
		set PlatformTarget=%DefaultPlatform%
	)
) else (
	set PlatformTarget=%3
)
echo Set Platform Target %PlatformTarget%


rem Setup Environment
:setupEnvironment
cd %VCVarsPath%
if "%PlatformTarget%"=="x86" (
	echo Running %VCVars32%...
	call %VCVars32%
) else (
	echo Running %VCVars64%...
	call %VCVars64%
)


rem Build
:build
cd %MSBuildPath%
msbuild.exe %SolutionPath% /p:Configuration=%Configuration% /p:PlatformTarget=%PlatformTarget% /p:PlatformToolset=%DefaultToolset%
exit %ERRORLEVEL%