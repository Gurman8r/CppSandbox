rem Build.bat
@echo off
cls

rem Initialize
:init
set WorkingDir=%cd%\
set VSVersion=Enterprise
set MSBuildPath=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\%VSVersion%\
set MSBuildExe=msbuild.exe
set VCTargetsPath=%MSBuildPath%Common7\IDE\VC\VCTargets\
set VCVarsPath=%MSBuildPath%VC\Auxiliary\Build\
set VCVars32=vcvars32.bat
set VCVars64=vcvars64.bat


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
	set /P Configuration="Configuration [Debug]: "
	if "%Configuration%"=="" (
		set Configuration=Debug
	)
) else (
	set Configuration=%2
)
echo Set Configuration %Configuration%


rem Set Platform Target
:setPlatformTarget
if "%3"=="" (
	set /P PlatformTarget="Platform Target [x86]: "
	if "%PlatformTarget%"=="" (
		set PlatformTarget=x86
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
echo Running %MSBuildExe%...
%MSBuildExe% %SolutionPath% /p:Configuration=%Configuration% /p:PlatformTarget=%PlatformTarget%
if "%ERRORLEVEL%"!="0" (
	pause
)
exit %ERRORLEVEL%