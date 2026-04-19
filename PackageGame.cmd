@echo off
setlocal

set "PROJECT_ROOT=%OneDrive%\Desktop\alexs"
if not exist "%PROJECT_ROOT%\TheLastRite.uproject" set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"

set "OUTPUT_DIR=%PROJECT_ROOT%\Packaged"
set "TEMP_ROOT=%LOCALAPPDATA%\TheLastRitePackage"
set "RUN_ID=%RANDOM%%RANDOM%"
set "RUN_ROOT=%TEMP_ROOT%\Run-%RUN_ID%"
set "LOG_DIR=%RUN_ROOT%\Logs"
set "ENGINE_SAVED_DIR=%RUN_ROOT%\EngineSaved"
set "LOCAL_DDC_DIR=%TEMP_ROOT%\DerivedDataCache"
set "ZEN_DATA_DIR=%TEMP_ROOT%\Zen\Data"
set "BUILD_LOG=%RUN_ROOT%\UBT-TheLastRiteShipping.log"
set "uebp_LogFolder=%LOG_DIR%"
set "uebp_EngineSavedFolder=%ENGINE_SAVED_DIR%"
set "UE-LocalDataCachePath=%LOCAL_DDC_DIR%"
set "UE-SharedDataCachePath=None"

if not exist "%TEMP_ROOT%" mkdir "%TEMP_ROOT%"
if not exist "%RUN_ROOT%" mkdir "%RUN_ROOT%"
if not exist "%LOG_DIR%" mkdir "%LOG_DIR%"
if not exist "%ENGINE_SAVED_DIR%" mkdir "%ENGINE_SAVED_DIR%"
if not exist "%LOCAL_DDC_DIR%" mkdir "%LOCAL_DDC_DIR%"
if not exist "%ZEN_DATA_DIR%" mkdir "%ZEN_DATA_DIR%"

echo Packaging game from:
echo %PROJECT_ROOT%
echo.
echo Output folder:
echo %OUTPUT_DIR%
echo.
echo Temp root:
echo %TEMP_ROOT%
echo.
echo Log folder:
echo %LOG_DIR%
echo.
echo Engine saved folder:
echo %ENGINE_SAVED_DIR%
echo.
echo Local derived data cache:
echo %LOCAL_DDC_DIR%
echo.
echo Shipping build log:
echo %BUILD_LOG%
echo.

call "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" TheLastRite Win64 Shipping -Project="%PROJECT_ROOT%\TheLastRite.uproject" -WaitMutex -log="%BUILD_LOG%"
if errorlevel 1 goto :done

call "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun ^
 -project="%PROJECT_ROOT%\TheLastRite.uproject" ^
 -noP4 ^
 -platform=Win64 ^
 -clientconfig=Shipping ^
 -skipbuildeditor ^
 -nobuild ^
 -cook ^
 -stage ^
 -pak ^
 -ddc=InstalledNoZenLocalFallback ^
 -archive ^
 -archivedirectory="%OUTPUT_DIR%"

:done
echo.
echo Done.
pause
