# Local Support Verification Checklist

Last updated: 2026-04-22
Checklist owner: `chat8`
Current support package artifact: grouped `VS-S2`

This file is the docs-only support verification checklist for future shell and helper reopening work.

It is maintained as one artifact inside the grouped docs-only `VS-S2` support-prep package.

It does not activate shell implementation.

It defines the proof required before each support-owned surface can be treated as safely reopenable.

## Baseline

Treat these as accepted support baselines:

- `VS-S1` support boundary inventory
- `VS-S2` shell-activation handoff pack
- `VS-S3` support activation trigger matrix
- `VS-S4` support activation order plan

Use this checklist only after the relevant trigger gate in `Docs/LocalSupportTriggerMatrix.md` has actually been crossed.

## Proof Required Before `GP-S1` Counts As A Real Shell-First Runtime Path

All of these should be true:

- a player can launch into a title/lobby/start state without dropping directly into Apartment 302
- the shell-owned entry path is the first intentional player-facing state, not just a transient dev redirect
- the path from shell entry into the current Apartment 302 session is runnable end-to-end
- the change does not require taking over case gameplay ownership from `chat3`
- the change does not introduce framework/generalization work that belongs to `chat9`
- the shell-first path can be described concretely enough that docs and helpers can align to it later without guessing

If any of those are false:

- `GP-S1` is not yet proven
- boot config stays frozen
- helper and docs alignment stays frozen

## Proof Required Before `Config/DefaultEngine.ini` May Change

All of these should be true:

- the shell-first runtime path exists in runnable form
- the shell-first path has been locally verified, not just drafted
- changing `GameDefaultMap` or startup routing will point at a real shell entry instead of an aspirational one
- the shell entry does not depend on temporary editor-only behavior
- the current fallback if the shell entry breaks is understood before the config change is made

If any of those are false:

- keep `Config/DefaultEngine.ini` frozen
- do not redirect default boot yet

## Proof Required Before `README.md` And Helper Scripts May Align To The Shell Path

All of these should be true:

- the shell-first runtime path is the real intended launch path
- the launch path can be repeated without relying on hidden one-off setup
- the wording needed for launch docs is stable enough not to drift immediately
- the helper change is a reflection of real runtime truth, not a speculative forward note

Affected surfaces:

- `README.md`
- `OpenUnreal.cmd`
- `BuildEditor.cmd`
- `BuildGame.cmd`
- `PackageGame.cmd`
- `PlayPackagedGame.cmd`

If any of those are false:

- keep docs and helpers frozen
- do not align them early

## Proof Required Before Packaged-Helper Reopening May Begin

All of these should be true:

- packaged behavior is no longer blocked or stale enough to make the helper misleading
- the packaged startup path is understood in the context of the shell-first runtime
- the packaged helper can be updated without contradicting the currently confirmed runtime truth
- `chat2` explicitly opens packaged-helper work after the relevant runtime/package gate is clear

If any of those are false:

- packaged-helper reopening remains closed
- packaged launch notes remain frozen

## Freeze Conditions

Keep support-owned surfaces frozen if any of these remain true:

- the shell-first path is still only planned and not verified
- boot config would change before the shell path is proven
- docs/helper updates would reflect an intended future path instead of the current real one
- packaged behavior is still unstable, blocked, or secondary to the current confirmed runtime path
- a support change would require editing gameplay-owned case logic
- a support change would require generalized framework or cross-case restructuring

## Verification Outcome Rule

Before reopening any support-owned surface beyond docs-only work, the verification outcome should be clear enough to classify as one of:

- proven and ready to reopen the next support-owned surface
- not yet proven and therefore still frozen
- blocked and returned to `chat2`
- parked behind an explicit later gate

If the proof is incomplete, treat the surface as still frozen.
