@echo off
cls
Color 0E

REM Check admin rights
NET SESSION >nul 2>&1
IF %ERRORLEVEL% EQU 0 (
    ECHO Running in admin mode
) ELSE (
    ECHO You need to run in admin mode
	pause
	goto:eof
)

echo.
echo This script and profiler will stop when the target app finishes the execution. 
echo Then open created vspx file with VisualStudio to analyze at the end
echo.
pause

SET TARGET_EXECUTABLE=liteExchange.exe
REM Change your drive letter accordingly
SET PROFILER=vsperf
REM
REM MSDN Link for using vsperf from command line : https://msdn.microsoft.com/en-us/library/dd255377(v=vs.140).aspx
REM For VS 2017 commnad prompt
SET VS_CMD_BAT="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
if exist %VS_CMD_BAT% (
	echo VisualStudio commmand prompt found
) else (
    echo VisualStudio commmand prompt not found && pause && exit 1
)
call %VS_CMD_BAT%
REM 
SET TARGET_EXECUTABLE_DIRECTORY=%~dp0..\bin
echo %TARGET_EXECUTABLE_DIRECTORY%
REM
REM By default, performance data is sampled every 10,000,000 non-halted processor clock cycles.
REM This is approximately one time every 10 seconds on a 1GHz processor
REM
%PROFILER% /stop:sample
%PROFILER% /start:sample
%PROFILER% /launch:%TARGET_EXECUTABLE_DIRECTORY%\%TARGET_EXECUTABLE% /console
