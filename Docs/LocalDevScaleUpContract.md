# Apartment 302 Scale-Up Contract

Last updated: 2026-04-22 02:30:04 +03:00
Contract owner: `chat3`
Current milestone: `M10`
Current slice runtime truth: `C:\dev\TheLastRite\Binaries\Win64\TheLastRite.exe`

This file is the docs-only Apartment 302 scale-up contract for the post-slice prototype transition.

It does not activate shell implementation, framework implementation, package recovery, or live replay work.

## Cross-Lane Authority Rule

This contract and the grouped support-prep package both describe future post-slice boundaries, but they do not have equal authority over every kind of boundary.

The authority rule is:

- `Docs/LocalSupportGP-S1Packet.md` and its grouped support package are authoritative on:
  - reopen boundaries
  - activation boundaries
  - trigger conditions for future shell/support work
  - activation order for future support-owned surfaces
  - verification and runtime-proof requirements before support-owned surfaces reopen
- this contract is authoritative on:
  - gameplay semantics
  - clue ownership semantics
  - ritual-readiness semantics
  - wrong-anchor consequences
  - fail visibility and case-outcome semantics
- if this contract appears to disagree with the grouped support packet on reopen or activation boundaries:
  - the grouped support packet wins
  - do not override it here
  - record the conflict in `Docs/LocalDevBlockers.md`
  - escalate to `chat2`

## Working Scope

The contract locks only these gameplay-side semantics:

- clue ownership semantics
- ritual-readiness semantics
- wrong-anchor consequences
- fail visibility and outcome boundaries
- shell/framework handoff boundaries that later lanes must preserve

Do not use this file to reopen:

- startup routing decisions
- helper-script alignment
- config reopen rules
- shell activation order
- package-lane truth
- framework ownership or implementation details

## 1. Clue Ownership Semantics

- Clues are case-logic state, not shell state.
- A clue becomes owned by the case only when an in-world inspect/use action is accepted by the case as a readable clue event.
- The opening route remains body-first in gameplay logic even if other readable props are visible in the room.
- Bait may appear early, but bait must not silently replace the body-first chain as the authoritative case route.
- Truth and bait are both allowed to be readable, but only corroborated case reads may advance the accepted clue chain toward ritual certainty.
- Future shell or framework work may change presentation, storage, or replication shape, but it must not redefine what counts as owned truth, owned bait, or opening-route progression without an explicit gameplay milestone.

## 2. Ritual-Readiness Semantics

- Ritual readiness belongs to the accepted clue chain, not to shell progression.
- The case becomes ritual-ready only after enough corroborating reads support the accepted anchor theory.
- Ritual readiness must continue to mean "the case has enough evidence to commit" rather than "the UI advanced to the ritual step."
- Future co-op work may distribute clue contribution across players, but readiness must still come from confirmed case evidence rather than timer state, lobby state, menu flow, or role assignment alone.

## 3. Wrong-Anchor Consequences

- Wrong-anchor selection is an immediate gameplay failure, not a soft mismatch or advisory warning.
- The failure must remain legible as a logic error against the confirmed clue chain.
- Bait is allowed to tempt the player, but the fail state must still teach why the chosen anchor was unsupported.
- Future ritual spectacle, co-op participation, or aftermath layers may expand presentation, but they must preserve the core rule that a wrong anchor resolves the case as failure inside gameplay.

## 4. Fail Visibility And Outcome Boundaries

- Fail and win outcomes must remain clearly visible to the player in both HUD/case text and world consequence.
- A correct anchor seals the case and unlocks exit closure.
- A wrong anchor fails the case immediately and must remain visibly distinct from success.
- Outcome visibility is gameplay-owned. Future shell work may wrap briefing, debrief, or replay around those outcomes, but it must not blur the line between:
  - investigation still in progress
  - failed case
  - sealed case with exit unlocked
- Future framework work may standardize transport, persistence, or replication of those states, but it must preserve those same outcome boundaries.

## 5. Shell And Framework Handoff Boundaries

- `chat8` may build future shell/startup/menu/briefing/debrief flow around the accepted slice, but may not redefine clue truth, ritual readiness, wrong-anchor logic, or case outcome semantics.
- `chat9` may later generalize reusable clue, ritual, session, or replication systems, but must treat this contract as the current behavioral lock for Apartment 302 gameplay semantics.
- Any future shell or framework abstraction must preserve the accepted runtime truth that the current playable slice baseline is `C:\dev\TheLastRite\Binaries\Win64\TheLastRite.exe`.
- Reopen and activation boundaries for future support-owned surfaces are governed by `Docs/LocalSupportGP-S1Packet.md`, not by this contract.
- If a future lane needs to change any gameplay rule recorded here, that is a planner-owned gameplay-scope decision and requires a new explicit milestone rather than silent drift during shell or framework work.

## 6. Deliberate Non-Decisions

- This contract does not define future menu, lobby, briefing, or debrief UX.
- This contract does not define future generalized data schemas or replication architecture.
- This contract does not reopen package recovery, live replay validation, or accepted gameplay fixes.
- This contract does not lock future three-demon content behavior beyond preserving the current Apartment 302 / Hollow Saint slice semantics as the baseline reference that later work must intentionally extend rather than accidentally rewrite.
