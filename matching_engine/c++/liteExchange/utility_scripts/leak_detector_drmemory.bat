@Echo off
cls
Color 0E
SET DR_MEMORY_PATH="C:\Program Files (x86)\Dr. Memory\bin64\"
SET DR_MEMORY_EXECUTABLE=drmemory.exe
setx PATH %DR_MEMORY_PATH%
if exist %DR_MEMORY_PATH%\%DR_MEMORY_EXECUTABLE% (
	echo DrMemory found
) else (
    echo DrMemory not found && pause && exit 1
)
SET TARGET_EXECUTABLE_DIRECTORY=%~dp0..\bin
SET TARGET_EXECUTABLE=liteExchange.exe
REM DrMemory arguments : http://www.drmemory.org/docs/page_options.html
%DR_MEMORY_EXECUTABLE% -leaks_only -logdir %~dp0 %TARGET_EXECUTABLE_DIRECTORY%\%TARGET_EXECUTABLE%
exit 0