# Local External Dependencies

Last updated: 2026-04-22
Lane owner: `chat12`

## Purpose

- this file is the queue/source-of-truth index for blockers that require OS-level, machine-level, admin-level, manual, or otherwise external recovery
- normal implementation lanes may wait on these items, but they should not keep re-planning them as ordinary milestone work
- if no real recovery action is running, the item must read `waiting on external owner`, not pseudo-active
- `chat12` owns external dependency recovery tracking, exact blocked-item tracking, latest recovery outcome capture, and next recovery action tracking
- `chat2` remains the planner and milestone sequencer, but it does not carry external blockers as ordinary implementation churn

## Handling rules

- keep every external dependency here, in `Docs/LocalExternalDepStatus.md`, and in `Docs/LocalExternalDepLog.md`
- normal dev/build/support status docs may reference an external dependency id, but they must not keep re-dating or re-explaining it
- only move an item back into a normal implementation lane after the external dependency is:
  - `resolved`
  - `canceled`
  - explicitly replaced by a normal build or implementation milestone
- if a real recovery action is in flight, record it here as `active recovery step running`
- if no real recovery action is in flight, record it here as `waiting on external owner`

## Dependency register

| ID | Owner | Dependency type | Affected lanes | Exact blocked item(s) | Current state | Next review time | Source of truth |
| --- | --- | --- | --- | --- | --- | --- | --- |
| EXD-UHTG1 | `chat12` | `machine` | `chat3` | `C:\dev\TheLastRite\Intermediate\Build\Win64\UnrealGame\Inc\TheLastRite\UHT\SessionToolPickup.gen.cpp.tmp`; `C:\dev\TheLastRite\Intermediate\Build\Win64\UnrealGame\Inc\TheLastRite\UHT\SessionToolPickup.generated.h.tmp`; `C:\dev\TheLastRite\Intermediate\Build\Win64\UnrealGame\Inc\TheLastRite\UHT\TheLastRiteGameMode.gen.cpp.tmp`; `C:\dev\TheLastRite\Intermediate\Build\Win64\UnrealGame\Inc\TheLastRite\UHT\TheLastRiteGameMode.generated.h.tmp`; `C:\dev\TheLastRite\Intermediate\Build\Win64\UnrealGame\Inc\TheLastRite\UHT\TheLastRite.init.gen.cpp.tmp`; fresh UnrealGame UHT export rename gate during bounded `GP-C2` compile attempt from `C:\dev\TheLastRite\Saved\UBT-TheLastRiteGame-1421019521.log` | `waiting on external owner` | `2026-04-22 17:05:00 +03:00` | `Docs/LocalExternalDepStatus.md`; `Docs/LocalExternalDepLog.md` |
| EXD-POL1 | `chat12` | `manual` | `chat8` | `C:\dev\TheLastRite\Intermediate\Build\Win64\UnrealEditor\Inc\TheLastRite\UHT\TheLastRite.init.gen.cpp.tmp`; `C:\dev\TheLastRite\Intermediate\Build\Win64\UnrealEditor\Inc\TheLastRite\UHT\ShellGameMode.gen.cpp.tmp`; `C:\dev\TheLastRite\Intermediate\Build\Win64\UnrealEditor\Inc\TheLastRite\UHT\ShellGameMode.generated.h.tmp`; worker-side cleanup command rejected by environment policy before `BuildEditor.cmd` could start | `resolved` | `none` | `Docs/LocalExternalDepStatus.md`; `Docs/LocalExternalDepLog.md` |
| EXD-SFC1 | `chat12` | `machine` | `chat3`, `chat7`, `chat8` | `C:\dev\TheLastRite\Intermediate\Build\SourceFileCache.bin`; formerly blocked bounded build/runtime verification for `GP-C1` and `GP-S-P1` | `resolved` | `none` | `Docs/LocalExternalDepStatus.md`; `Docs/LocalExternalDepLog.md` |
