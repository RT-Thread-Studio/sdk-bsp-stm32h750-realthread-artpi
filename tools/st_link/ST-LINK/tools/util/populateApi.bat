@echo off
%windir%\System32\xcopy /y /I /S "%~dp0/../bin/ExternalLoader" "%~dp0/../api/lib/ExternalLoader"
%windir%\System32\xcopy /y /I /S "%~dp0/../bin/FlashLoader" "%~dp0/../api/lib/FlashLoader"
%windir%\System32\xcopy /y /I /S "%~dp0/../Data_Base" "%~dp0/../api/Data_Base"
