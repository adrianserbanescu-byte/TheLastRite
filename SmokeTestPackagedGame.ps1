param(
    [int]$StartupTimeoutSeconds = 20,
    [int]$LiveWindowSeconds = 5
)

$ErrorActionPreference = 'Stop'

$projectRoot = $PSScriptRoot
$launcherPath = Join-Path $projectRoot 'Packaged\Windows\TheLastRite.exe'
$launcherName = 'TheLastRite'
$shippingName = 'TheLastRite-Win64-Shipping'

Write-Host "Smoke testing packaged build from:"
Write-Host $projectRoot
Write-Host

if (-not (Test-Path $launcherPath)) {
    Write-Host 'Packaged build not found:'
    Write-Host $launcherPath
    exit 1
}

Write-Host 'Closing stale packaged game processes...'
Get-Process -Name $launcherName, $shippingName -ErrorAction SilentlyContinue | Stop-Process -Force

Write-Host 'Launching packaged build...'
$launcherProcess = Start-Process -FilePath $launcherPath -PassThru
$deadline = (Get-Date).AddSeconds($StartupTimeoutSeconds)
$shippingProcess = $null

while ((Get-Date) -lt $deadline) {
    $shippingProcess = Get-Process -Name $shippingName -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($shippingProcess -and $shippingProcess.MainWindowHandle -ne 0) {
        break
    }

    Start-Sleep -Milliseconds 500
}

if (-not $shippingProcess) {
    Write-Host "Shipping process did not appear within $StartupTimeoutSeconds seconds."
    exit 1
}

if ($shippingProcess.MainWindowHandle -eq 0) {
    Write-Host "Shipping process started but no visible window appeared within $StartupTimeoutSeconds seconds."
    exit 1
}

Write-Host "Shipping process is live (PID $($shippingProcess.Id))."
Write-Host "Keeping it open for $LiveWindowSeconds seconds before closing it."
Start-Sleep -Seconds $LiveWindowSeconds

$shippingProcess.Refresh()
if (-not $shippingProcess.HasExited) {
    Stop-Process -Id $shippingProcess.Id -Force
}

if (-not $launcherProcess.HasExited) {
    Stop-Process -Id $launcherProcess.Id -Force
}

Write-Host 'Packaged build smoke test completed.'
exit 0
