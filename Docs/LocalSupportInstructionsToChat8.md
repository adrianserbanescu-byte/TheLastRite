# Local Instructions To Chat8

Last updated: 2026-04-22
Instruction owner: `chat2`
Current instruction seed: `GP-S-P1` is accepted, so the support lane is parked until the next explicit non-overlapping package opens

## Current assignment

Active support milestone: `none`.

## Loop-break rule

- success condition:
  - define the exact support output or helper/task result that must exist
- blocker condition:
  - define the exact point where support validation or execution cannot continue reliably from the current environment or write set
- max retries:
  - default 2 total methods unless `chat2` explicitly changes it
  - one main attempt
  - one narrow retry
- max exploration time:
  - default 20 minutes for support/tooling tasks

## Exact task

- `GP-S-P1` is accepted and closed as the current support proof gate
- stay parked until `chat2` opens the next explicit non-overlapping support package
- the primary next support path is `GP-S-P2` after gameplay exposes stable case-result and debrief payload needs
- the fallback path is to reopen `GP-S-P1` only if `chat13` or an explicit `chat7` deterministic verification task finds a regression in accepted shell traversal evidence
- do not keep retrying runtime verification, cleanup, or build commands from this lane while no active support package is open

## Out of scope for this milestone

- gameplay logic or case-owned files
- framework/generalization work owned by `chat9`
- package recovery or fresh packaged-build work
- queue rewrites or roadmap rewrites

## Ownership reminder

- `chat2` owns planning, queue state, and milestone reassignment
- `chat8` owns future product shell, startup flow, menu/briefing/debrief loop, and tooling/support/runtime helpers
- no active support package is open right now
