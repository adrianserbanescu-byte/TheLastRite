# Two-Laptop 12-Hour Deliverables

This file defines what laptop 1 and laptop 2 should deliver over the next 12 hours while following `Docs/TwoLaptopAsyncPlan.md`.

Use this as the execution target for the current work window.

## Shared rules

- Laptop 1 stays on `codex/gameplay`.
- Laptop 2 stays on `codex/content`.
- `main` stays untouched.
- Each laptop commits and pushes only its own branch.
- Work continues from queue item to queue item without waiting after success.
- Only real blockers should be written to blocker files.

## End-of-window objective

At the end of 12 hours:

- laptop 1 should have a stronger, clearer, more reliable gameplay loop on `codex/gameplay`
- laptop 2 should have a more reliable package lane and a cleaner content/package branch on `codex/content`
- both branches should still be independently usable
- no work should require `main`
- no cross-lane merge should have happened

## Laptop 1 deliverables

### Deliverable 1: gameplay flow clarity

- opening sweep is explicit and readable
- the player is guided through the intended early clue order
- dead-end or ambiguous early prompts are reduced

### Deliverable 2: prompt and ritual clarity

- inspect prompts point to the next actionable step
- ritual selection state is understandable
- correct and wrong anchor outcomes are clearly communicated

### Deliverable 3: interaction and reliability pass

- interaction targeting feels more deliberate and less noisy
- restart / exit / completion flow is stable
- touched gameplay code remains maintainable

### Laptop 1 success signals

- local gameplay behavior matches the intended opening flow
- prompts and next-move guidance remain coherent
- local gameplay validation passes for touched systems
- `BuildGame.cmd` succeeds after meaningful checkpoints

## Laptop 2 deliverables

### Deliverable 1: reliable package lane

- launcher and packaging helpers are repo-local and predictable
- package validation can be rerun without guesswork
- package logs and outputs are easy to locate

### Deliverable 2: package validation tooling and docs

- package smoke-test, summary, and validation helpers stay usable
- package-lane docs reflect current branch reality
- repeated package validation is easier and less ambiguous

### Deliverable 3: independent content/readability progress

- content/readability or room-dressing work moves forward without touching gameplay ownership
- any material content change is validated through the package lane when needed
- branch stays clearly inside packaging/content scope

### Laptop 2 success signals

- package flow can be rerun on `codex/content`
- smoke validation clearly confirms launcher and shipping handoff behavior
- package helper scripts and docs agree with actual output paths and behavior
- content work remains independent from gameplay C++ changes

## Delivery rhythm

- aim for a meaningful checkpoint every 2-4 hours
- commit/push sooner if a coherent verified change is already ready
- do not manufacture status commits between real checkpoints
- after each successful checkpoint, continue immediately to the next queue item

## End-of-window summary

At the end of the 12-hour window, each laptop should publish one concise summary in its branch docs or commit message covering:

- latest head
- meaningful checkpoints pushed
- which deliverables are done / partial / blocked
- verification summary
- whether a reviewed shared checkpoint is needed
