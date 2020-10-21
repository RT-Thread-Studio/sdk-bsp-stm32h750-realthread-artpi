@echo off
REM If enough rights, check the machine architecture
if /I "%PROCESSOR_ARCHITECTURE%"=="AMD64" goto AMD64
REM If here, we are running a 32-bit version of cmd; but still may be on a 64-bit machine
if /I "%PROCESSOR_ARCHITEW6432%"=="AMD64" goto AMD64
start "" "%~dp0/dpinst_x86.exe"
goto END

:AMD64
start "" "%~dp0/dpinst_amd64.exe"

:END