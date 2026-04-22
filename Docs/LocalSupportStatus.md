# Local Support Status

Last updated: 2026-04-22
Status owner: `chat2`

## Current state

- workflow mode: local-only
- current roadmap stage: `Game Prototype`
- active support worker: `none`
- active support task: `none`
- owner: `chat8`
- lane state:
  - accepted-awaiting-next-package
- latest accepted package:
  - `GP-S-P1`
- write scope:
  - startup routing
  - title/lobby shell surfaces
  - briefing/tool-grab/case-launch shell flow
  - shell entry code
  - helper-facing launch support
- dependency:
  - accepted `SD-VS-1`
  - accepted grouped `VS-S2`
  - accepted `M10`
  - accepted `ACC-GP-S-P1`
- cross-lane authority note:
  - the grouped support packet remains authoritative on future shell/support reopen and activation boundaries
  - `Docs/LocalDevScaleUpContract.md` may define gameplay semantics, but it must defer to the grouped support packet on reopen/activation boundary questions
- verification:
  - `chat13` accepted the deterministic shell traversal evidence for `GP-S-P1`
  - the accepted runtime proof in `Saved\Cooked\Windows\TheLastRite\Saved\Logs\TheLastRite.log` covers `Title -> Lobby -> Briefing -> ToolGrab -> CaseLaunch -> Apartment302`
- definition of done:
  - a player can launch into a shell-first session flow and move cleanly through title, lobby, briefing, tool grab, and case launch without editor-only assumptions

## Timing

- start time: `none`
- expected next outcome type: `none`
- stale-if-no-update-by: `none`

## Recent batching change

- former `GP-S1` and `GP-S2` now live under one larger accepted package `GP-S-P1`
- acceptance runs concurrently on arriving evidence bundles and does not wait for planner cleanup
- repeatable deterministic proof paths should go to `chat7` when explicitly opened instead of staying inside the support lane
- `chat7` should also capture the logs/artifacts and package the resulting evidence bundle for `chat13`

## Why this lane is safe in parallel now

- `GP-S-P1` is accepted and closed as the current shell-session proof gate
- no next isolated support package is open yet, so holding `chat8` parked avoids overlap with the newly active gameplay package
- the next safe support reopen depends on a stable post-case payload seam rather than more shell traversal rework

## Current parked reason

- `GP-S-P1` is accepted
- no next non-overlapping support package is currently active
- the primary next support path is `GP-S-P2` after `GP-C2` exposes stable result/debrief payload needs
- the fallback next path is to reopen `GP-S-P1` only if `chat13` or an explicit `chat7` verification task finds a regression in accepted shell traversal evidence

## Current risks

- the support lane must stay inside shell/startup/helper surfaces and must not drift into gameplay logic
- the accepted grouped packet must remain authoritative if gameplay semantics appear to conflict with reopen or activation boundaries
- do not wake `chat7` implicitly; scripted proof belongs there only when `chat2` opens an explicit deterministic verification task

## Latest instruction issued to `chat8`

`GP-S-P1` is accepted and the support lane is parked.

- do not keep retrying shell verification from this lane
- stay parked until `chat2` opens the next explicit non-overlapping support package
- the primary next support path is `GP-S-P2` after gameplay exposes stable result/debrief payload needs
- the fallback path is a targeted reopen only if accepted shell traversal evidence regresses
