# Two-Laptop Async Development Plan

This is the active operating plan for independent two-laptop work through GitHub-visible files.

Use this plan instead of the earlier laptop 2 bring-up flow when deciding what each machine should do next.

## Summary

- Laptop 1 owns gameplay/integration on `codex/gameplay`.
- Laptop 2 owns packaging + content on `codex/content`.
- `main` stays untouched.
- Each laptop commits and pushes only its own branch.
- Each laptop reports every 30 minutes by updating a GitHub status file on its own branch.
- Each laptop is expected to produce a meaningful pushed checkpoint every 2-3 hours, not every 30 minutes.
- The coordinator responds by updating a GitHub coordinator-reply file on the affected laptop's branch.
- The laptops check GitHub for coordinator replies before resuming from a blocker.

## Branch ownership

- Laptop 1 may edit, commit, and push only `codex/gameplay`.
- Laptop 2 may edit, commit, and push only `codex/content`.
- Neither laptop may merge to `main`.
- Neither laptop may manually integrate the other laptop's unreviewed branch work.
- Cross-lane integration happens only after a reviewed shared checkpoint is chosen.

## Lane scope

### Laptop 1 scope

- Gameplay C++ logic
- Investigation flow
- Clue logic and ritual logic
- HUD prompts and next-move guidance
- Restart / fail / win / exit flow
- Local gameplay verification

### Laptop 1 out of scope

- Packaging scripts and smoke-test tooling
- Packaging docs except tiny gameplay-reality corrections
- Content-only room dressing
- Any edits on `codex/content` or `main`

### Laptop 2 scope

- Packaging scripts and launcher helpers
- Packaged-build smoke tests and validation tools
- Packaging docs and package-lane summaries
- Content/readability work and room dressing that does not require gameplay-code ownership
- Package-ready branch hygiene and deterministic output/log behavior

### Laptop 2 out of scope

- Gameplay C++ logic
- Pulling or manually integrating laptop 1 local work
- Any edits on `codex/gameplay` or `main`

## Commit and push policy

- Use small coherent commits.
- Keep each commit inside the assigned lane.
- Push after each meaningful checkpoint.
- Do not create mixed gameplay/package commits.

## Cadence

- Every 30 minutes: status update only
- Every 2-3 hours: expected meaningful checkpoint with commit/push if coherent work is ready
- Immediate: blocker report as soon as it appears

Do not force a commit every 30 minutes. The 30-minute cadence is for coordination, not for shipping a feature slice on the clock.

## GitHub coordination files

Laptop 1 uses these files on `codex/gameplay`:

- `Docs/Laptop1Status.md`
- `Docs/Laptop1CoordinatorReply.md`

Laptop 2 uses these files on `codex/content`:

- `Docs/Laptop2Status.md`
- `Docs/Laptop2CoordinatorReply.md`

Status files are written by the laptop that owns the branch.

Coordinator reply files are written by the coordinator.

## 30-minute reporting format

Both laptops update their status file in this exact shape:

```text
laptop<N> update

time: <local time>
branch: <branch>
head: <git sha or none>
working tree: <clean / dirty>
current task: <one line>
last 30m progress: <one short paragraph or no material change>
verification: <result / not run>
blockers: <none / exact blocker>
needs coordinator: <yes / no>
```

If nothing meaningful changed, `last 30m progress` may be `no material change`.

Each 30-minute report must be committed and pushed on the laptop's own branch.

Meaningful checkpoints should usually happen every 2-3 hours, or sooner if a coherent verified change is already ready.

## Coordinator rules

When a status-file report arrives:

- If `blockers: none` and `needs coordinator: no`, do nothing.
- If the blocker is local and lane-safe, give the smallest next action that stays inside that lane.
- If the blocker would require cross-lane integration, branch ownership change, or touching `main`, stop the affected laptop and hold the other lane steady.
- If a laptop drifts outside scope, tell it to stop, preserve the working tree, and return to the last in-scope task.
- If one laptop is blocked but the other is unaffected, keep the unaffected laptop moving.

When the coordinator replies:

- Update the laptop's coordinator-reply file on that branch
- Keep the reply short, concrete, and action-oriented
- Ask the laptop to report back through its status file after taking that action

## Reviewed shared checkpoint trigger

A reviewed shared checkpoint is needed only when:

- Laptop 2 needs gameplay behavior that exists only on `codex/gameplay`
- Laptop 1 needs package/content validation that depends on laptop 2 branch changes
- A user-facing milestone needs both lanes combined
- Either branch can no longer make meaningful progress independently

Until then, both branches stay separate and active.

## Work queue: laptop 1

1. Investigation flow and opening sweep clarity
2. Inspect prompts and next-move guidance
3. Ritual selection clarity and fail/win messaging
4. Interaction targeting and crosshair hit behavior
5. Restart / exit / case completion reliability
6. Cleanup inside touched gameplay code only

### Laptop 1 required verification

- `BuildGame.cmd`
- Any targeted local gameplay validation already used in this lane

### Laptop 1 stop-and-report conditions

- Git/index or repo permissions fail again
- A gameplay fix now depends on packaging/content branch changes
- A required file falls clearly into laptop 2 ownership
- The next safe step would change branch/workflow assumptions
- A coordinator reply is required before continuing

## Work queue: laptop 2

1. Launcher and packaging helper reliability
2. Package smoke-test and summary tooling
3. Deterministic package output/log behavior
4. Package-lane docs
5. Content/readability and room-dressing work that stays gameplay-code independent
6. Re-run package + smoke validation after material package/content changes

### Laptop 2 required verification

- `PackageGame.cmd`
- Packaged launcher smoke test
- Packaging summary/status helper output when relevant

### Laptop 2 stop-and-report conditions

- Packaging fails and the fix points into gameplay-code ownership
- Content work now requires gameplay C++ changes
- Branch scope would expand beyond packaging/content
- The next safe step would require consuming laptop 1 branch work
- A coordinator reply is required before continuing

## Quick coordinator actions

Use these defaults when replying quickly:

- Build failure in-lane: retry once with exact failing command + capture log path, then report blocker
- Git lock/ACL issue: stop retries after one reproducer, preserve working tree, report exact command + error
- Scope drift: stop editing the out-of-scope file and return to the last in-scope task
- Cross-lane dependency: stop the blocked laptop, keep the other lane moving, ask for a reviewed shared checkpoint
- No material change: stay quiet and continue current task

## GitHub-only workflow rule

Assume both laptops can read only GitHub-visible branch files.

That means:

- active instructions must live in committed branch files
- blocker reports must be pushed through the laptop's status file
- coordinator guidance must be pushed through the laptop's coordinator-reply file
- no laptop should depend on local-only files from this machine
