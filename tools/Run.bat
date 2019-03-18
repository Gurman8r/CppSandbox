@echo off

set TargetName=%1
set Configuration=%2
set PlatformTarget=%3

set FilePath=.\bin\%Configuration%\%PlatformTarget%
set FileName=%TargetName%_%Configuration%_%PlatformTarget%.exe

if not exist %FilePath%\%FileName% (
	echo File Not Found: "%FilePath%\%FileName%"
	pause
	exit 1
)

cd %FilePath%
start %FileName%
exit %ERRORLEVEL%