@echo off
REM Find latest Visual Studio using vswhere
for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    set "VSINSTALL=%%i"
)

call "%VSINSTALL%\VC\Auxiliary\Build\vcvars64.bat"
cmd
