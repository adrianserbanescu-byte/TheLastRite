# Local Ops Queue

Last updated: 2026-04-22
Queue owner: `chat11`
Planning authority: `chat2`

## Lane split

- `chat2` owns milestone priority, roadmap translation, sequencing, and scope decisions
- `chat11` owns coordination hygiene, review control, blocker/staleness follow-up, doc-state consistency, and limited coordination-state maintenance
- `chat12` owns the external-dependency lane
- `chat13` owns acceptance and QA review
- `chat11` must not directly reprioritize worker milestones without planner approval
- `chat2` should use `chat11` findings as the default ops-control input and should not rerun routine ops audits unless a planner-level decision requires it

## Ops tasks

| ID | State | Objective | Dependency | Owner | Verification gate |
| --- | --- | --- | --- | --- | --- |
| O1 | active | Audit current lane consistency, confirm no active task lacks an outcome, confirm managed blockers have owners and next review times, and report any inconsistency locally. | current local queue/status/instruction docs | `chat11` | `Docs/LocalOpsReview.md` records the audit result and either confirms consistency or names the exact inconsistency/blocker. |
| O2 | future | Run stale-task sweep and force normalization of any active task that survives past its stale window without a recorded outcome. | any future active lane task crossing its stale window | `chat11` | Any stale task is converted locally into completed, blocked, reissued, canceled, or parked-waiting-on-dependency. |
| O3 | future | Run managed-blocker review sweep, classify each blocker as `ops-touch only`, `planner-review needed`, or `strategy change needed`, confirm whether a real dependency is still actively running, and confirm each blocker is touched before its next review time. | any managed blocker reaching review time | `chat11` | Every managed blocker is either maintained locally under the allowed ops-touch rule while a real dependency is still running, or explicitly escalated to `chat2` with the correct blocker class when no active dependency justifies passive delay. |
| O4 | future | Audit accepted-state consistency drift only when an already-accepted milestone still leaves stale blocker/task language behind in queue, status, instruction, or archive docs; do not take over acceptance or QA review. | accepted milestone transitions that create doc-state drift | `chat11` | Accepted state is reflected consistently across the owning queue/status/instruction/review docs without `chat11` taking ownership of acceptance or QA review. |
| O5 | future | Detect the fully parked condition and escalate it if no implementation lane is active and no explicit freeze or named gate state is recorded within 5 minutes. | all implementation lanes parked or idle with only ops work remaining | `chat11` | The project is explicitly marked `implementation active`, `intentionally frozen`, or `fully parked pending gate` with owner, resume trigger, and next review time. |

## Queue rules

- `chat11` is the only editor of `Docs/LocalOps*.md`
- `chat2` remains the only planner and priority owner
- `chat11` may normalize coordination state and flag issues, but must not change roadmap direction or milestone order without planner approval
- `chat11` must not take over external-dependency ownership from `chat12` or acceptance/QA review ownership from `chat13`
- `chat11` may maintain coordination state when the real strategy is unchanged, including one unchanged re-date for `ops-touch only` blockers
- if a blocker or freeze is being passively delayed, `chat11` must verify that a real dependency is still actively running
- if no active dependency is still running, `chat11` must stop maintenance-only delay and escalate the item back to `chat2`
- `chat11` must escalate back to `chat2` when a blocker changes class to `planner-review needed` or `strategy change needed`
- `chat11` findings are the default ops-review input for `chat2` heartbeats and coordination passes
- `chat2` should only duplicate those reviews when milestone assignment, dependency choice, roadmap translation, or scope control requires direct planner inspection
- if there is no `active` ops task, `chat11` waits
- blocked ops tasks do not auto-advance; `chat11` records the blocker and waits for planner approval if the next step changes worker priority or scope
