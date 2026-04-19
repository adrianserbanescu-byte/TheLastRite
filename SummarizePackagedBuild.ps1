param(
    [string]$ProjectRoot = $PSScriptRoot
)

$ErrorActionPreference = 'Stop'

$packageRoot = Join-Path $ProjectRoot 'Packaged\Windows'
$launcherPath = Join-Path $packageRoot 'TheLastRite.exe'
$shippingPath = Join-Path $packageRoot 'TheLastRite\Binaries\Win64\TheLastRite-Win64-Shipping.exe'
$shippingPdbPath = Join-Path $packageRoot 'TheLastRite\Binaries\Win64\TheLastRite-Win64-Shipping.pdb'
$localBuildTemp = Join-Path $ProjectRoot 'LocalBuildTemp'

if (-not (Test-Path $packageRoot)) {
    Write-Host 'Packaged output folder not found:'
    Write-Host $packageRoot
    exit 1
}

$latestUbtDir = Get-ChildItem -Path $localBuildTemp -Directory -Filter 'UBT-*' -ErrorAction SilentlyContinue |
    Sort-Object LastWriteTime -Descending |
    Select-Object -First 1
$latestUatLogDir = Get-ChildItem -Path $localBuildTemp -Directory -Filter 'UATLogs-*' -ErrorAction SilentlyContinue |
    Sort-Object LastWriteTime -Descending |
    Select-Object -First 1

$allFiles = Get-ChildItem -Path $packageRoot -Recurse -File
$totalBytes = ($allFiles | Measure-Object -Property Length -Sum).Sum
if ($null -eq $totalBytes) { $totalBytes = 0 }
$totalGiB = [math]::Round($totalBytes / 1GB, 3)

Write-Host 'Packaged build summary'
Write-Host
Write-Host ('package root: {0}' -f $packageRoot)
Write-Host ('launcher exists: {0}' -f (Test-Path $launcherPath).ToString().ToLower())
Write-Host ('launcher path: {0}' -f $launcherPath)
Write-Host ('shipping exe exists: {0}' -f (Test-Path $shippingPath).ToString().ToLower())
Write-Host ('shipping exe path: {0}' -f $shippingPath)
Write-Host ('shipping pdb exists: {0}' -f (Test-Path $shippingPdbPath).ToString().ToLower())
Write-Host ('shipping pdb path: {0}' -f $shippingPdbPath)
Write-Host ('package size gib: {0}' -f $totalGiB)

if ($latestUbtDir) {
    $shippingBuildLog = Join-Path $latestUbtDir.FullName 'UBT-TheLastRiteShipping.log'
    Write-Host ('latest shipping build log: {0}' -f $shippingBuildLog)
}

if ($latestUatLogDir) {
    Write-Host ('latest uat log dir: {0}' -f $latestUatLogDir.FullName)
}

exit 0
