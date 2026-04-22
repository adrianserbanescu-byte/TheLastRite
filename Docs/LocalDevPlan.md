# Local Development Plan

Last updated: 2026-04-21
Runtime target: Unreal Engine 5.4 vertical slice for Apartment 302 / Hollow Saint
Workflow mode: local-only on this laptop

## Active objective

Use the next local milestone sequence to move the Unreal vertical slice to a real prototype-built state:

- `chat3` on code, runtime verification, and package-lane recovery
- `chat5` on asset export-prep and implementation-ready reference support

The product focus stays the same:

- first-minute onboarding clarity
- stronger clue-to-ritual reasoning
- clearer ritual outcome and exit feedback
- reliable packaged-build verification
- art support that can be carried into Unreal without re-deriving the visual rules
- one actual runnable prototype path, not just a successful compile

## Source of truth

Use these files as the only active coordination system:

- `Docs/LocalDevPlan.md` for the operating model and current work window
- `Docs/LocalDevQueue.md` for milestone order and definitions of done
- `Docs/LocalDevStatus.md` for the current state snapshot
- `Docs/LocalDevInstructionsToChat3.md` for the exact current task
- `Docs/LocalDevProgressReport.md` for implementation checkpoints
- `Docs/LocalDevBlockers.md` for real blockers only
- `Docs/LocalArtQueue.md` for art task order and definitions of done
- `Docs/LocalArtStatus.md` for the current art-lane snapshot and chat5 assignment
- `Docs/LocalArtHandoff.md` for completed art outputs and integration notes
- `Docs/LocalArtBlockers.md` for real art-production blockers only

No required step in this workflow depends on GitHub.

## Repo reality check

Current repo inspection confirms:

- the active Unreal slice is a single-map build centered on `Content/Maps/Apartment302.umap`
- the current readable-gameplay loop is driven directly from C++ in `Source/TheLastRite/Private/`
- first-minute guidance, progress copy, deduction copy, and room flow are primarily controlled by:
  - `TheLastRiteGameMode.cpp`
  - `TheLastRiteHUD.cpp`
  - `InspectableProp.cpp`
  - `TheLastRiteCharacter.cpp`
  - `RitualAnchor.cpp`
  - `CaseExit.cpp`
- the current packaged executable exists at `Packaged/Windows/TheLastRite.exe`, but it was last built on 2026-04-20 and still needs a fresh package-lane verification later in this work window
- a local `BuildGame.cmd` sanity check completed successfully on 2026-04-21 after a multi-minute compile/link pass, so build health is confirmed enough for `M1` while package-lane verification still remains later in the queue

Conclusion:

- `M1` remains the correct first move because the highest-payoff path is still first-minute readability in the existing C++ onboarding systems
- packaged-build reliability stays in the same 6-hour window, but after readability, clue logic, and ritual feedback passes

## Feature-basic product finish line

This planning horizon is broader than the current vertical-slice target, but it is still narrower than "the whole finished game."

Treat it as the roadmap for how the local workflow should continue after the current Hollow Saint slice becomes a real baseline and grows into a feature-basic version of the actual Early Access concept.

The target is not "Visual Studio build succeeds."

The target is not only "Apartment 302 reaches a real prototype-built state."

The target is "The Last Rite reaches a feature-basic build of its real one-apartment party-game concept."

That means all of these are true:

- there is one clearly documented current runtime path for the slice
- the player can launch the slice locally and complete the intended evidence -> ritual -> end-state loop
- the first minute, evidence interpretation, ritual choice, and end-state messaging have been checked in a live run, not only in compile output
- launch and verification docs match repo reality
- the accepted source-art pack is ready to support implementation without re-deriving the visual rules
- Apartment 302 can grow beyond one fixed Hollow Saint reading
- the 4-hand ritual exists in at least prototype form
- the wrong-anchor path can evolve into the possession cascade instead of a quiet dead end
- the product shell around the apartment is real enough to support session-based play

## Milestones Until Feature-basic Product Build

### `chat3` lane

Phase 1: prototype-built gate

Gate 1:

- `M4` package-lane recovery
- clear the stale Shipping intermediate folder, rerun `PackageGame.cmd`, and continue to `PlayPackagedGame.cmd` only if packaging succeeds

Gate 2:

- `M5` runtime-path confirmation
- establish the current real prototype runtime path
- packaged path is preferred
- if packaged remains blocked, explicitly confirm the fallback runnable path and document it clearly

Gate 3:

- `M6` live prototype sweep
- run the slice through onboarding, evidence interpretation, ritual choice, fail path, win path, and exit messaging
- record only the small concrete gaps that affect the prototype read

Gate 4:

- `M7` prototype polish pass
- fix one or two bounded readability or feedback issues discovered in the live sweep
- verify with `BuildGame.cmd` and rerun the affected runtime path

Gate 5:

- `M8` launch and docs alignment
- align `README.md`, `PackageGame.cmd`, `PlayPackagedGame.cmd`, and any fallback local notes with the real runtime or package flow confirmed so far

Gate 6:

- `M9` final verification close-out
- rerun the confirmed runtime path
- leave the slice with one explicit prototype-built statement or one precise blocking statement
- `M10` coordinator review follows after that work window

Phase 2: feature-basic product gate

