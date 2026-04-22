# Local Instructions To Chat3

Last updated: 2026-04-22
Instruction owner: `chat2`
Current instruction seed: `GP-C2` has returned a fresh compile-side blocker, and `chat12` now owns that blocker as active dependency `EXD-UHTG1`

## Current assignment

Active gameplay milestone: `GP-C2`.

- start time:
  - `2026-04-22 14:37:32 +03:00`
- expected next outcome type:
  - `external-recovery-outcome-or-explicit-reassignment`
- stale-if-no-update-by:
  - `none`

## Milestone execution rule

For every milestone, use this loop-break structure:

- exact success condition
- exact blocker condition
- max retries
- max exploration time

Autonomous progression rule for `chat3`:

- when the current gameplay milestone is completed cleanly, continue directly to the next eligible gameplay milestone in `Docs/LocalDevQueue.md` without waiting for a fresh prompt from `chat2`
- only do that if:
  - dependencies are satisfied
  - there is no blocker on the current milestone
  - there is no ownership conflict with `chat8` or `chat9`
  - there is no explicit hold in `Docs/LocalDevStatus.md` or this file
- if the current milestone is blocked, do not skip past it unless `chat2` explicitly reassigns or reorders the lane
- if a future active gameplay task is marked active but no new progress report, blocker, or status transition appears by its stale deadline, treat that as stale-task planner action for `chat2`, not permission to keep improvising or wait indefinitely

## Exact task

1. Read `README.md`, `Docs/LocalDevQueue.md`, `Docs/LocalDevStatus.md`, `Docs/LocalDevScaleUpContract.md`, and `Docs/LocalSupportGP-S1Packet.md`.
2. Treat accepted `GP-C1`, accepted `GP-S-P1`, accepted `M10`, and accepted `SD-VS-1` as the fixed baseline.
3. Do not continue `GP-C2` implementation churn while the fresh UnrealGame UHT temp-export rename failure is parked behind active dependency `EXD-UHTG1`.
4. Do not edit shell/startup/menu/lobby/helper/config surfaces owned by `chat8`.
5. Do not generalize into framework/data/config plumbing owned by `chat9`.
6. Keep `Docs/LocalDevScaleUpContract.md` authoritative on gameplay semantics, but defer to `Docs/LocalSupportGP-S1Packet.md` on reopen and activation boundaries.
7. Do not attempt ad hoc cleanup, build retry, or verification reruns unless `chat2` explicitly reopens the lane or hands the proof path to `chat7`.
8. Wait for one of two explicit outcomes only:
   - `chat12` records a real `EXD-UHTG1` recovery outcome, or
   - `chat2` publishes a new bounded reassignment for `GP-C2`

## Out of scope for this milestone

- queue or roadmap rewrites
- workflow redesign
- GitHub, branch, PR, or push requirements
- product-shell, startup, menu, briefing, debrief, tooling, or helper-lane work assigned to `chat8`
- reusable framework, generalized systems, data/config, or cross-case plumbing work reserved for `chat9`
- broad Intermediate cleanup or multi-method verification churn
- autonomous retries while `GP-C2` is blocked pending dedicated-lane intake

## Ownership reminder

- `chat2` owns planning, queue state, and milestone reassignment
- `chat3` owns implementation, local debugging, progress reports, and blocker reports for the active gameplay lane when that lane is explicitly open
- the accepted `M10` contract remains the gameplay-side semantic source of truth
- do not widen `GP-C2` into shell/framework work, package recovery, or off-lane cleanup experiments
