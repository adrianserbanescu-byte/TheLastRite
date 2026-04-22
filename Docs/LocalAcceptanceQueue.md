# Local Acceptance Queue

Last updated: 2026-04-22
Queue owner: `chat13`
Planner authority: `chat2`

## Purpose

- `chat13` owns milestone acceptance review, bounded QA verification, and done-means-done checks before planner advancement where practical
- `chat13` does not own roadmap sequencing, stale-task policing, or implementation work
- `chat2` advances milestones only after acceptance notes are recorded here and in `Docs/LocalAcceptanceReview.md` when the lane is using the acceptance flow
- acceptance is concurrent work: credible evidence bundles should be queued here immediately instead of waiting for a separate planner round

## Near-term acceptance items

| ID | Target milestone | Owner | State | Dependency | Verification style | Definition of done |
| --- | --- | --- | --- | --- | --- | --- |
| ACC-AUDIT-GP-A-P3 | `GP-A-P3` | `chat13` | completed | active art package definition available | Definition-of-done and acceptance-flow audit | The active art package has an explicit acceptance checklist that can be applied without reopening product scope. |
| ACC-GP-A-P3 | `GP-A-P3` | `chat13` | completed | `chat5` reported package completion in `Docs/LocalArtHandoff.md` at `2026-04-22 05:56:44 +03:00` | Bounded source-art acceptance review | The grouped shell implementation-handoff pack matches its definition of done, stays inside source-art roots, and does not claim runtime proof. |
| ACC-GP-C1 | `GP-C1` | `chat13` | completed | deterministic runtime evidence handed off from planner-owned verification in `Saved\Cooked\Windows\TheLastRite\Saved\Logs\TheLastRite.log` at `2026-04-22 14:27:54 +03:00` | Runtime gameplay acceptance review | The first 4-hand ritual prototype meets the `GP-C1` definition of done without crossing shell/framework ownership. |
| ACC-GP-S-P1 | `GP-S-P1` | `chat13` | completed | deterministic runtime evidence handed off from planner-owned verification in `Saved\Cooked\Windows\TheLastRite\Saved\Logs\TheLastRite.log` at `2026-04-22 14:27:53 +03:00` | Runtime shell/session acceptance review | The grouped shell-session-flow package meets its definition of done without drifting into gameplay ownership. |
| ACC-GP-C2 | `GP-C2` | `chat13` | future | credible gameplay evidence bundle from `chat3` and, when opened, explicit deterministic verification evidence from `chat7` | Runtime gameplay acceptance review | Tool-scarcity role formation works cleanly around the apartment session loop without reopening shell/framework ownership. |
| ACC-GP-A-P4 | `GP-A-P4` | `chat13` | completed | completed art handoff batch recorded in `Docs/LocalArtHandoff.md` | Bounded source-art acceptance review | The grouped result/debrief/replay/requeue source-art pack stays inside art roots, supports future shell work, and does not claim runtime proof. |
| ACC-GP-A-P5 | `GP-A-P5` | `chat13` | completed | completed art handoff batch recorded in `Docs/LocalArtHandoff.md` | Bounded source-art acceptance review | The grouped Apartment 302 demon-variance source-art pack stays inside art roots, preserves bounded carry-forward readability guidance, and does not claim runtime proof. |

## Queue rules

- keep acceptance items bounded to one milestone package at a time
- acceptance failures belong in `Docs/LocalAcceptanceBlockers.md`
- external machine/admin/manual blockers stay with `chat12`
- a claimed completion is not planner-ready until `chat13` records one of:
  - accepted
  - accepted with bounded follow-up note
  - rejected with exact acceptance failure
