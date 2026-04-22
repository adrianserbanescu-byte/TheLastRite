# Local Support Trigger Matrix

Last updated: 2026-04-22
Matrix owner: `chat8`
Current support package artifact: grouped `VS-S2`

This file is the docs-only support activation trigger matrix for future shell and helper reopening work.

It is maintained as one artifact inside the grouped docs-only `VS-S2` support-prep package.

It does not activate shell implementation.

It defines the exact gates that must be crossed before specific support-owned surfaces can be reopened.

## Baseline

Treat these as already accepted support baselines:

- `VS-S1` support boundary inventory
- `VS-S2` shell-activation handoff pack in `Docs/LocalSupportHandoff.md`

Until a gate below is crossed, support work stays frozen to docs-only coordination.

## Trigger Matrix

### Gate For `GP-S1` Shell Activation

Trigger:

- `SD-VS-1` accepted by `chat2`, or an explicit written exception from `chat2` that opens `GP-S1` early

What this gate unlocks:

- minimal title flow
- minimal lobby/start flow
- shell-first entry routing into the existing Apartment 302 session
- the future `GP-S1` write set already defined in `Docs/LocalSupportHandoff.md`

What stays frozen until this gate:

- all shell/startup implementation work
- default boot rerouting
- lobby/title/session-shell code

Do-not-touch condition:

- if `SD-VS-1` is not accepted and no explicit exception exists, do not start shell implementation

### Gate For Helper-Script Alignment

Trigger:

- a shell-first runtime path actually exists and is locally verified, and `chat2` explicitly opens helper alignment work

What this gate unlocks:

- alignment updates for:
  - `BuildEditor.cmd`
  - `BuildGame.cmd`
  - `OpenUnreal.cmd`
  - `PackageGame.cmd`
  - `PlayPackagedGame.cmd`
  - `README.md`

What stays frozen until this gate:

- helper-script edits based only on intended future shell flow
- launch-note changes based only on docs assumptions

Do-not-touch condition:

- if the shell-first runtime path is not real and verified, helper scripts stay frozen to avoid drift

### Gate For Config Boot Changes

Trigger:

- `GP-S1` startup routing exists in runnable form, and `chat2` explicitly approves changing the default boot path

What this gate unlocks:

- `Config/DefaultEngine.ini` updates that stop default boot from going straight to Apartment 302

What stays frozen until this gate:

- direct changes to `GameDefaultMap`
- editor startup rerouting
- any config-level boot change that would redirect players before the shell path is proven

Do-not-touch condition:

- if config boot changes would point at a non-verified shell entry, keep `DefaultEngine.ini` frozen

### Gate For Packaged-Helper Reopening

Trigger:

- packaged runtime truth is stable enough that `chat2` explicitly reopens packaged-helper work after the current packaging/runtime uncertainty is resolved

What this gate unlocks:

- packaged-launch helper alignment
- packaged runtime notes
- packaged path support messaging in docs

What stays frozen until this gate:

- packaged-helper assumptions
- packaged launch-note changes based on stale or blocked package behavior

Do-not-touch condition:

- if packaged behavior is still blocked, stale, or secondary to the current confirmed runtime path, keep packaged helpers frozen

## Freeze Conditions

Keep support-owned surfaces frozen when any of these are true:

- `chat3` still owns the active runtime-truth or gameplay lane and the change would overlap that lane
- `chat9` would need to own the work because it becomes framework/generalization instead of support routing
- the shell-first runtime path is still only planned, not verified
- the packaged path is still blocked or unresolved
- `chat2` has not explicitly opened the next milestone

## Boundary Risks

- `TheLastRiteGameMode` currently carries both case-flow and startup presentation weight, so shell activation must prefer new shell-owned entry surfaces over stuffing more responsibility into the current case runtime.
- helper scripts are high-drift surfaces; reopening them before a verified shell path exists will recreate stale runtime notes.
- `DefaultEngine.ini` is a high-impact boot switch; changing it before the shell path is proven would cause ownership churn and runtime confusion.
- packaged-helper reopening must trail real packaged truth, not aspirational packaging recovery.

## Use Rule

When a future support milestone is opened, re-check this matrix first.

If the gate for that category is not crossed, the work remains frozen even if the target file is support-owned in the long term.
