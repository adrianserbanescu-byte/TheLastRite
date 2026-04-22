# Local Development Queue

Last updated: 2026-04-22
Active objective: keep gameplay sequencing honest while `GP-C2` is parked behind `EXD-UHTG1`, close accepted `GP-A-P5` cleanly instead of leaving it in fake intake state, and treat acceptance plus deterministic verification as concurrent support functions instead of serial planner chores
Queue owner: `chat2`

## Ownership model

- current stage: `Game Prototype`
- `chat3` owns case gameplay, clue flow, ritual flow, fail/win loop, and per-case logic
- `chat8` owns product shell, startup flow, menu/briefing/debrief loop, and tooling/support/runtime helpers
- `chat9` owns reusable case framework, generalized systems, data/config structure, and cross-case plumbing
- `chat5` owns source-art production only; Unreal or code integration is assigned separately
- `chat7` owns explicit build/package tasks and repeatable deterministic verification tasks
- `chat11` owns ops hygiene, stale-task control, and queue/status consistency
- `chat12` owns external dependency recovery tracking and source-of-truth maintenance
- `chat13` owns live milestone acceptance review and bounded QA verification before planner advancement where practical
- milestone batching rule:
  - batch adjacent work only when owner, dependency gate, write scope, and verification style stay the same and no planner decision is required between slices

## Current-stage closed items

- `M4` package recovery is canceled for the current stage
- `M6` live replay is canceled for the current stage
- accepted gameplay baseline milestones:
  - `M1`
  - `M2`
  - `M3`
  - `M5`
  - `M7`
  - `M8`
  - `M9`
  - `M10`
  - `SD-VS-1`

## Near-term gameplay/code milestones

| ID | Stage | Owner | State | Write scope | Dependency | Verification | Definition of done |
| --- | --- | --- | --- | --- | --- | --- | --- |
| GP-C1 | Game Prototype | `chat3` | accepted | gameplay and ritual systems in `Source/TheLastRite/Private/` and `Source/TheLastRite/Public/` | accepted `SD-VS-1`; accepted `M10`; accepted grouped `VS-S2`; accepted `ACC-GP-C1` review in `Docs/LocalAcceptanceStatus.md` and `Docs/LocalAcceptanceReview.md` | Accepted deterministic runtime gameplay evidence | The first 4-hand ritual prototype runs with visible ritual flow and bounded solo placeholder support. |
| GP-C2 | Game Prototype | `chat3` | parked behind `EXD-UHTG1` | gameplay/session flow and tool logic in gameplay-owned source files | accepted `GP-C1`; accepted `GP-S-P1`; active external dependency `EXD-UHTG1` in `Docs/LocalExternalDepStatus.md` | gameplay-owned runtime evidence after blocker clearance; `chat7` deterministic verification if the proof path is automatable; immediate `chat13` acceptance intake when a credible evidence bundle exists | Tool-scarcity role formation works cleanly around the apartment session loop without reopening shell or framework ownership. |

## Parallel-ready milestone packages

### Group 1: active now

- `chat3`
  - no active gameplay production package; `GP-C2` is parked behind active external dependency `EXD-UHTG1`
- `chat5`
  - no active production package; `GP-A-P5` is accepted and closed
- `chat13`
  - start acceptance immediately when any active lane emits a credible evidence bundle
- `chat7`
  - stays parked until `chat2` opens an explicit deterministic verification or build task

### Group 2: next support reopen

- `chat8`
  - `GP-S-P2` after `GP-C2` exposes a stable post-case payload seam

### Group 3: only after a real isolated seam exists

- `chat9`
  - `GP-F1`

## Package boundaries

- merged art package `GP-A-P1`:
  - folds former `GP-A1` through `GP-A6` into one completed shell-session foundation package
- merged art package `GP-A-P2`:
  - folds former `GP-A7` through `GP-A13` into one completed shell-resilience and service-state package
- merged support package `GP-S-P1`:
  - folds former `GP-S1` and `GP-S2` into one accepted shell-session-flow package
- intentionally unmerged gameplay boundary:
  - `GP-C1` and `GP-C2` stay separate because the accepted ritual proof gate and the next gameplay/session-role package have different acceptance bars

## Queue rules

- `chat2` is the only queue editor
- every active, blocked, or acceptance-pending package must declare:
  - write scope
  - dependency
  - verification
  - definition of done
- every blocked or acceptance-pending package must also declare:
  - primary next path
  - fallback next path
- route credible evidence bundles to `chat13` immediately; do not wait for a separate planner-cleanup turn before acceptance starts
- when a proof path is automatable, prefer an explicit `chat7` deterministic verification task over keeping repeatable proof work in the implementation lane
- keep one low-risk parallel art or support package live whenever a bounded non-overlapping write scope exists
- do not reopen package work unless `chat2` creates a new explicit package milestone
- do not activate shell or framework code early just because docs-only prep exists
- when `Docs/LocalDevScaleUpContract.md` and the grouped `Docs/LocalSupport*.md` packet both describe future boundaries:
  - the grouped support packet is authoritative on reopen and activation boundaries
  - the dev scale-up contract is authoritative on gameplay semantics only inside those boundaries
- if an active lane becomes blocked, either:
  - assign a recovery task immediately, or
  - park the lane behind a named dependency with a real owner
- future milestones exist for dependency planning only until they are explicitly activated
