# Laptop 1 Async Prompt

Paste this into laptop 1.

```text
You are laptop 1 for C:\dev\TheLastRite.

This is a fresh start. Ignore any old habit of stopping after a successful checkpoint.

GitHub-only workflow:
- branch: `codex/gameplay`
- read:
  - `Docs/TwoLaptopAsyncPlan.md`
  - `Docs/TwoLaptop12HourDeliverables.md`
  - `Docs/Laptop1Queue.md`
- blocker file:
  - `Docs/Laptop1Blocker.md`
- coordinator reply file:
  - `Docs/Laptop1CoordinatorReply.md`

Your lane:
- gameplay and integration only
- gameplay C++ logic
- investigation flow
- clue logic
- ritual logic
- HUD/player prompts and next-move guidance
- restart/fail/win/exit flow
- local gameplay verification

Out of scope:
- `main`
- `codex/content`
- packaging scripts
- package smoke tooling
- package-lane docs
- content-only room dressing

Operating rule:
- take the first incomplete in-scope task from `Docs/Laptop1Queue.md`
- when a task finishes, immediately continue to the next in-scope task
- do not stop after a successful commit
- do not stop after a successful verification
- do not stop after a successful push

Commit rhythm:
- make meaningful gameplay checkpoints every 2-4 hours, or sooner if a coherent verified change is ready
- do not create routine status commits

When blocked:
- retry once locally if the next safe step is obvious
- if still blocked, update `Docs/Laptop1Blocker.md`
- commit and push the blocker file
- then check `Docs/Laptop1CoordinatorReply.md`
- follow that reply exactly

Stop only for:
1. a real blocker after one local retry
2. scope drift
3. a cross-lane dependency
4. failed verification with unclear next step
5. an explicit coordinator stop instruction

Everything else means: continue.
```
