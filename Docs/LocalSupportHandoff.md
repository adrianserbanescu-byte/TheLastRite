# Local Support Handoff

Last updated: 2026-04-22
Handoff owner: `chat8`
Current support package artifact: grouped `VS-S2`

This file is the docs-only shell-activation handoff pack for the future `GP-S1` support lane.

It is maintained as one artifact inside the grouped docs-only `VS-S2` support-prep package.

It does not activate shell implementation early.

It defines the cleanest first support-owned write set once `SD-VS-1` allows `GP-S1` to start.

## Purpose

Use this handoff pack when `chat2` explicitly activates `GP-S1`.

The goal of the first shell activation pass is:

- stop booting directly into Apartment 302 as the public-facing session entry
- add a minimal title/lobby/start flow owned by the support lane
- keep gameplay-owned case logic under `chat3` untouched unless `chat2` explicitly broadens the write set

## Future `GP-S1` write set

These are the first support-owned surfaces that should be considered writable for `GP-S1` once `chat2` activates it:

- `Config/DefaultEngine.ini`
  - to move default boot away from direct Apartment 302 entry once a shell map or shell bootstrap path exists
- `README.md`
  - to align launch instructions with the shell-first runtime path after it is real
- `BuildEditor.cmd`
  - only if shell-facing launch notes need alignment after the shell entry path exists
- `BuildGame.cmd`
  - only if shell-facing launch notes need alignment after the shell entry path exists
- `OpenUnreal.cmd`
  - only if editor-start expectations need alignment after the shell entry path exists
- `PackageGame.cmd`
  - only if packaged launch behavior or notes must be updated to match the shell-first flow
- `PlayPackagedGame.cmd`
  - only if the packaged entry path changes and the helper must reflect it
- startup-routing code added for shell boot
  - preferred outcome: a new shell-owned startup surface rather than folding more startup responsibility into the current case-only GameMode

## Still-frozen surfaces

These surfaces should stay frozen unless `chat2` explicitly reassigns ownership or widens the write set:

- `Source/TheLastRite/Private/TheLastRiteGameMode.cpp`
- `Source/TheLastRite/Public/TheLastRiteGameMode.h`
- `Source/TheLastRite/Private/TheLastRiteHUD.cpp`
- `Source/TheLastRite/Public/TheLastRiteHUD.h`
- `Source/TheLastRite/Private/TheLastRiteCharacter.cpp`
- `Source/TheLastRite/Public/TheLastRiteCharacter.h`
- `Source/TheLastRite/Private/InspectableProp.cpp`
- `Source/TheLastRite/Public/InspectableProp.h`
- `Source/TheLastRite/Private/RitualAnchor.cpp`
- `Source/TheLastRite/Public/RitualAnchor.h`
- `Source/TheLastRite/Private/CaseExit.cpp`
- `Source/TheLastRite/Public/CaseExit.h`
- any gameplay-facing clue, ritual, fail/win, or case-complete logic owned by `chat3`
- any generalized framework, config abstraction, or cross-case plumbing owned by `chat9`

## First shell activation checklist

Run this checklist only after `chat2` explicitly activates `GP-S1`:

1. Confirm `SD-VS-1` has been accepted or `chat2` has explicitly opened an exception.
2. Re-read:
   - `Docs/LocalSupportQueue.md`
   - `Docs/LocalSupportStatus.md`
   - `Docs/LocalSupportInstructionsToChat8.md`
   - this file
3. Confirm the shell write set is still isolated from:
   - `chat3` case gameplay ownership
   - `chat9` framework/generalization ownership
4. Choose the smallest shell-first boot path:
   - a minimal title screen
   - a minimal lobby/start state
   - a controlled transition into the existing Apartment 302 session
5. Prefer a new shell-owned entry surface over expanding the current case GameMode with more session-shell responsibility.
6. Keep the first shell pass narrow:
   - boot path
   - title/lobby/start
   - map or mode routing
   - launch-helper alignment only after the runtime path is real
7. Verify the result against the support lane’s intended outcome:
   - the player no longer launches straight into a dev-style case start
   - the shell path is readable
   - helper docs do not drift from the real startup flow
8. Record the milestone outcome as one of:
   - completed
   - blocked
   - reissued by `chat2`
   - canceled/parked by `chat2`
   - waiting on explicit dependency

## Open boundary risks

- `TheLastRiteGameMode` currently owns both case gameplay flow and startup presentation, so shell activation can overlap case logic quickly if the first shell surface is not isolated.
- `Config/DefaultEngine.ini` is the cleanest shell boot switch, but changing it too early would conflict with the still-active slice runtime-truth lane.
- launch helpers and `README.md` should not be updated until the new shell-first runtime path is runnable and verified; otherwise docs drift will be created again.
- if shell work starts by modifying clue, ritual, fail/win, or case-complete behavior to make the shell fit, the lane has crossed into `chat3` ownership.
- if shell work starts by introducing generalized manifests, reusable framework abstractions, or multi-case config scaffolding, the lane has crossed into `chat9` ownership.

## Expected `GP-S1` outcome

The intended first shell milestone should leave the repo with:

- a minimal title/lobby/start entry owned by the support lane
- a controlled path into the existing Apartment 302 session
- no accidental takeover of case gameplay logic
- no accidental framework/generalization expansion
- helper and launch-note alignment only after the shell-first path is real
