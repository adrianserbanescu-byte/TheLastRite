@echo off
setlocal

set "PROJECT_ROOT=%OneDrive%\Desktop\alexs"
if not exist "%PROJECT_ROOT%\TheLastRite.uproject" set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"

set "OUTPUT_DIR=%PROJECT_ROOT%\Packaged"

echo Packaging game from:
echo %PROJECT_ROOT%
echo.
echo Output folder:
echo %OUTPUT_DIR%
echo.

call "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun ^
 -project="%PROJECT_ROOT%\TheLastRite.uproject" ^
 -noP4 ^
 -platform=Win64 ^
 -clientconfig=Shipping ^
 -build ^
 -cook ^
 -stage ^
 -pak ^
 -archive ^
 -archivedirectory="%OUTPUT_DIR%"

echo.
echo Done.
pause
