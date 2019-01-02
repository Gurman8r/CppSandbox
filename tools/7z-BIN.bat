@echo off

set ExeName=Demo
set ExeConf=Debug
set ExeCPU=x86

set ProgDir=C:\Program Files\7-Zip
set CurDir=%cd%/
set OutDir=%CurDir%

set ArchiveName=%ExeName%_%ExeConf%_%ExeCPU%
set ArchiveType=.zip

set OutFile=%OutDir%%ArchiveName%%ArchiveType%
set RunFile=%ArchiveName%.bat

cd %ProgDir%

7z a 	%OutFile% %CurDir%%RunFile%
7z a 	%OutFile% %CurDir%../config.ini
7z a 	%OutFile% %CurDir%../README.md
7z a -r %OutFile% %CurDir%../bin
7z a -r %OutFile% %CurDir%../assets

pause
exit 0