@echo off
setlocal

set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"
set "PAUSE_AT_END=1"
if /I "%~1"=="--no-pause" set "PAUSE_AT_END="
set "UPROJECT=%PROJECT_ROOT%\TheLastRite.uproject"
set "UE_BUILD_BAT=C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat"
set "UE_RUN_UAT_BAT=C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\RunUAT.bat"

set "OUTPUT_DIR=%PROJECT_ROOT%\Packaged"
set "OUTPUT_WINDOWS_DIR=%OUTPUT_DIR%\Windows"
set "PACKAGED_EXE=%OUTPUT_WINDOWS_DIR%\TheLastRite.exe"
set "TEMP_ROOT=%PROJECT_ROOT%\LocalBuildTemp"
set "RUN_ID=%RANDOM%%RANDOM%"
set "UBT_ROOT=%TEMP_ROOT%\UBT-%RUN_ID%"
set "LOG_DIR=%TEMP_ROOT%\UATLogs-%RUN_ID%"
set "ENGINE_SAVED_DIR=%TEMP_ROOT%\UATSaved-%RUN_ID%"
set "STAGE_DIR=%TEMP_ROOT%\Stage-%RUN_ID%"
set "LOCAL_DDC_DIR=%TEMP_ROOT%\DerivedDataCache"
set "ZEN_DATA_DIR=%TEMP_ROOT%\Zen\Data"
set "BUILD_LOG=%UBT_ROOT%\UBT-TheLastRiteShipping.log"
set "UE-LocalDataCachePath=%LOCAL_DDC_DIR%"
set "UE-SharedDataCachePath=None"

if not exist "%UPROJECT%" (
    echo Project file not found:
    echo %UPROJECT%
    set "EXIT_CODE=1"
    goto :done
)

if not exist "%UE_BUILD_BAT%" (
    echo Unreal build script not found:
    echo %UE_BUILD_BAT%
    set "EXIT_CODE=1"
    goto :done
)

if not exist "%UE_RUN_UAT_BAT%" (
    echo Unreal automation script not found:
    echo %UE_RUN_UAT_BAT%
    set "EXIT_CODE=1"
    goto :done
)

if not exist "%TEMP_ROOT%" mkdir "%TEMP_ROOT%"
if not exist "%UBT_ROOT%" mkdir "%UBT_ROOT%"
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
echo Stage folder:
echo %STAGE_DIR%
echo.
echo Local derived data cache:
echo %LOCAL_DDC_DIR%
echo.
echo Shipping build log:
echo %BUILD_LOG%
echo.

echo Closing stale packaged game processes...
taskkill /IM TheLastRite.exe /F >nul 2>&1
taskkill /IM TheLastRite-Win64-Shipping.exe /F >nul 2>&1

echo Cleaning stale cook and cache folders...
if exist "%PROJECT_ROOT%\Saved\Cooked\Windows" rmdir /S /Q "%PROJECT_ROOT%\Saved\Cooked\Windows" >nul 2>&1
if exist "%PROJECT_ROOT%\Saved\Cooked\_Del" rmdir /S /Q "%PROJECT_ROOT%\Saved\Cooked\_Del" >nul 2>&1
if exist "%PROJECT_ROOT%\Saved\StagedBuilds\Windows" rmdir /S /Q "%PROJECT_ROOT%\Saved\StagedBuilds\Windows" >nul 2>&1
if exist "%PROJECT_ROOT%\Intermediate\Staging" rmdir /S /Q "%PROJECT_ROOT%\Intermediate\Staging" >nul 2>&1
if exist "%PROJECT_ROOT%\Intermediate\CachedAssetRegistry_0.bin" del /F /Q "%PROJECT_ROOT%\Intermediate\CachedAssetRegistry_0.bin" >nul 2>&1

if exist "%OUTPUT_WINDOWS_DIR%" (
    echo Removing previous packaged Windows build...
    rmdir /S /Q "%OUTPUT_WINDOWS_DIR%" >nul 2>&1
)

call "%UE_BUILD_BAT%" TheLastRite Win64 Shipping -Project="%UPROJECT%" -WaitMutex -log="%BUILD_LOG%"
if errorlevel 1 goto :done

set "uebp_LogFolder=%LOG_DIR%"
set "uebp_EngineSavedFolder=%ENGINE_SAVED_DIR%"

call "%UE_RUN_UAT_BAT%" BuildCookRun ^
 -project="%UPROJECT%" ^
 -noP4 ^
 -platform=Win64 ^
 -clientconfig=Shipping ^
 -skipbuildeditor ^
 -nobuild ^
 -cook ^
 -stage ^
 -stagingdirectory="%STAGE_DIR%" ^
 -pak ^
 -ddc=InstalledNoZenLocalFallback ^
 -archive ^
 -archivedirectory="%OUTPUT_DIR%"
if errorlevel 1 goto :done

if not exist "%PACKAGED_EXE%" (
    echo Expected packaged launcher not found:
    echo %PACKAGED_EXE%
    set "EXIT_CODE=1"
    goto :done
)

:done
if not defined EXIT_CODE set "EXIT_CODE=%ERRORLEVEL%"
echo.
if "%EXIT_CODE%"=="0" (
    echo Packaged launcher:
    echo %PACKAGED_EXE%
    echo.
    echo Shipping build log:
    echo %BUILD_LOG%
    echo.
    echo UAT log folder:
    echo %LOG_DIR%
    echo.
    echo Done.
) else (
    echo Packaging failed with exit code %EXIT_CODE%.
)
if defined PAUSE_AT_END pause
exit /b %EXIT_CODE%
