@echo off

set TargetName=Demo
set Configuration=Release
set PlatformTarget=x64

set Path=.\bin\%Configuration%\%PlatformTarget%
set Name=%TargetName%_%Configuration%_%PlatformTarget%.exe
set File=%Path%\%Name%

if not exist %File% (
	echo File Not Found: "%File%"
	pause
	exit 1
)

echo %File%
cd %Path%
call %Name%
exit %ERRORLEVEL%