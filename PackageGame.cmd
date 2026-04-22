@echo off
setlocal

set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"
if not defined UE_ROOT set "UE_ROOT=C:\Program Files\Epic Games\UE_5.4"
set "BUILD_BAT=%UE_ROOT%\Engine\Build\BatchFiles\Build.bat"
set "RUN_UAT_BAT=%UE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat"

set "OUTPUT_DIR=%PROJECT_ROOT%\Packaged"
set "OUTPUT_WINDOWS_DIR=%OUTPUT_DIR%\Windows"
set "TEMP_ROOT=%PROJECT_ROOT%\LocalBuildTemp"
set "SAVED_ROOT=%PROJECT_ROOT%\Saved"
set "PACKAGE_LOG_DIR=%SAVED_ROOT%\Logs"
set "RUN_ID=%RANDOM%%RANDOM%"
set "LOG_DIR=%TEMP_ROOT%\UATLogs-%RUN_ID%"
set "ENGINE_SAVED_DIR=%TEMP_ROOT%\UATSaved-%RUN_ID%"
set "STAGE_DIR=%TEMP_ROOT%\Stage-%RUN_ID%"
set "LOCAL_DDC_DIR=%TEMP_ROOT%\DerivedDataCache"
set "ZEN_DATA_DIR=%TEMP_ROOT%\Zen\Data"
set "BUILD_LOG=%PACKAGE_LOG_DIR%\UBT-TheLastRiteShipping-%RUN_ID%.log"
set "UE-LocalDataCachePath=%LOCAL_DDC_DIR%"
set "UE-SharedDataCachePath=None"

if not exist "%BUILD_BAT%" (
    echo Unreal Build.bat not found:
    echo %BUILD_BAT%
    echo Set UE_ROOT to your Unreal Engine install path and try again.
    exit /b 1
)

if not exist "%RUN_UAT_BAT%" (
    echo Unreal RunUAT.bat not found:
    echo %RUN_UAT_BAT%
    echo Set UE_ROOT to your Unreal Engine install path and try again.
    exit /b 1
)

if not exist "%TEMP_ROOT%" mkdir "%TEMP_ROOT%"
if not exist "%SAVED_ROOT%" mkdir "%SAVED_ROOT%"
if not exist "%PACKAGE_LOG_DIR%" mkdir "%PACKAGE_LOG_DIR%"
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

call "%BUILD_BAT%" TheLastRite Win64 Shipping -Project="%PROJECT_ROOT%\TheLastRite.uproject" -WaitMutex -log="%BUILD_LOG%"
if errorlevel 1 goto :done

set "uebp_LogFolder=%LOG_DIR%"
set "uebp_EngineSavedFolder=%ENGINE_SAVED_DIR%"

call "%RUN_UAT_BAT%" BuildCookRun ^
 -project="%PROJECT_ROOT%\TheLastRite.uproject" ^
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
set "EXIT_CODE=%ERRORLEVEL%"
goto :finish

:done
set "EXIT_CODE=%ERRORLEVEL%"
:finish
echo.
if "%EXIT_CODE%"=="0" (
    echo Done.
) else (
    echo Packaging failed with exit code %EXIT_CODE%.
)
exit /b %EXIT_CODE%
