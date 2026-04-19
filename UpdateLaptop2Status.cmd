@echo off
setlocal

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0UpdateLaptop2Status.ps1" %*
exit /b %ERRORLEVEL%
