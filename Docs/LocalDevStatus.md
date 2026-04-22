# Local Development Status

Last updated: 2026-04-22
Status owner: `chat2`

## Current state

- workflow mode: local-only
- project state: `gameplay-blocked`
- project-state owner: `chat2`
- project-state reason: `chat12` has now reopened `EXD-UHTG1` as the active external dependency on `GP-C2`, and `chat13` has already accepted `GP-A-P5`. The planner is no longer holding fake intermediate states: gameplay is parked behind the named machine blocker, the art lane is accepted and closed rather than left acceptance-pending, `chat13` remains the continuous acceptance gate for future evidence, and `chat7` stays parked until `chat2` opens an explicit build or deterministic verification task.
- project-state next review time: `2026-04-22 17:05:00 +03:00`
- coordination hygiene owner: `chat11`
- lane-role summary:
  - `chat2`: planning authority, priority owner, milestone sequencer, and non-overlap gatekeeper
  - `chat11`: ops hygiene, stale-task control, and queue-status consistency
  - `chat12`: external dependency recovery tracking, follow-up, and source-of-truth maintenance
  - `chat13`: live milestone acceptance review, bounded QA verification, and done-means-done checks that start as soon as a credible evidence bundle exists
- persistent coordination heartbeat rule:
  - `chat11`, `chat12`, and `chat13` are persistent coordination lanes and must keep their infrastructure heartbeats alive continuously
  - temporary idleness means quiet polling, not heartbeat deletion
  - heartbeat deletion is allowed only if the lane itself is explicitly retired by planner or user decision
- parallelism rule:
  - credible evidence bundles from `chat3`, `chat5`, `chat7`, or `chat8` go to `chat13` immediately; do not wait for a separate planner cleanup round before acceptance starts
  - when a proof path is automatable, use an explicit `chat7` deterministic verification task as the default execution path instead of keeping repeatable proof work in the implementation lane
  - `chat7` should capture logs/artifacts and package the evidence bundle for `chat13` whenever it runs a bounded proof task
  - keep one low-risk parallel art or support package live whenever a bounded non-overlapping write scope exists
  - blocked or acceptance-pending milestones must record a primary next path and a fallback next path so lanes do not sit in dead time
- cross-lane authority rule:
  - `Docs/LocalSupportGP-S1Packet.md` and the grouped support package are authoritative on future shell/support reopen and activation boundaries
  - `Docs/LocalDevScaleUpContract.md` is authoritative on gameplay semantics only inside those accepted reopen/activation boundaries
  - if they disagree on reopen or activation boundaries, the grouped support packet wins and `chat3` must record the conflict instead of overriding it
- current roadmap stage: `Game Prototype`

## Parallel lane plan

### `chat3` gameplay/code

- state: parked-behind-external-dependency
- milestone package: `GP-C2`
- owner: `chat2`
- write scope:
  - gameplay/session flow and tool logic in gameplay-owned files under `Source/TheLastRite/Private/`
  - gameplay/session declarations in gameplay-owned files under `Source/TheLastRite/Public/`
- dependency:
  - accepted `GP-C1`
  - accepted `GP-S-P1`
  - accepted `SD-VS-1`
  - accepted `M10`
  - accepted grouped `VS-S2`
- verification:
  - gameplay-owned runtime markers and bounded runtime proof remain the preferred evidence style
  - if proof can be automated cleanly, `chat7` owns the repeatable deterministic verification run, artifact capture, and evidence packaging
  - `chat13` starts acceptance immediately on a credible evidence bundle instead of waiting for planner cleanup
- definition of done:
  - tool-scarcity role formation works cleanly around the apartment session loop without reopening shell or framework ownership
- start time: `2026-04-22 14:37:32 +03:00`
- expected next outcome type: `external-recovery-outcome-or-explicit-reassignment`
- stale-if-no-update-by: `none`
- primary next path:
  - `chat12` owns `EXD-UHTG1` and records the next real machine-level recovery outcome for the fresh UnrealGame UHT rename failure from `C:\dev\TheLastRite\Saved\UBT-TheLastRiteGame-1421019521.log`
