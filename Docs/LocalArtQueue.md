# Local Art Queue

Last updated: 2026-04-22
Active objective: keep one low-risk bounded art package live in parallel with gameplay whenever a safe non-overlapping source-art scope exists
Queue owner: `chat2`

## Ownership model

- lane owner: `chat5`
- `chat5` owns source-art production and art reference packs only
- Unreal implementation, code integration, and runtime placement stay outside the art lane unless explicitly reassigned
- current-stage batching rule:
  - batch adjacent shell-art tasks when they stay inside the same art roots and do not require a planner decision between them

## Art carry-forward baseline

- accepted vertical-slice art pack:
  - `A1-A9`
- current safe expansion rule:
  - shell-facing source art may continue inside `ArtSource/*`
  - product-shell implementation in code or Unreal is still not open

## Near-term art milestone packages

| ID | Stage | Owner | State | Write scope | Dependency | Verification | Definition of done |
| --- | --- | --- | --- | --- | --- | --- | --- |
| VS-A10 | Vertical Slice | `chat5` | completed | `ArtSource/ProductShell/`, `ArtSource/SharedUI/`, optional `ArtSource/ProductShell/references/` and supporting README notes | accepted `A1-A9` | Accepted | One bounded shell-art preproduction pack exists for future shell work without requiring runtime integration. |
| GP-A-P1 | Game Prototype | `chat5` | completed | `ArtSource/ProductShell/`, `ArtSource/SharedUI/`, `ArtSource/ProductShell/references/`, `ArtSource/WorldShell/` | accepted `SD-VS-1`; accepted `VS-A10` | Accepted | Former `GP-A1` through `GP-A6` now stand as one shell-session foundation package covering core shell UI, threshold/return flow, setup/readiness, briefing/launch, and archive/replay carry-forward. |
| GP-A-P2 | Game Prototype | `chat5` | completed | `ArtSource/ProductShell/ui/`, `ArtSource/SharedUI/`, `ArtSource/ProductShell/references/` | accepted `GP-A-P1` | Accepted | Former `GP-A7` through `GP-A13` now stand as one shell-resilience and service-state package covering help/recovery, party state, permissions, exit, disconnect, compatibility, and maintenance/queue/restoration carry-forward. |
| GP-A-P3 | Game Prototype | `chat5` | completed | `ArtSource/ProductShell/ui/`, `ArtSource/SharedUI/`, `ArtSource/ProductShell/references/` plus README carry-forward inside those roots when needed | accepted `GP-A-P2` | Accepted by `chat13`; closed by `chat2` | One grouped shell implementation-handoff pack exists that consolidates the accepted source-art families into browse-first boards, reuse routes, and handoff references without touching Unreal content. |
| GP-A-P4 | Game Prototype | `chat5` | completed | `ArtSource/ProductShell/ui/`, `ArtSource/SharedUI/`, `ArtSource/ProductShell/references/` plus README carry-forward inside those roots when needed | accepted `GP-A-P3`; accepted `GP-S-P1`; accepted `ACC-GP-A-P4` review in `Docs/LocalAcceptanceStatus.md` | Accepted by `chat13`; closed by `chat2` | One grouped post-case shell source-art pack exists for result, debrief, replay, and requeue surfaces plus reusable shared components without touching Unreal content or claiming runtime proof. |
| GP-A-P5 | Game Prototype | `chat5` | accepted and closed | `ArtSource/Apartment302/props/`, `ArtSource/Apartment302/references/`, and `ArtSource/Apartment302/README.md` | accepted `GP-A-P4`; accepted Apartment 302 source-art baseline `A1-A9`; completed handoff recorded in `Docs/LocalArtHandoff.md` at `2026-04-22 15:53:49 +03:00`; accepted `ACC-GP-A-P5` in `Docs/LocalAcceptanceStatus.md` | Accepted by `chat13`; closed by `chat2` | One grouped Apartment 302 demon-variance source-art pack exists for Hollow Saint, Weeping Matron, and The Appetite read-separation boards plus bounded carry-forward notes without touching Unreal content or claiming runtime proof. |

## Queue rules

- `chat2` is the only queue editor
- keep source-art-only work separate from Unreal integration work
- active art work should usually be one meaningful package, not a string of tiny surface-specific accepts
- where practical, package acceptance happens through `Docs/LocalAcceptanceStatus.md` before `chat2` advances the art lane
- after `chat13` records acceptance, do not keep the package in a fake completion-pending state; close it or explicitly reassign the lane
- once a completed handoff is recorded, do not keep the package in fake active production state; route it to acceptance or explicitly park it
- if no next safe non-overlapping art package is ready, park the lane explicitly instead of inventing follow-on art work
- if a safe bounded art package does exist, prefer opening it instead of leaving the art lane idle by default
- do not reopen gameplay, support, or framework ownership through art packaging
- future packages are planning-only until explicitly activated
