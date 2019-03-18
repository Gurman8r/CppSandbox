@echo off

set TargetName=%1
set Configuration=%2
set PlatformTarget=%3

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
if "%Configuration%"=="Release" (
	start %Name%
) else ( 
	call %Name%
)

exit %ERRORLEVEL%