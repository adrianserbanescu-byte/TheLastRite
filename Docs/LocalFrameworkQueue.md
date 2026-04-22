# Local Framework Queue

Last updated: 2026-04-22
Active objective: keep framework parked until `GP-C2` and the next post-case support reopen expose a truly isolated shared bootstrap write set
Queue owner: `chat2`

## Ownership model

- lane owner: `chat9`
- future ownership:
  - reusable case framework
  - generalized systems
  - data/config structure
  - cross-case plumbing
- current-stage rule:
  - do not activate framework work unless the write set is truly isolated from active gameplay and shell/startup prototype work

## Near-term framework milestones

| ID | Stage | Owner | State | Write scope | Dependency | Verification | Definition of done |
| --- | --- | --- | --- | --- | --- | --- | --- |
| GP-F1 | Game Prototype | `chat9` | parked | future shared bootstrap, demon registration, and config/data structures | accepted `GP-C1`; accepted `GP-S-P1`; cannot open safely until `GP-C2` and the next post-case support reopen expose a clean isolated shared bootstrap seam | Future framework review | Three demons can be registered and bootstrapped through shared data/config and bootstrap plumbing without duplicating Apartment 302 startup assumptions. |
| GP-F2 | Game Prototype | `chat9` | future | shared clue truth/bait mapping, evidence-slot variation, and run-state plumbing | `GP-F1` | Future framework review | Demon-local gameplay plugs into shared apartment variation systems instead of hardcoded one-case logic. |
| GP-F3 | Game Prototype | `chat9` | future | reusable ritual phase systems and role-state plumbing | `GP-F2` | Future framework review | The apartment runs a reusable multi-phase ritual instead of one-off final-choice logic only. |

## Queue rules

- `chat2` is the only queue editor
- do not activate `chat9` unless the write set is clearly isolated from:
  - active gameplay/code work
  - active support/tooling work
  - active shell art preproduction decisions that still define surface boundaries
- parked means no safe parallel framework write set exists yet, not that the lane is forgotten
