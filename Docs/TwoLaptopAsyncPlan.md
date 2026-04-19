# Two-Laptop Async Development Plan

This is the active operating plan for independent two-laptop work through GitHub-visible files.

This plan replaces the old status-driven workflow. The laptops should no longer stop after a successful checkpoint just to report progress.

## Summary

- Laptop 1 owns gameplay/integration on `codex/gameplay`.
- Laptop 2 owns packaging + content on `codex/content`.
- `main` stays untouched.
- Each laptop commits and pushes only its own branch.
- Work is driven by a queue file on each branch.
- The coordinator intervenes only when a blocker file is pushed.
- Successful in-scope work is never a stop condition.

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
- Do not create routine progress-only commits unless reporting a blocker.

## Cadence

- Every 2-4 hours: expected meaningful checkpoint with commit/push if coherent work is ready
- Immediate: blocker report as soon as it appears after one local retry
- Every 12-15 minutes: optional heartbeat/watchdog to resume work, not to publish status

There is no routine 30-minute status-commit requirement in this mode.

## GitHub coordination files

Laptop 1 uses these files on `codex/gameplay`:

- `Docs/Laptop1Queue.md`
- `Docs/Laptop1Blocker.md`
- `Docs/Laptop1CoordinatorReply.md`

Laptop 2 uses these files on `codex/content`:

- `Docs/Laptop2AsyncPrompt.md`
- `Docs/Laptop2Queue.md`
- `Docs/Laptop2Blocker.md`
- `Docs/Laptop2CoordinatorReply.md`

Queue files define the active in-scope work order on each branch.

Blocker files are written only by the laptop that owns the branch, and only when a real blocker appears.

Coordinator reply files are written only by the coordinator.

Old status files are no longer part of the active process.

## Queue-driven workflow

Each laptop must:

1. Read the queue file on its branch
2. Take the first incomplete in-scope task
3. Work until a meaningful checkpoint or a real blocker appears
4. Commit and push coherent work when ready
5. Immediately continue to the next in-scope queue item if no blocker exists

Successful work is not a reason to pause.

## Blocker-driven coordination

Only report through the blocker file when:

- one local retry failed
- scope drift appeared
- a cross-lane dependency appeared
- verification failed and the next safe step is unclear
- a repo/workflow issue blocks normal commit/push

When blocked:

1. update the blocker file on the branch
2. commit and push the blocker file
3. check the coordinator reply file
4. follow the coordinator reply exactly

Do not check the coordinator reply file after ordinary success.

## Coordinator rules

When a blocker file arrives:

- If the blocker is local and lane-safe, give the smallest next action that stays inside that lane.
- If the blocker would require cross-lane integration, branch ownership change, or touching `main`, stop the affected laptop and hold the other lane steady.
- If a laptop drifts outside scope, tell it to stop, preserve the working tree, and return to the last in-scope task.
- If one laptop is blocked but the other is unaffected, keep the unaffected laptop moving.

When the coordinator replies:

- Update the laptop's coordinator-reply file on that branch
- Keep the reply short, concrete, and action-oriented
- Ask the laptop to report back only if the blocker persists or after the requested retry/action completes

## Reviewed shared checkpoint trigger

A reviewed shared checkpoint is needed only when:

- Laptop 2 needs gameplay behavior that exists only on `codex/gameplay`
- Laptop 1 needs package/content validation that depends on laptop 2 branch changes
- A user-facing milestone needs both lanes combined
- Either branch can no longer make meaningful progress independently

Until then, both branches stay separate and active.

## Required verification

### Laptop 1

- `BuildGame.cmd`
- Any targeted local gameplay validation already used in this lane

### Laptop 2

- `PackageGame.cmd` after material package-lane or shipped-content changes
- Packaged launcher smoke test after material package-lane or shipped-content changes
- Packaging summary/status helper output when relevant

Docs-only and ArtSource-only checkpoints do not require forced package reruns unless they affect the shipped lane or package behavior.

## Exact stop rule

A laptop may stop only for:

1. a real blocker after one local retry
2. scope drift
3. cross-lane dependency
4. failed verification with unclear next step
5. an explicit coordinator stop instruction

Everything else means: continue.
