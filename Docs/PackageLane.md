# Package Lane Runbook

This is the laptop 2 packaging runbook for `codex/content`.

Use it when validating the non-main package lane, rerunning a packaged smoke test, or locating the latest package logs without guessing.

If the result needs a GitHub-visible package note, use `Docs/PackageValidationNoteTemplate.md`.

## Scope

This runbook is for:

- packaging validation on `codex/content`
- packaged launcher smoke tests
- package summary and log lookup
- repo-local launcher behavior

This runbook is not for:

- gameplay C++ changes
- `main`
- `codex/gameplay`
- cross-lane integration

## Required machine state

- Unreal Engine 5.4 installed at `C:\Program Files\Epic Games\UE_5.4`
- repo at `C:\dev\TheLastRite`
- branch `codex/content`
- packaged launcher expected at `C:\dev\TheLastRite\Packaged\Windows\TheLastRite.exe`

## Main scripts

- `PackageGame.cmd`
  - builds `TheLastRite` in `Win64 Shipping`
  - runs Unreal `BuildCookRun`
  - archives to `Packaged\Windows`
  - fails if the packaged launcher is missing

- `SmokeTestPackagedGame.cmd`
  - launches the packaged build
  - waits for `TheLastRite-Win64-Shipping.exe`
  - requires a visible game window
  - closes the packaged process after a short live-window check
  - passes optional PowerShell timeout arguments through to `SmokeTestPackagedGame.ps1`

- `SummarizePackagedBuild.cmd`
  - confirms launcher and shipping executable presence
  - reports launcher and shipping executable timestamps
  - reports whether the shipping PDB exists
  - reports package size
  - reports the latest packaged-file timestamp
  - supports `-Concise` for a one-line package verdict
  - prints the latest correlated UBT, UAT, saved, and stage paths

- `ValidatePackageLane.cmd`
  - runs `PackageGame.cmd`
  - runs `SmokeTestPackagedGame.cmd`
  - runs `SummarizePackagedBuild.cmd`
  - forwards optional smoke-test timeout arguments to the smoke step
  - finishes with a one-line package verdict from `SummarizePackagedBuild.cmd -Concise`
  - use this when you want one package-lane verdict instead of three separate steps

- `PublishLaptop2Status.cmd`
  - updates `Docs\Laptop2Status.md`
  - commits the status update on `codex/content`
  - pushes the branch so the GitHub-visible report stays current
  - prints the previous branch head and the pushed status-commit SHA

- `PlayPackagedGame.cmd`
  - launches the repo-local packaged build from `Packaged\Windows`
  - prints a clear missing-build message if the launcher does not exist
  - prints the launcher path and the started process id
  - supports `--check-only` for a fast launcher-path validation without starting the game

## Normal workflows

### 1. Full package-lane validation

Use this after material package/content changes:

```text
ValidatePackageLane.cmd
```

Longer-timeout variant:

```text
ValidatePackageLane.cmd -StartupTimeoutSeconds 30 -LiveWindowSeconds 8
```

Non-interactive variant:

```text
PackageGame.cmd --no-pause
SmokeTestPackagedGame.cmd
SummarizePackagedBuild.cmd
```

### 2. Smoke-check an already packaged build

Use this when packaging is already done and you only need launcher/shipping handoff confidence:

```text
SmokeTestPackagedGame.cmd
```

Longer-timeout variant for slower machines:

```text
SmokeTestPackagedGame.cmd -StartupTimeoutSeconds 30 -LiveWindowSeconds 8
```

### 3. Human launch of the current packaged build

```text
PlayPackagedGame.cmd
```

Fast launcher-path check:

```text
PlayPackagedGame.cmd --check-only
```

### 4. Quick package summary

```text
SummarizePackagedBuild.cmd
```

One-line summary variant:

```text
SummarizePackagedBuild.cmd -Concise
```

### 5. Publish the 30-minute laptop 2 report

Use this when the branch needs its routine GitHub-visible status update:

```text
PublishLaptop2Status.cmd -CurrentTask continue-Apartment302-room-dressing-import-prep -Last30mProgress "added a new import manifest for stable bundle names and screenshot labels" -Verification not-run-docs-only-content-checkpoint
```

## Output and log paths

Package output:

- `C:\dev\TheLastRite\Packaged\Windows\TheLastRite.exe`

Temp root:

- `C:\dev\TheLastRite\LocalBuildTemp`

Per-run folders:

- shipping build log root: `C:\dev\TheLastRite\LocalBuildTemp\UBT-<run>`
- UAT log root: `C:\dev\TheLastRite\LocalBuildTemp\UATLogs-<run>`
- UAT saved root: `C:\dev\TheLastRite\LocalBuildTemp\UATSaved-<run>`
- stage root: `C:\dev\TheLastRite\LocalBuildTemp\Stage-<run>`
- local derived data cache: `C:\dev\TheLastRite\LocalBuildTemp\DerivedDataCache`

The shipping build log inside a run looks like:

- `C:\dev\TheLastRite\LocalBuildTemp\UBT-<run>\UBT-TheLastRiteShipping.log`

`SummarizePackagedBuild.cmd` will print the latest correlated paths automatically.

## What `PackageGame.cmd` cleans

Before packaging, the script clears stale local outputs that commonly confuse reruns:

- stale packaged game processes
- `Saved\Cooked\Windows`
- `Saved\Cooked\_Del`
- `Saved\StagedBuilds\Windows`
- `Intermediate\Staging`
- `Intermediate\CachedAssetRegistry_0.bin`
- previous `Packaged\Windows`

That cleanup is local to the repo and supports repeatable non-main packaging.

## Expected success signals

`PackageGame.cmd` success:

- `Packaged\Windows\TheLastRite.exe` exists
- shipping build log path is printed
- UAT log folder path is printed

`SmokeTestPackagedGame.cmd` success:

- shipping process appears
- shipping process creates a visible window
- packaged game stays live briefly
- the script closes the launcher and shipping process cleanly

`SummarizePackagedBuild.cmd` success:

- launcher path is present
- shipping executable path is present
- packaged-file freshness is visible
- latest run id and temp/log roots are printed when available

## First checks when something fails

If `PackageGame.cmd` fails:

1. copy the exact command used
2. note the printed shipping build log path
3. note the printed UAT log path if present
4. inspect whether the failure is clearly local tooling, disk, or Unreal packaging
5. if the fix points into gameplay-code ownership, stop and report instead of improvising

If `SmokeTestPackagedGame.cmd` fails:

1. confirm `Packaged\Windows\TheLastRite.exe` exists
2. rerun `SummarizePackagedBuild.cmd`
3. check whether the launcher starts but the shipping process never appears
4. check whether the shipping process appears without a visible window

If `PlayPackagedGame.cmd` fails:

1. verify the packaged launcher path exists
2. rerun `ValidatePackageLane.cmd` if the package is stale or uncertain

## Reporting rule on this branch

- every 30 minutes: status update only in `Docs\Laptop2Status.md`
- every 2-3 hours: meaningful pushed checkpoint if coherent work is ready
- blocker: push the blocker status immediately, then check `Docs\Laptop2CoordinatorReply.md`

Do not package on `main`.
