# Local Support Runtime Proof Template

Last updated: 2026-04-22
Template owner: `chat8`
Current support package artifact: grouped `VS-S2`

This file is the docs-only runtime-proof evidence template for future shell and helper reopening work.

It is maintained as one artifact inside the grouped docs-only `VS-S2` support-prep package.

It does not activate shell implementation.

It defines the exact evidence that must be captured before support-owned surfaces can be reopened with confidence.

## Use Rule

Use this template only after:

- `chat2` explicitly activates the relevant support milestone
- the trigger conditions in `Docs/LocalSupportTriggerMatrix.md` are crossed
- the support lane is operating inside its approved write set

If the evidence below is incomplete, the affected support-owned surface stays frozen.

## Evidence Required Before `GP-S1` Counts As A Real Shell-First Runtime Path

Record all of the following:

- shell entry surface:
  - exact surface or route name that the player first sees
- entry behavior:
  - what happens from launch to title/lobby/start
- transition proof:
  - exact path from shell entry into the existing Apartment 302 session
- verification method:
  - how the shell-first path was locally verified
- ownership check:
  - confirmation that the shell-first path did not require taking over gameplay-owned case logic
- fallback note:
  - what still happens if the shell entry is bypassed or fails

If any of the above cannot be stated concretely:

- `GP-S1` is not yet proven

## Evidence Required Before `Config/DefaultEngine.ini` May Change

Record all of the following:

- current verified shell entry target:
  - the exact path that default boot would be redirected to
- current verified boot result:
  - what the player actually sees after boot
- non-editor dependency check:
  - confirmation that the path is not relying on editor-only assumptions
- rollback path:
  - the exact fallback if the new default boot route fails

If any of the above are missing:

- keep `Config/DefaultEngine.ini` frozen

## Evidence Required Before `README.md` And Helper Scripts May Align

Record all of the following:

- current real launch path:
  - the exact steps or helper route that now represent the intended shell-first startup
- repeatability:
  - confirmation that the path can be repeated consistently
- scope of alignment:
  - which of these surfaces actually need changes:
    - `README.md`
    - `OpenUnreal.cmd`
    - `BuildEditor.cmd`
    - `BuildGame.cmd`
    - `PackageGame.cmd`
    - `PlayPackagedGame.cmd`
- drift check:
  - why the new wording or helper behavior reflects runtime truth rather than planned future behavior

If any of the above are missing:

- keep docs and helpers frozen

## Evidence Required Before Packaged-Helper Reopening May Begin

Record all of the following:

- packaged runtime status:
  - whether packaged behavior is currently reliable enough to support alignment
- packaged shell entry result:
  - what the packaged build does on startup in the shell-first path
- helper justification:
  - why packaged-helper reopening is now warranted
- contradiction check:
  - confirmation that the packaged path no longer conflicts with the currently confirmed runtime truth

If any of the above are missing:

- packaged-helper reopening remains frozen

## Freeze Rule

If any required proof entry is unknown, unverified, blocked, or only aspirational:

- do not reopen the affected support-owned surface
- record the missing proof as still incomplete
- return to `chat2` if the missing proof creates an ownership or scope conflict
