# Laptop 2 Local Prep

Local branch-only note for the content / packaging machine.

## Machine readiness

- Repo path: `C:\dev\TheLastRite`
- Local branch: `codex/content`
- Unreal: `C:\Program Files\Epic Games\UE_5.4`
- Visual Studio: `C:\Program Files\Microsoft Visual Studio\2022\Community`
- Preferred MSVC toolchain present: `14.38.33130`
- Windows SDK present: `10.0.18362.0`
- `BuildEditor.cmd` and `BuildGame.cmd` both succeeded locally on this machine
- `TheLastRite.sln` opens in Visual Studio
- `TheLastRite.uproject` opens in Unreal

## Packaging readiness audit

`PackageGame.cmd` is scoped correctly for laptop 2:

- output archive root: `C:\dev\TheLastRite\Packaged`
- expected packaged exe path: `C:\dev\TheLastRite\Packaged\Windows\TheLastRite.exe`
- temp root: `C:\dev\TheLastRite\LocalBuildTemp`
- shipping build log: `C:\dev\TheLastRite\LocalBuildTemp\UBT-<run>\UBT-TheLastRiteShipping.log`
- UAT log root: `C:\dev\TheLastRite\LocalBuildTemp\UATLogs-<run>`
- stage root: `C:\dev\TheLastRite\LocalBuildTemp\Stage-<run>`
- local DDC root: `C:\dev\TheLastRite\LocalBuildTemp\DerivedDataCache`

The script also:

- kills stale packaged game processes before packaging
- removes prior cooked / staged / cached outputs before packaging
- removes the previous `Packaged\Windows` output before a new archive
- builds `TheLastRite` in `Win64 Shipping` before `RunUAT BuildCookRun`
- uses `-ddc=InstalledNoZenLocalFallback`, which matches an installed Epic UE build better than a shared network cache assumption
- supports `--no-pause` for non-interactive local validation runs on laptop 2

## First local package pass

Local package pass on `codex/content` succeeded.

- archive output: `C:\dev\TheLastRite\Packaged\Windows`
- packaged launcher exe: `C:\dev\TheLastRite\Packaged\Windows\TheLastRite.exe`
- shipping build log: `C:\dev\TheLastRite\LocalBuildTemp\UBT-261058629\UBT-TheLastRiteShipping.log`
- cook / stage logs: `C:\dev\TheLastRite\LocalBuildTemp\UATLogs-261058629\`

Local smoke test also succeeded:

- the packaged launcher started
- the shipping executable handed off correctly to `TheLastRite-Win64-Shipping.exe`
- the packaged game reached a live window and was then closed cleanly
- `PlayPackagedGame.cmd` also resolves to the repo-local packaged build correctly on this branch and launched a live shipping window during validation
- `SmokeTestPackagedGame.cmd` now provides a repeatable repo-local smoke check for launcher and shipping handoff validation
- `SummarizePackagedBuild.cmd` now reports packaged artifact presence, package size, debug-symbol presence, and the latest log roots
- `ValidatePackageLane.cmd` now reruns packaging and the packaged smoke test in one local pass

This result is still local laptop 2 validation only and is not shared / integrated state.

## Likely first-package concerns

These are not blockers yet, but they should be kept in mind before the first real packaging pass:

1. Packaging will be disk-heavy even for a small project because `Intermediate`, `Binaries`, cook output, stage output, and archived output all coexist during the run.
2. `README.md` and `PlayPackagedGame.cmd` were corrected locally on `codex/content` so packaged-play instructions no longer depend on an old OneDrive path.
3. Unreal-generated `DefaultInput.ini` churn was discarded locally because the current build still uses legacy `BindAxis` / `BindAction` input wiring rather than an intentional Enhanced Input migration.

## Disk suitability

- Free space on `C:` during audit: about `91.49 GB`
- Current local repo-heavy folders already present:
  - `Intermediate`: about `4.06 GB`
  - `Binaries`: about `716 MB`
  - `Saved`: under `1 GB`

This still looks suitable for laptop 2's first package attempt, but the first package should be watched for peak temp usage under `LocalBuildTemp` and `Saved`.

## Content / package lane readiness

The current repo shape is small and fits the laptop 2 lane well:

- `Content/Maps/Apartment302.umap` is the only shipped map asset currently visible in content
- `Saved/Cooked/Windows/TheLastRite/Content/Maps/Apartment302.umap` and `.uexp` were produced during the local package pass
- `ArtSource/Apartment302/` already contains prop, material, reference, and UI source mockups
- `DesignData/` contains the browser-to-Unreal data source files called out in `Docs/UnrealMigration.md`
- top-level launch/build helpers are simple and local-machine readable

Laptop 2 can safely own:

- packaging validation
- packaged build smoke checks
- map readability and room-dressing follow-up
- content import prep from `ArtSource/Apartment302`
- output-path cleanup for local packaged play
- documentation for packaging and content handoff

## Follow-up packaging notes

- The current archived Windows package is about `0.434 GB`.
- The archived output currently includes debug files such as:
  - `TheLastRite-Win64-Shipping.pdb`
  - `tbb.pdb`
  - `tbbmalloc.pdb`
- That is acceptable for local validation on laptop 2, but it is a good candidate for later cleanup if the team wants a leaner shared build artifact after the first reviewed checkpoint.

Laptop 2 should not assume ownership yet for:

- gameplay-code integration from laptop 1
- merging any candidate handoff work
- changing `main`

## First safe task after shared checkpoint approval

The safest first shared-checkpoint task is a controlled packaging pass on a non-main branch, followed by a short packaged-build smoke test and cleanup of local packaged-launch path assumptions (`README.md` and `PlayPackagedGame.cmd`) so laptop 2 can validate builds without any OneDrive dependency.
