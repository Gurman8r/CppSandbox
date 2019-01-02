@echo off

set CurDir=%cd%/
set OutDir=%CurDir%
set OutExt=.zip
set OutName=test
set OutFile=%OutDir%%OutName%%OutExt%

cd C:\Program Files\7-Zip

7z a -r %OutFile% %CurDir%../config.ini
7z a -r %OutFile% %CurDir%../README.md
7z a -r %OutFile% %CurDir%../bin
7z a -r %OutFile% %CurDir%../assets

pause
exit 0