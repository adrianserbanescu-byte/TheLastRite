param(
    [Parameter(Mandatory = $true)]
    [string]$CurrentTask,

    [string]$Last30mProgress = 'no material change',

    [string]$Verification = 'not run',

    [string]$Blockers = 'none',

    [ValidateSet('yes', 'no')]
    [string]$NeedsCoordinator = 'no',

    [string]$ProjectRoot = ''
)

$ErrorActionPreference = 'Stop'

if ([string]::IsNullOrWhiteSpace($ProjectRoot)) {
    $ProjectRoot = $PSScriptRoot
}

$gitExe = 'C:\Program Files\Git\cmd\git.exe'
if (-not (Test-Path $gitExe)) {
    $gitExe = 'git'
}

$statusFile = Join-Path $ProjectRoot 'Docs\Laptop2Status.md'
if (-not (Test-Path (Split-Path $statusFile -Parent))) {
    throw "Status file folder not found: $(Split-Path $statusFile -Parent)"
}

$head = (& $gitExe rev-parse HEAD).Trim()
$branch = (& $gitExe rev-parse --abbrev-ref HEAD).Trim()
$statusLines = @(& $gitExe status --porcelain)
$workingTree = if ($statusLines.Count -eq 0) { 'clean' } else { 'dirty' }
$time = Get-Date -Format 'yyyy-MM-dd HH:mm:ss zzz'

$content = @"
laptop2 update

time: $time
branch: $branch
head: $head
working tree: $workingTree
current task: $CurrentTask
last 30m progress: $Last30mProgress
verification: $Verification
blockers: $Blockers
needs coordinator: $NeedsCoordinator
"@

Set-Content -Path $statusFile -Value $content -Encoding ascii
Write-Host "Updated status file:"
Write-Host $statusFile
