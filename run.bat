@echo off
SETLOCAL EnableDelayedExpansion

:: set up helper for colored‑echo (https://stackoverflow.com/questions/21660249/how-do-i-make-one-particular-line-of-a-batch-file-a-different-color-then-the-oth):
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
    set "DEL=%%a"
)

:: * parsing arguments
set "BUILD_TYPE=Debug"
if "%~1"=="--release" (
    set "BUILD_TYPE=Release"
)
:: * parsing arguments end

:: * variables end

call :colorEcho 3 "Assuming Platform is Windows..."
cls

:: ensure build folder exists
if not exist build (
    mkdir build
)

call :colorEcho 6 "Generating build files using CMake preset..."
cmake --preset default-configure -DCMAKE_BUILD_TYPE=%BUILD_TYPE%

call :colorEcho 6 "Compiling using CMake preset..."
cmake --build --preset default-build --config %BUILD_TYPE%

call :colorEcho 5 "Executable Started..."

set "EXE_PATH=build\main.exe"

if not exist "%EXE_PATH%" (
    call :colorEcho 4 "Executable not found: %EXE_PATH%"
    exit /b 1
)

:: get start time
for /f "tokens=1-3 delims=:." %%a in ("%time%") do (
    set /a startsecs=%%a*3600 + %%b*60 + %%c
)

:: run executable
"%EXE_PATH%"

:: get end time
for /f "tokens=1-3 delims=:." %%a in ("%time%") do (
    set /a endsecs=%%a*3600 + %%b*60 + %%c
)

:: compute elapsed time
set /a elapsed=endsecs-startsecs
if %elapsed% LSS 0 set /a elapsed+=86400

call :colorEcho 5 "Elapsed Time - %elapsed% seconds"

endlocal
goto :eof

:: https://stackoverflow.com/questions/21660249/how-do-i-make-one-particular-line-of-a-batch-file-a-different-color-then-the-oth
:colorEcho
  rem %1 = color code (e.g. 1 = Blue, 2 = Green, 3 = Aqua, 4 = Red, 5 = Purple, 6 = Yellow, etc.)
  rem %~2 = message text
  <nul set /p ".=%DEL%" > "%~2"
  findstr /v /a:%1 /R "^$" "%~2" nul
  del "%~2" > nul 2>nul
  echo.
goto :eof
