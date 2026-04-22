# Local Support GP-S1 Packet

Last updated: 2026-04-22
Packet owner: `chat8`
Current support milestone: grouped `VS-S2`

This file is the canonical packet inside the grouped docs-only `VS-S2` support-prep package for the future `GP-S1` shell activation lane.

It does not activate `GP-S1` early.

Use it only after `chat2` explicitly activates `GP-S1`.

## Purpose

This packet gives the future support lane one ordered doc set, one document-precedence rule, one escalation path, and one smallest safe starting action before any support-owned code or helper surface is reopened.

If a future `GP-S1` task conflicts with this packet, stop and escalate instead of widening scope.

## Cross-Lane Authority Rule

`Docs/LocalDevScaleUpContract.md` may define gameplay-side semantics for the future scale-up, but this grouped support packet is authoritative on:

- reopen boundaries
- activation boundaries
- shell/support trigger conditions
- activation order for support-owned surfaces
- verification and runtime-proof requirements before support-owned surfaces reopen

If `Docs/LocalDevScaleUpContract.md` appears to disagree with this packet on any of those boundary topics:

- this packet wins
- do not reopen the boundary here
- record the conflict in the appropriate blocker file
- escalate to `chat2`

## Ordered Prep Docs

Read these in order once `GP-S1` is truly activated:

1. `Docs/LocalSupportStatus.md`
   - role:
     - confirms the currently active support task, active timing, write boundary, current risks, and planner-owned activation state
2. `Docs/LocalSupportInstructionsToChat8.md`
   - role:
     - gives the exact worker instruction, current success condition, blocker rule, retry limits, and out-of-scope guardrails
3. this file
   - role:
     - turns the accepted support baselines into one ordered activation packet for `GP-S1`
4. `Docs/LocalSupportTriggerMatrix.md`
   - role:
     - confirms which gate must already be crossed before shell work, config work, helper alignment, or packaged-helper reopening may begin
5. `Docs/LocalSupportActivationOrder.md`
   - role:
     - defines the first support-owned write step, the opening order for later surfaces, the stop rule, and the rollback boundary
6. `Docs/LocalSupportHandoff.md`
   - role:
     - names the intended future `GP-S1` write set, the still-frozen surfaces, and the original shell-activation intent
7. `Docs/LocalSupportVerificationChecklist.md`
   - role:
     - states the proof required before the shell-first path, boot config, launch docs, or helpers may be treated as real
8. `Docs/LocalSupportRuntimeProofTemplate.md`
   - role:
     - defines the exact evidence format that future `GP-S1` work must capture before support-owned surfaces reopen
9. `Docs/LocalSupportBlockers.md`
   - role:
     - records any real ownership conflict, gate failure, or dependency stop that must go back to `chat2`
10. `Docs/LocalSupportProgressReport.md`
    - role:
      - records the latest completed support checkpoints so the future shell lane can see what is already done and what still needs planner-side acceptance

## Document Precedence

If two support docs disagree, use this rule:

1. `Docs/LocalSupportStatus.md` and `Docs/LocalSupportInstructionsToChat8.md` win on current activation state, active milestone, write boundary, and task-specific direction because they are planner-owned and define the live lane state.
2. This packet wins on the read order, doc relationship summary, and the single escalation path for future `GP-S1` activation because that is the sole purpose of this milestone.
3. `Docs/LocalSupportTriggerMatrix.md` wins on whether a category of support work is actually unlocked.
4. `Docs/LocalSupportActivationOrder.md` wins on what order reopened support surfaces must be touched.
5. `Docs/LocalSupportVerificationChecklist.md` and `Docs/LocalSupportRuntimeProofTemplate.md` win on what proof and evidence are required before a support-owned surface may stay reopened.
6. `Docs/LocalSupportHandoff.md` supplies the intended write set and frozen-surface context when the higher-precedence docs do not override it.
7. `Docs/LocalSupportBlockers.md` records a real stop, but it does not by itself authorize a new direction.
8. `Docs/LocalSupportProgressReport.md` is historical evidence only; it never overrides the live planner-owned status or instruction docs.

If a conflict still remains after applying the order above:

- stop work
- write the conflict to `Docs/LocalSupportBlockers.md`
- escalate to `chat2`

## Stop-And-Escalate Rule

Stop and return to `chat2` immediately if any of these are true:

- the current `GP-S1` request requires editing gameplay-owned clue, ritual, fail/win, case-complete, or per-case logic
- the next required change becomes framework, generalization, reusable case plumbing, or cross-case setup owned by `chat9`
- the trigger gate for the requested support surface is not actually crossed in `Docs/LocalSupportTriggerMatrix.md`
- the first shell-owned startup surface cannot be isolated without taking over gameplay-owned startup or case files
- the requested change would reopen `Config/DefaultEngine.ini`, `README.md`, or helper scripts before the shell-first path is real and verified
- planner-owned docs still point at a different milestone, different write set, or different boundary than the active `GP-S1` request

When this happens:

- do not widen scope
- do not improvise a new lane
- write the blocker or dependency stop to `Docs/LocalSupportBlockers.md`
- wait for `chat2`

## Smallest Safe First Action

Once `GP-S1` is truly activated and the shell gate is crossed, the smallest safe first action is:

- create or open the smallest shell-owned startup surface that can present a title, lobby, or start state without editing gameplay-owned case logic

That first action must stay narrower than:

- boot-config rerouting
- helper-script alignment
- launch-doc alignment
- packaged-helper reopening
- framework/generalization work

## Immediate GP-S1 Use Rule

When `GP-S1` is opened for real:

1. Re-read `Docs/LocalSupportStatus.md` and `Docs/LocalSupportInstructionsToChat8.md`.
2. Re-read this packet.
3. Confirm the shell activation gate is open in `Docs/LocalSupportTriggerMatrix.md`.
4. Use `Docs/LocalSupportActivationOrder.md` to keep the first write step narrow.
5. Use `Docs/LocalSupportVerificationChecklist.md` and `Docs/LocalSupportRuntimeProofTemplate.md` to decide whether later support-owned surfaces may reopen.
6. If any ambiguity remains, stop and escalate instead of expanding the lane.
