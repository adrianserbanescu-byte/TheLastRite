# Laptop 1 Async Prompt

Paste this into laptop 1.

```text
You are laptop 1 for C:\dev\TheLastRite.

Read first:
- `Docs/TwoLaptopAsyncPlan.md` on branch `codex/gameplay`
- `Docs/TwoLaptop12HourDeliverables.md` on branch `codex/gameplay`
- `Docs/Laptop1CoordinatorReply.md` on branch `codex/gameplay`

Role:
- gameplay and integration machine
- own gameplay code, case flow, clue logic, ritual logic, HUD/player feedback, restart/fail/win/exit flow, and local gameplay verification

Hard rules:
- work only on `codex/gameplay`
- `main` stays untouched
- do not merge to `main`
- do not pull or manually integrate laptop 2 branch work
- keep changes inside gameplay/integration scope
- commit and push small coherent gameplay-only checkpoints
- do not force commits on the 30-minute clock; use the 30-minute cadence for status only
- a completed in-scope task is not a stop condition
- after any successful checkpoint, immediately check for blockers and continue with the next in-scope gameplay task if none exist
- do not wait for acknowledgment after a normal checkpoint

Work queue order:
1. investigation flow and opening sweep clarity
2. inspect prompts and next-move guidance
3. ritual selection clarity and fail/win messaging
4. interaction targeting and crosshair hit behavior
5. restart / exit / case completion reliability
6. cleanup inside touched gameplay code only

Required verification after meaningful changes:
- `BuildGame.cmd`
- any targeted local gameplay validation already used in this lane

Every 30 minutes, report only this:

laptop1 update

time: <local time>
branch: codex/gameplay
head: <git sha or none>
working tree: <clean / dirty>
current task: <one line>
last 30m progress: <one short paragraph or no material change>
verification: <result / not run>
blockers: <none / exact blocker>
needs coordinator: <yes / no>

When blocked:
- stop broad experimentation
- report the exact blocker and exact command/error if relevant
- preserve the working tree
- write the update into `Docs/Laptop1Status.md`
- commit and push that status update on `codex/gameplay`
- then check `Docs/Laptop1CoordinatorReply.md` on GitHub before doing anything broader

Normal reporting rule:
- write each 30-minute report to `Docs/Laptop1Status.md`
- commit and push it on `codex/gameplay`

Checkpoint rule:
- aim for a meaningful gameplay checkpoint every 2-3 hours
- commit and push sooner if a coherent verified gameplay change is already ready
- after any checkpoint, continue immediately unless a real blocker, scope drift, cross-lane dependency, failed verification needing coordinator action, or an explicit stop instruction appears

Do not touch:
- packaging scripts
- package smoke tooling
- package-lane docs
- content-only room-dressing ownership
- `codex/content`
- `main`
```
