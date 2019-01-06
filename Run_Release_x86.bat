@echo off

set TargetName=Demo
set Configuration=Release
set PlatformTarget=x86

set FilePath=.\bin\%Configuration%\%PlatformTarget%
set FileName=%TargetName%_%Configuration%_%PlatformTarget%.exe

if not exist %FilePath%\%FileName% (
	echo File Not Found: "%FilePath%\%FileName%"
	pause
	exit 1
)

cd %FilePath%
%FileName%

if "%ERRORLEVEL%"!="0" (
	pause
)
exit %ERRORLEVEL%