param(
    [string]$ProjectRoot = $PSScriptRoot,
    [string]$RunId = ''
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

function Get-RunDirectories {
    param(
        [string]$BasePath,
        [string]$Prefix
    )

    Get-ChildItem -Path $BasePath -Directory -Filter "$Prefix-*" -ErrorAction SilentlyContinue |
        ForEach-Object {
            if ($_.Name -match "^$([regex]::Escape($Prefix))-(.+)$") {
                [pscustomobject]@{
                    Prefix = $Prefix
                    RunId = $Matches[1]
                    Directory = $_
                }
            }
        }
}

$runDirectoryGroups = @{}
foreach ($entry in (Get-RunDirectories -BasePath $localBuildTemp -Prefix 'UBT')) {
    if (-not $runDirectoryGroups.ContainsKey($entry.RunId)) {
        $runDirectoryGroups[$entry.RunId] = [ordered]@{}
    }
    $runDirectoryGroups[$entry.RunId].UBT = $entry.Directory
}

foreach ($entry in (Get-RunDirectories -BasePath $localBuildTemp -Prefix 'UATLogs')) {
    if (-not $runDirectoryGroups.ContainsKey($entry.RunId)) {
        $runDirectoryGroups[$entry.RunId] = [ordered]@{}
    }
    $runDirectoryGroups[$entry.RunId].UATLogs = $entry.Directory
}

foreach ($entry in (Get-RunDirectories -BasePath $localBuildTemp -Prefix 'UATSaved')) {
    if (-not $runDirectoryGroups.ContainsKey($entry.RunId)) {
        $runDirectoryGroups[$entry.RunId] = [ordered]@{}
    }
    $runDirectoryGroups[$entry.RunId].UATSaved = $entry.Directory
}

foreach ($entry in (Get-RunDirectories -BasePath $localBuildTemp -Prefix 'Stage')) {
    if (-not $runDirectoryGroups.ContainsKey($entry.RunId)) {
        $runDirectoryGroups[$entry.RunId] = [ordered]@{}
    }
    $runDirectoryGroups[$entry.RunId].Stage = $entry.Directory
}

$selectedRun = $null
if ($RunId) {
    if ($runDirectoryGroups.ContainsKey($RunId)) {
        $selectedRun = [pscustomobject]@{
            RunId = $RunId
            Paths = $runDirectoryGroups[$RunId]
            SortTime = @($runDirectoryGroups[$RunId].Values | ForEach-Object { $_.LastWriteTime } | Sort-Object -Descending | Select-Object -First 1)[0]
        }
    }
} else {
    $selectedRun = $runDirectoryGroups.GetEnumerator() |
        ForEach-Object {
            $sortTime = @($_.Value.Values | ForEach-Object { $_.LastWriteTime } | Sort-Object -Descending | Select-Object -First 1)[0]
            [pscustomobject]@{
                RunId = $_.Key
                Paths = $_.Value
                SortTime = $sortTime
            }
        } |
        Sort-Object SortTime -Descending |
        Select-Object -First 1
}

$allFiles = Get-ChildItem -Path $packageRoot -Recurse -File
$totalBytes = ($allFiles | Measure-Object -Property Length -Sum).Sum
if ($null -eq $totalBytes) { $totalBytes = 0 }
$totalGiB = [math]::Round($totalBytes / 1GB, 3)
$latestPackagedFile = $allFiles | Sort-Object LastWriteTime -Descending | Select-Object -First 1

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
if ($latestPackagedFile) {
    Write-Host ('latest packaged file time: {0}' -f $latestPackagedFile.LastWriteTime.ToString('yyyy-MM-dd HH:mm:ss zzz'))
    Write-Host ('latest packaged file path: {0}' -f $latestPackagedFile.FullName)
}

if ($selectedRun) {
    Write-Host ('run id: {0}' -f $selectedRun.RunId)
    if ($selectedRun.SortTime) {
        Write-Host ('run timestamp: {0}' -f $selectedRun.SortTime.ToString('yyyy-MM-dd HH:mm:ss zzz'))
    }
}

if ($selectedRun -and $selectedRun.Paths.Contains('UBT')) {
    $shippingBuildLog = Join-Path $selectedRun.Paths.UBT.FullName 'UBT-TheLastRiteShipping.log'
    Write-Host ('latest shipping build log: {0}' -f $shippingBuildLog)
}

if ($selectedRun -and $selectedRun.Paths.Contains('UATLogs')) {
    Write-Host ('latest uat log dir: {0}' -f $selectedRun.Paths.UATLogs.FullName)
}

if ($selectedRun -and $selectedRun.Paths.Contains('UATSaved')) {
    Write-Host ('latest uat saved dir: {0}' -f $selectedRun.Paths.UATSaved.FullName)
}

if ($selectedRun -and $selectedRun.Paths.Contains('Stage')) {
    Write-Host ('latest stage dir: {0}' -f $selectedRun.Paths.Stage.FullName)
}

exit 0
