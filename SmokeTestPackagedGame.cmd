@echo off
setlocal

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0SmokeTestPackagedGame.ps1"
set "EXIT_CODE=%ERRORLEVEL%"

echo.
if not "%EXIT_CODE%"=="0" (
    echo Smoke test failed.
    exit /b %EXIT_CODE%
)

echo Smoke test passed.
exit /b 0
