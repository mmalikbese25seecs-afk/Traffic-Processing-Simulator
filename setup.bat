@echo off
SETLOCAL EnableDelayedExpansion

:: set up helper for colored‑echo (https://stackoverflow.com/questions/21660249/how-do-i-make-one-particular-line-of-a-batch-file-a-different-color-then-the-oth):
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
    set "DEL=%%a"
)

:: update submodules
call :colorEcho 3 "Updating git submodules ..."
git submodule update --init --recursive

call :colorEcho 3 "Assuming Platform is Windows ..."

:: bootstrap vcpkg if needed
if not exist "external\vcpkg\vcpkg.exe" (
    call :colorEcho 6 "Bootstrapping vcpkg ..."
    call external\vcpkg\bootstrap-vcpkg.bat
) else (
    call :colorEcho 2 "vcpkg already bootstrapped."
)

:: install packages
call :colorEcho 3 "Installing packages ..."
external\vcpkg\vcpkg.exe install raylib
:: CHANGE: ADD YOUR PACKAGES HERE; vcpkg-install package_name
call :colorEcho 5 "To install packages run"
call :colorEcho 5 "external\vcpkg\vcpkg.exe install package_name"

:: setup cmake preset; REMOVE after setting up preset
cmake --preset default-configure

call :colorEcho 5 "In VS Code, open Command Palette and select 'CMake Select Build Preset' - 'Default'"
call :colorEcho 5 "After successful setup, syntax highlighting and other features will activate."

call :colorEcho 3 "Run run.bat to build and run the program."
call :colorEcho 2 "Setup complete!"

endlocal
goto :eof

:: https://stackoverflow.com/questions/21660249/how-do-i-make-one-particular-line-of-a-batch-file-a-different-color-then-the-oth
:colorEcho
  rem %1 = color code (e.g. 1=Blue, 2=Green, 3=Aqua, 4=Red, 5=Purple, 6=Yellow)
  rem %~2 = message text
  <nul set /p ".=%DEL%" > "%~2"
  findstr /v /a:%1 /R "^$" "%~2" nul
  del "%~2" > nul 2>nul
  echo.
goto :eof
