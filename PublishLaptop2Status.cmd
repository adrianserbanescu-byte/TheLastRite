@echo off
setlocal

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0PublishLaptop2Status.ps1" %*
exit /b %ERRORLEVEL%