- fallback next path:
  - if `EXD-UHTG1` does not progress or is declined as the recovery path, `chat2` opens one explicit `chat7` build or deterministic verification task or parks `GP-C2` behind one named non-external gate instead of leaving it pseudo-active
- planner note:
  - `GP-C1` is accepted and closed as the current gameplay proof gate
  - the latest `chat3` progress report at `2026-04-22 14:54:16 +03:00` says the bounded `GP-C2` gameplay slice is implemented but blocked before build verification by a fresh UnrealGame UHT temp-export rename failure
  - `chat12` has now reopened that blocker as active dependency `EXD-UHTG1`, so `GP-C2` is no longer pending intake; it is parked behind a named recovery owner
  - do not widen this lane into shell/support or framework ownership

### `chat5` art

- state: accepted-and-closed
- milestone package: `GP-A-P5`
- owner: `chat2`
- write scope:
  - `ArtSource/Apartment302/props/`
  - `ArtSource/Apartment302/references/`
  - `ArtSource/Apartment302/README.md`
- dependency:
  - accepted `GP-A-P4`
  - accepted Apartment 302 source-art baseline `A1-A9`
- verification:
  - `chat13` acceptance review before planner advancement where practical
  - source-art-only review; no runtime proof or Unreal integration claims
- definition of done:
  - one grouped Apartment 302 demon-variance source-art package exists for Hollow Saint, Weeping Matron, and The Appetite read-separation boards plus bounded carry-forward notes without touching Unreal content or claiming runtime implementation proof
- start time: `2026-04-22 15:49:00 +03:00`
- expected next outcome type: `none`
- stale-if-no-update-by: `none`
- primary next path:
  - no immediate art-lane follow-up is open; `GP-A-P5` is accepted and closed
- fallback next path:
  - if a later planner turn identifies one exact low-risk non-overlapping art package, open it explicitly rather than leaving `GP-A-P5` fake-active after acceptance
- planner note:
  - `GP-A-P4` is accepted and closed as the current post-case shell-art gate
  - `chat13` has now accepted `GP-A-P5`, so the handoff batch is no longer a live intake target
  - no next non-overlapping art package is explicitly opened yet; keep the lane closed instead of inventing follow-on churn

### `chat8` support/tooling

- state: parked-after-accepted-package
- latest accepted package: `GP-S-P1`
- owner: `chat8`
- write scope:
  - startup routing
  - title/lobby shell surfaces
  - briefing/tool-grab/case-launch shell flow
  - shell entry code
  - helper-facing launch support
- verification:
  - `chat13` accepted the deterministic shell traversal evidence for `GP-S-P1`
- definition of done:
  - a player can launch into a shell-first session flow and move cleanly through title, lobby, briefing, tool grab, and case launch without editor-only assumptions
- start time: `none`
- expected next outcome type: `none`
- stale-if-no-update-by: `none`
- primary next path:
  - open the next bounded support package only after `GP-C2` exposes stable result/debrief payload needs
- fallback next path:
  - reopen `GP-S-P1` only if `chat13` or an explicit `chat7` deterministic verification task finds a regression in the accepted shell traversal evidence
- planner note:
  - `GP-S-P1` is accepted and no longer sits at a fake acceptance boundary
  - keep `chat8` parked until a new non-overlapping support package is explicitly opened

### `chat9` framework

- state: parked
- owner: `chat9`
- parked reason:
  - no truly isolated framework write set exists yet
  - any early generalization would still guess abstractions before `GP-C2` exposes a real shared seam
- next safe activation point:
  - after `GP-C2` and any post-session support package expose a clean isolated shared bootstrap or result-flow surface

### `chat7` build and deterministic verification

