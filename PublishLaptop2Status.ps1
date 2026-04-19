param(
    [Parameter(Mandatory = $true)]
    [string]$CurrentTask,

    [string]$Last30mProgress = 'no material change',

    [string]$Verification = 'not run',

    [string]$Blockers = 'none',

    [ValidateSet('yes', 'no')]
    [string]$NeedsCoordinator = 'no',

    [string]$CommitMessage = 'docs: publish laptop2 status update',

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

$updateScript = Join-Path $ProjectRoot 'UpdateLaptop2Status.ps1'
if (-not (Test-Path $updateScript)) {
    throw "Update script not found: $updateScript"
}

& $updateScript `
    -CurrentTask $CurrentTask `
    -Last30mProgress $Last30mProgress `
    -Verification $Verification `
    -Blockers $Blockers `
    -NeedsCoordinator $NeedsCoordinator `
    -ProjectRoot $ProjectRoot

Push-Location $ProjectRoot
try {
    $startingHead = (& $gitExe rev-parse HEAD).Trim()
    $branch = (& $gitExe rev-parse --abbrev-ref HEAD).Trim()

    & $gitExe add -- 'Docs/Laptop2Status.md'
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }

    & $gitExe diff --cached --quiet -- 'Docs/Laptop2Status.md'
    if ($LASTEXITCODE -eq 0) {
        Write-Host 'Laptop2 status file already matches HEAD.'
        exit 0
    }

    & $gitExe commit -m $CommitMessage
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }

    $publishedHead = (& $gitExe rev-parse HEAD).Trim()
    & $gitExe push origin $branch
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }

    Write-Host "Previous branch head: $startingHead"
    Write-Host "Published status commit: $publishedHead"
    Write-Host "Pushed branch: $branch"
    exit 0
}
finally {
    Pop-Location
}
