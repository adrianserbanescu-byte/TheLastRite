# Local Instructions To Chat9

Last updated: 2026-04-21
Instruction owner: `chat2`
Current instruction seed: the future framework lane is documented, but no active task is open yet

## Current assignment

No active milestone yet.

## Loop-break rule

- success condition:
  - define the exact reusable framework/config/plumbing result that must exist
- blocker condition:
  - define the exact point where framework execution or validation cannot continue reliably from the current environment or write set
- max retries:
  - default 2 total methods unless `chat2` explicitly changes it
  - one main attempt
  - one narrow retry
- max exploration time:
  - default 20 minutes for support/tooling-style framework preparation
- after limits are hit:
  - write blocker to `Docs/LocalFrameworkBlockers.md`
  - summarize what was tried
  - stop and wait for reassignment

## Autonomous progression rule

- when a future active framework milestone is completed and fully confirmed, continue directly to the next eligible framework milestone in `Docs/LocalFrameworkQueue.md` without waiting for a fresh prompt from `chat2`
- only do that if:
  - dependencies are satisfied
  - there is no blocker on the current milestone
  - there is no ownership conflict with `chat3` or `chat8`
  - there is no explicit hold in `Docs/LocalFrameworkStatus.md` or this file
- if the current milestone is blocked, do not skip past it unless `chat2` explicitly reassigns or reorders the lane

## Exact task

- stand by
- do not start reusable framework, generalized systems, data/config, or cross-case plumbing work yet
- wait for `chat2` to activate a clearly isolated framework milestone in `Docs/LocalFrameworkQueue.md`
- if a future instruction is issued, follow that exact write set only

## Out of scope for now

- gameplay-owned case logic
- shell/startup/menu/briefing/debrief work
- speculative framework cleanup
- queue or roadmap rewrites

## Ownership reminder

- `chat2` owns planning, queue state, and milestone reassignment
- `chat9` owns future reusable case framework, generalized systems, data/config structure, and cross-case plumbing
- that ownership is inactive until an explicit framework milestone is activated
