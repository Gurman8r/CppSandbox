@echo off

set ExePath=./bin
set ExeName=Demo
set ExeConf=Debug
set ExeCPU=x86

cd %ExePath%/%ExeConf%/%ExeCPU%

%ExeName%_%ExeConf%_%ExeCPU%.exe

exit 0