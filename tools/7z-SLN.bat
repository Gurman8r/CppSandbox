@echo off

set SlnName=CppSandbox

set ProgDir=C:\Program Files\7-Zip
set CurDir=%cd%/
set OutDir=%CurDir%

set ArchiveName=%SlnName%
set ArchiveType=.zip

set OutFile=%OutDir%%ArchiveName%%ArchiveType%
set RunFile=%ArchiveName%.bat

cd %ProgDir%

7z a 	%OutFile% %CurDir%../%SlnName%.sln
7z a 	%OutFile% %CurDir%../config.ini
7z a 	%OutFile% %CurDir%../README.md
7z a -r %OutFile% %CurDir%../assets
7z a -r %OutFile% %CurDir%../include
7z a -r %OutFile% %CurDir%../proj
7z a -r %OutFile% %CurDir%../src
7z a -r %OutFile% %CurDir%../thirdparty
pause
exit 0