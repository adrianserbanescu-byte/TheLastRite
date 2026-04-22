# Local Support Activation Order

Last updated: 2026-04-22
Plan owner: `chat8`
Current support package artifact: grouped `VS-S2`

This file is the docs-only support activation order plan for the first future shell reopening pass.

It is maintained as one artifact inside the grouped docs-only `VS-S2` support-prep package.

It does not authorize shell implementation early.

It defines the first support-owned write step, the file-by-file opening order, the stop rule, the rollback boundary, and the surfaces that stay deferred until the first shell path is proven.

## Preconditions

Do not use this plan until all of these are true:

- `chat2` explicitly activates `GP-S1`
- the relevant trigger gates in `Docs/LocalSupportTriggerMatrix.md` are crossed
- the current support lane remains isolated from:
  - `chat3` gameplay ownership
  - `chat9` framework/generalization ownership

## First Future `GP-S1` Support-Owned Write Step

First step:

- create or open the smallest shell-owned startup surface that can present a title/lobby/start state without changing case gameplay logic

Why this is first:

- it gives the support lane a clean ownership anchor
- it avoids expanding the current case-only boot logic before a shell entry exists
- it keeps later boot/config/helper changes downstream of a real shell-first path instead of turning them into guesses

## File-By-File Opening Order

Open these surfaces in this order once the gate is crossed:

1. New shell-owned startup surface
   - preferred target: a new shell entry layer rather than expanding gameplay-owned case runtime files
   - goal: title/lobby/start entry only
2. Shell routing surface
   - map or startup-routing logic that connects the shell entry to the existing Apartment 302 session
   - goal: shell-first path exists in runnable form
3. `Config/DefaultEngine.ini`
   - only after the shell-first runtime path is verified
   - goal: stop default boot from dropping directly into Apartment 302
4. `README.md`
   - only after the real startup path is verified
   - goal: align docs with the actual shell-first entry
5. Helper scripts
   - `OpenUnreal.cmd`
   - `BuildEditor.cmd`
   - `BuildGame.cmd`
   - `PackageGame.cmd`
   - `PlayPackagedGame.cmd`
   - only after the new startup flow or packaged path truth actually requires alignment

## Stop Rule

Pause support work and return to `chat2` immediately if any of these happen:

- the first shell-owned startup surface cannot be isolated without editing gameplay-owned clue, ritual, fail/win, or case-complete logic
- the next required change becomes framework/generalization work instead of shell/support routing
- changing startup flow would require speculative packaged-helper updates before the path is verified
- boot/config changes would point at a shell path that is not yet runnable
- the first shell pass reveals ownership ambiguity about whether a file belongs to `chat8`, `chat3`, or `chat9`

If any of those happen:

- stop widening scope
- write the blocker or dependency state in `Docs/LocalSupportBlockers.md`
- wait for `chat2`

## Rollback Boundary

If the first shell pass cannot be verified cleanly, keep these surfaces untouched:

- `Config/DefaultEngine.ini`
- `README.md`
- `BuildEditor.cmd`
- `BuildGame.cmd`
- `OpenUnreal.cmd`
- `PackageGame.cmd`
- `PlayPackagedGame.cmd`
- gameplay-owned startup/case files

Preferred rollback rule:

- if the shell entry surface itself is not yet verified, do not propagate the change into boot config, helper scripts, or launch docs

## Deferred Surfaces

These should stay deferred until after the first shell path is proven:

- packaged-helper reopening
- packaged runtime-note alignment
- config-level default boot rerouting
- any helper-script edits
- any launch-doc edits
- any support-owned expansions beyond title/lobby/start routing

## Boundary Notes

- `TheLastRiteGameMode` should remain a protected boundary unless `chat2` explicitly reassigns part of it.
- Support should prefer adding a shell-owned entry path over mutating the case runtime into a shell.
- Helper and doc alignment is a follow-through step, not the first step.
- A proven shell path comes before convenience cleanup.
