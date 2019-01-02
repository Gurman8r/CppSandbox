@echo off

set ExeName=%1
set ExeConf=%2
set ExeCPU=%3

set CurDir=%cd%/
set OutDir=%CurDir%

set ArchiveName=%ExeName%_%ExeConf%_%ExeCPU%
set ArchiveType=.zip

set OutFile=%OutDir%%ArchiveName%%ArchiveType%
set RunFile=%ArchiveName%.bat

set ZipDir=%ProgramFiles%\7-Zip
cd %ZipDir%

rem 7z a 	%OutFile% %CurDir%%RunFile%
7z a 	%OutFile% %CurDir%%ExeName%_%ExeConf%_%ExeCPU%.bat
7z a 	%OutFile% %CurDir%../config.ini
7z a 	%OutFile% %CurDir%../README.md
7z a -r %OutFile% %CurDir%../assets/
7z a -r %OutFile% %CurDir%../*%ExeConf%_%ExeCPU%.exe*
7z a -r %OutFile% %CurDir%../*%ExeConf%_%ExeCPU%.dll*

pause
exit 0