- state: parked
- owner: `chat7`
- parked reason:
  - no explicit build, package, or deterministic verification task is currently opened
  - `chat7` is no longer limited to build break/fix work; it is also the default owner for repeatable scripted proof paths, artifact capture, and evidence packaging when `chat2` opens one
  - the fresh `GP-C2` blocker is currently being pointed to chat12-owned external intake first rather than opened immediately as a chat7 task
- next safe activation point:
  - `chat2` explicitly opens a build/package task or an automatable deterministic verification task for a live milestone

## Current-stage closed items

- `M4` package recovery: canceled for the current stage
- `M6` live replay: canceled for the current stage
- current confirmed runtime path:
  - `C:\dev\TheLastRite\Binaries\Win64\TheLastRite.exe`

## Parked lanes and why they cannot safely run yet

- `chat8`
  - `GP-S-P1` is already accepted, and no next isolated support package is opened yet
- `chat9`
  - framework work would still be fake parallelism because the gameplay and result/debrief seams are not isolated yet
- no additional gameplay package lane:
  - only one gameplay package should stay live unless `chat2` explicitly opens another non-overlapping package

## Next 2-3 parallel milestone groups

### Group 1: active now

- `chat13`
  - live acceptance intake on arriving evidence bundles from active lanes
- `chat7`
  - stays parked until `chat2` opens an explicit deterministic verification or build task

### Group 2: next support reopen

- `chat8`
  - `GP-S-P2` after `GP-C2` exposes a stable result/debrief payload seam

### Group 3: after a real seam is exposed

- `chat9`
  - `GP-F1`

## Active risks

- accepted `M10` must remain the gameplay-side semantic source of truth until a later planner change replaces it
- accepted grouped `VS-S2` must not be mistaken for permission to start shell/startup/config/helper implementation early outside explicitly opened support packages
- `VS-A10` must stay source-art-only; it should not imply that shell code is already open
- `GP-C2` must stay inside gameplay ownership and must not drift into support or framework files
- `GP-A-P5` must stay source-art-only and must not imply runtime implementation proof
- `GP-A-P5` must remain readability/reference support for demon variance and must not turn into hidden gameplay/system design by art lane expansion
- `GP-C2` must not sit in fake active implementation state after the recorded UnrealGame UHT rename blocker; its next legitimate state is recovery through `EXD-UHTG1` or explicit reassignment
- if a future proof path becomes repeatable and automatable, failing to hand it to `chat7` will recreate the old planner serialization

## Latest instruction issued to `chat3`

`GP-C2` is now blocked behind `EXD-UHTG1`.

- do not continue gameplay-lane retries until `chat12` records a real `EXD-UHTG1` recovery outcome or `chat2` explicitly reassigns the lane
- do not reopen shell/support or framework ownership
- when a credible future evidence bundle exists, route it to `chat13` immediately instead of waiting for planner cleanup
- if proof becomes fully scriptable after blocker clearance, expect `chat2` to hand the repeatable deterministic run to `chat7` rather than keeping retry work inside the gameplay lane

## Summary

- active lanes in parallel now:
  - `GP-A-P5` is accepted and closed; no live art production package is currently open
  - `chat13` is a continuous acceptance gate, not a delayed post-planner review
  - `chat7` is parked until an explicit build or deterministic verification task is opened
- what milestone groups were merged:
  - art `GP-A1` through `GP-A6` merged into completed package `GP-A-P1`
  - art `GP-A7` through `GP-A13` merged into accepted package `GP-A-P2`
  - support `GP-S1` and `GP-S2` merged into accepted package `GP-S-P1`
- new larger active milestone packages:
  - none currently open outside the parked `GP-C2` gameplay branch
- boundaries intentionally left unmerged:
  - `GP-C1` and `GP-C2` remain separate because the accepted ritual proof gate and the next gameplay/session-role package have different acceptance bars
  - `GP-S-P1` is accepted and parked because the next safe support package depends on a stable post-case payload seam from gameplay
  - framework milestones remain unmerged because no isolated shared seam exists yet
