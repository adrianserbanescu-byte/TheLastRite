# Two-Laptop 12-Hour Deliverables

This file defines what laptop 1 and laptop 2 should deliver over the next 12 hours while following `Docs/TwoLaptopAsyncPlan.md`.

Use this as the execution target for the current work window.

## Shared rules

- Laptop 1 stays on `codex/gameplay`.
- Laptop 2 stays on `codex/content`.
- `main` stays untouched.
- Each laptop commits and pushes only its own branch.
- Each laptop reports every 30 minutes by updating and pushing its GitHub status file using the format in `Docs/TwoLaptopAsyncPlan.md`.
- Each laptop should aim for a meaningful pushed checkpoint roughly every 2-3 hours, not every 30 minutes.
- If a blocker appears, stop broad experimentation and ask the coordinator for the smallest next safe action.

## End-of-window objective

At the end of 12 hours:

- laptop 1 should have a stronger, clearer, more reliable gameplay loop on `codex/gameplay`
- laptop 2 should have a more reliable package lane and a cleaner content/package branch on `codex/content`
- both branches should still be independently usable
- no work should require `main`
- no cross-lane merge should have happened

## Laptop 1: 12-hour deliverables

Laptop 1 owns gameplay/integration delivery.

### Deliverable 1: investigation flow clarity

Expected outcome:

- opening sweep is explicit and readable
- the player is guided through the intended early clue order
- dead-end or ambiguous early prompts are reduced

Acceptance signal:

- local gameplay behavior matches the intended opening flow
- prompts and next-move guidance remain coherent across the opening phase

### Deliverable 2: prompt and ritual clarity

Expected outcome:

- inspect prompts point to the next actionable step
- ritual selection state is understandable
- correct and wrong anchor outcomes are clearly communicated

Acceptance signal:

- case progression is understandable without guessing
- failure/win state messaging does not conflict with current case state

### Deliverable 3: interaction and reliability pass

Expected outcome:

- interaction targeting feels more deliberate and less noisy
- restart / exit / completion flow is stable
- touched gameplay code is cleaned up enough to remain maintainable

Acceptance signal:

- local gameplay validation passes for touched systems
- `BuildGame.cmd` succeeds after meaningful checkpoints

### Suggested time split

Hour 0-4:

- investigation flow and opening sweep clarity
- at least one stable gameplay checkpoint committed/pushed during this window

Hour 4-8:

- inspect prompts, next-move guidance, ritual clarity, fail/win messaging
- at least one stable gameplay checkpoint committed/pushed during this window

Hour 8-12:

- interaction targeting, restart/exit/completion reliability, cleanup inside touched gameplay code
- final stable checkpoint commit/push and end-of-window summary

### Required laptop 1 verification

- run `BuildGame.cmd` after each meaningful gameplay checkpoint
- use the existing targeted gameplay validation already used in this lane when relevant

### Required laptop 1 end-of-window report

```text
laptop1 12h summary

branch: codex/gameplay
latest head: <sha>
commits pushed: <count or list>
deliverable 1: <done / partial / blocked>
deliverable 2: <done / partial / blocked>
deliverable 3: <done / partial / blocked>
verification: <summary>
remaining blockers: <none / list>
needs reviewed shared checkpoint: <yes / no>
```

## Laptop 2: 12-hour deliverables

Laptop 2 owns packaging + content delivery.

### Deliverable 1: reliable package lane

Expected outcome:

- launcher and packaging helpers are repo-local and predictable
- package validation can be rerun without guesswork
- package logs and outputs are easy to locate

Acceptance signal:

- package flow can be rerun on `codex/content`
- smoke validation clearly confirms launcher and shipping handoff behavior

### Deliverable 2: package validation tooling and docs

Expected outcome:

- package smoke-test, summary, and validation helpers stay usable
- package-lane docs reflect current branch reality
- repeated package validation is easier and less ambiguous

Acceptance signal:

- package helper scripts and docs agree with actual output paths and behavior
- package-lane status can be summarized quickly after a validation run

### Deliverable 3: independent content/readability progress

Expected outcome:

- content/readability or room-dressing work moves forward without touching gameplay ownership
- any material content change is validated through the package lane when needed
- branch stays clearly inside packaging/content scope

Acceptance signal:

- content work remains independent from gameplay C++ changes
- `PackageGame.cmd` and smoke validation still pass after material content/package changes

### Suggested time split

Hour 0-4:

- launcher/package helper reliability
- at least one stable package-lane checkpoint committed/pushed during this window

Hour 4-8:

- package smoke-test, summary tooling, deterministic output/log behavior, docs
- at least one stable package-lane checkpoint committed/pushed during this window

Hour 8-12:

- content/readability or room-dressing work inside lane
- rerun package + smoke validation after material changes
- final stable checkpoint commit/push and end-of-window summary

### Required laptop 2 verification

- run `PackageGame.cmd` after meaningful package/content checkpoints
- run packaged launcher smoke validation after material packaging/content changes
- use package summary/status helper output when relevant

### Required laptop 2 end-of-window report

```text
laptop2 12h summary

branch: codex/content
latest head: <sha>
commits pushed: <count or list>
deliverable 1: <done / partial / blocked>
deliverable 2: <done / partial / blocked>
deliverable 3: <done / partial / blocked>
verification: <summary>
remaining blockers: <none / list>
needs reviewed shared checkpoint: <yes / no>
```

## Coordinator expectations during the 12 hours

- keep both lanes moving independently whenever possible
- if one lane is blocked, keep the other lane active
- do not collapse the work into a shared checkpoint unless a real dependency appears
- prefer the smallest next safe action when replying to blockers
- use the GitHub coordinator-reply file on the affected branch when a laptop is blocked
- treat the 30-minute cadence as coordination/reporting, not as a requirement to manufacture commits