Gate 7:

- `M11` session shell baseline
- build the minimal lobby, briefing, play, debrief, and replay flow around Apartment 302

Gate 8:

- `M12` ritual prototype pass
- replace the simple end-case choice with the first real prototype of the 4-hand ritual, even if some hands still use placeholder support

Gate 9:

- `M13` possession-cascade prototype
- implement the wrong-anchor inversion and the first playable form of the possession failure branch inside Apartment 302

Gate 10:

- `M14` three-demon apartment pass
- extend Apartment 302 beyond Hollow Saint into the first playable multi-demon variance model

Gate 11:

- `M15` co-op viability pass
- define and test the first acceptable authority/state model for synchronized clue flow, ritual flow, and fail/win state
- `M16` coordinator review follows after that gate

### `chat5` lane

Phase 1: prototype-built support

Gate A:

- `A5` export-prep pack
- turn the accepted source-art set into clean export-ready references for likely Unreal-facing use

Gate B:

- `A6` decal and material prep references
- create carry-forward guidance for nursery sigils, mirror bait, handprints, chalk marks, and false-lead wear

Gate C:

- `A7` UI export variants
- prepare dark/light or opacity-safe variants for accepted UI-support art without redesigning the HUD

Gate D:

- `A8` pack audit and contact sheet
- produce a quick-find contact sheet and naming audit for the accepted Apartment 302 art pack

Gate E:

- `A9` implementation-facing placement pack
- create one compact placement and scale guidance pack for decals, props, and UI-support carry-forward use

Gate F:

- `A10` targeted support reserve
- only produce a new batch if `chat2` issues a concrete follow-up tied to export, Unreal reinterpretation, or implementation support
- otherwise hold and keep the accepted pack stable

Phase 2: feature-basic product support

Gate G:

- `A11` shell and lobby kit
- extend the accepted UI language into reusable source art for lobby, tool grab, briefing, report, and replay states around Apartment 302

Gate H:

- `A12` ritual-hand visual kit
- build source-art support for Reader, Anchor, Censer, Guard, and ritual-phase readability

Gate I:

- `A13` three-demon variant pack
- create source-art support that lets one apartment read correctly for Hollow Saint, Weeping Matron, and The Appetite

Gate J:

- `A14` possession-state pack
- build the inversion, emergency-state, and suspicion-supporting visual language for the wrong-anchor branch

Gate K:

- `A15` product-pack audit
- produce the carry-forward contact sheet and audit needed for the broader one-apartment Early Access package

## Ownership boundaries

`chat2` owns planning and review.

- may edit `Docs/LocalDevPlan.md`
- may edit `Docs/LocalDevQueue.md`
- may edit `Docs/LocalDevStatus.md`
- may edit `Docs/LocalDevInstructionsToChat3.md`
- may edit `Docs/LocalArtQueue.md`
- may edit `Docs/LocalArtStatus.md`
- may inspect any repo file and any local build or package output
- should not take over broad product implementation work while acting as coordinator

`chat3` owns implementation and execution.

- may edit gameplay, UI, content, scripts, and package-facing files needed for the assigned milestone
- owns `Docs/LocalDevProgressReport.md`
- owns `Docs/LocalDevBlockers.md`
- should not rewrite the planning docs unless `chat2` explicitly reassigns that work

`chat5` owns art production and handoff.

- may edit source-art files under `ArtSource/`
- owns `Docs/LocalArtHandoff.md`
- owns `Docs/LocalArtBlockers.md`
- should not edit gameplay code, packaging scripts, queue state, or workflow design docs

Shared rule:

- both chats read all local workflow files before acting

Coordinator review loop:

- when checking repo state, `chat2` reviews both the dev lane and the art lane docs
- the art lane is local-only and parallel, but it does not self-assign integration work; any follow-up for Unreal or code integration is routed back through `chat2`

## Milestone standard

Every milestone in `Docs/LocalDevQueue.md` must include:

- objective
- likely files or systems involved
- verification
- definition of done

Milestones should stay small enough that `chat3` can finish one, verify it, and report it without reopening product scope.

## Local checkpoint rule

A checkpoint is complete when:

- the repo files reflect the change
- the required local verification step ran
- `Docs/LocalDevProgressReport.md` records the result
- `Docs/LocalDevStatus.md` and `Docs/LocalDevQueue.md` agree on what is next

Commits are optional local hygiene. They are not the coordination system.

## Blocker protocol

When `chat3` hits a real blocker:

1. write the blocker in `Docs/LocalDevBlockers.md`
2. include the exact blocked step, what was tried, the visible error or conflict, and the smallest decision needed
3. stop changing scope
4. wait for the next instruction in `Docs/LocalDevInstructionsToChat3.md`

`chat2` answers blockers by updating:

- `Docs/LocalDevInstructionsToChat3.md`
- `Docs/LocalDevStatus.md`

When `chat5` hits a real blocker:

1. write the blocker in `Docs/LocalArtBlockers.md`
2. include the exact blocked step, what was tried, the visible error or conflict, and the smallest decision needed
3. stop changing scope
4. wait for the next instruction in `Docs/LocalArtStatus.md`

`chat2` answers art blockers by updating:

- `Docs/LocalArtStatus.md`
