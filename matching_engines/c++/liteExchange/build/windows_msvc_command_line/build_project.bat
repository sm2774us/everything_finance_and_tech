@echo off
cls
Color 0E
SET VS_COMMAND_PROMPT=%1
SET SOLUTION=%2
SET CONFIGURATION=%3
SET PLATFORM=%4
SET TARGET=%5
IF [%1] == [] GOTO display_usage
IF [%2] == [] GOTO display_usage
IF [%3] == [] GOTO display_usage
IF [%4] == [] GOTO display_usage
cd %~dp0
pushd %CD% 
REM SWITCH TO VS COMMAND PROMPT
call %VS_COMMAND_PROMPT%
REM Arrange working directory
popd
IF [%5] == [] GOTO build_without_target
IF NOT [%5] == [] GOTO build_with_target
GOTO End

:build_with_target
echo Building with target
msbuild %SOLUTION% /t:%TARGET% /p:Configuration=%CONFIGURATION% /property:Platform=%PLATFORM%
GOTO End
:build_without_target
echo Building without target
msbuild %SOLUTION% /p:Configuration=%CONFIGURATION% /property:Platform=%PLATFORM%
GOTO End
:display_usage
echo Do not directly call this file. You should use build_clean, build_debug or build_release
:End
pause