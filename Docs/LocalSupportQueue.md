# Local Support Queue

Last updated: 2026-04-22
Active objective: keep support ready for the next safe package while gameplay runs `GP-C2`, art carries one low-risk parallel package, and acceptance stays live on arriving evidence
Queue owner: `chat2`

## Ownership model

- lane owner: `chat8`
- future ownership from GP-C1 onward:
  - product shell
  - startup flow
  - menu/briefing/debrief loop
  - tooling/support/runtime helpers
- batching rule:
  - batch adjacent shell/startup/support tasks when they stay inside the same support-owned shell flow, use the same verification style, and do not cross a real planner gate

## Near-term support milestone packages

| ID | Stage | Owner | State | Write scope | Dependency | Verification | Definition of done |
| --- | --- | --- | --- | --- | --- | --- | --- |
| VS-S1 | Vertical Slice | `chat8` | completed | `Docs/LocalSupport*.md` | none | Accepted | The future support-owned startup, shell, and helper surfaces are inventoried and the first safe post-slice write set is documented. |
| VS-S2 | Vertical Slice | `chat8` | completed | `Docs/LocalSupportHandoff.md`, `Docs/LocalSupportTriggerMatrix.md`, `Docs/LocalSupportActivationOrder.md`, `Docs/LocalSupportVerificationChecklist.md`, `Docs/LocalSupportRuntimeProofTemplate.md`, `Docs/LocalSupportGP-S1Packet.md`, `Docs/LocalSupportProgressReport.md`, optional `Docs/LocalSupportBlockers.md` | accepted `VS-S1` | Accepted | One grouped docs-only `GP-S1` activation prep package is internally coherent. |
| GP-S-P1 | Game Prototype | `chat8` | accepted | startup routing, title/lobby shell surfaces, briefing/tool-grab/case-launch shell flow, shell entry code, helper-facing launch support | accepted `SD-VS-1`; accepted grouped `VS-S2`; accepted `M10`; accepted `ACC-GP-S-P1` | Accepted deterministic shell traversal evidence | Former `GP-S1` and `GP-S2` now stand as one shell-session-flow package: a player can launch into a shell-first session flow and move cleanly through title, lobby, briefing, tool grab, and case launch without editor-only assumptions. |
| GP-S-P2 | Game Prototype | `chat8` | future | case-result handoff, debrief flow, replay/requeue shell flow, post-case return routing, and helper-facing result support | accepted `GP-S-P1`; stable case-result payload exposed by `GP-C2` | `chat7` deterministic shell-loop verification when automatable plus immediate `chat13` acceptance intake on credible evidence | A player can finish a case, enter debrief, and replay or requeue without editor-only assumptions or gameplay/framework ownership drift. |

## Queue rules

- `chat2` is the only queue editor
- support prep steps that share the same docs-only write set, dependency gate, owner, and verification style should stay grouped instead of being split into tiny milestone churn
- keep only one live support package unless `chat2` explicitly opens safe parallel support work
- route credible support evidence to `chat13` immediately; do not wait for planner cleanup before acceptance starts
- when shell proof is automatable, prefer an explicit `chat7` deterministic verification task over keeping repeatable proof inside the support lane
- do not overlap gameplay-owned files, shell code owned by other lanes, or framework files during the current prototype window
