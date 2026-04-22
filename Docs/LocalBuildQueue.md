# Local Build Queue

Last updated: 2026-04-22
Queue owner: `chat2`
Active objective: keep explicit build, package, and deterministic verification work aligned with active milestones without dragging repeatable proof automation back into the implementation lanes

## Build policy

- `chat2` is the only build-queue editor
- `chat7` is the only build/package/archive worker and the default deterministic verification worker for scripted proof paths
- `chat7` is also the default evidence-packaging lane for automation-friendly proof runs:
  - run the bounded scripted verification
  - capture the logs and artifacts
  - package the evidence bundle so `chat13` can review it immediately
- do not run `PackageGame.cmd` after every milestone by default
- after an accepted code milestone, queue `BuildGame.cmd` when the accepted work changes runtime code, gameplay flow, UI, startup, launch helpers, packaging flow, or player-facing behavior
- after an accepted art milestone, queue a build only if the accepted art is already integrated into the runtime or the milestone explicitly requires runtime/package verification
- if a proof path can be automated and repeated safely, open that scripted verification here on `chat7` instead of keeping it inside `chat3` or `chat8`
- queue `PackageGame.cmd` only for:
  - package-lane milestones
  - runtime-truth milestones
  - slice/stage acceptance gates
  - explicit fresh playable build requests
- if no build is warranted, mark the item as `none`

## Build tasks

| ID | Source milestone | Accepted | Build action | Reason | Dependency | State | Output expectation | Verification gate |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| B1 | `M3` | yes | `BuildGame.cmd` | `M3` changed accepted runtime gameplay code, HUD feedback, ritual outcome, and exit-closure behavior, so the new build lane needed one clean chat7-owned build archive entry for the current accepted gameplay baseline. | `none` | completed | `Binaries\Win64\TheLastRite.exe` and one archived `BuildGame.cmd` result with the produced UBT log path | `BuildGame.cmd` exits successfully and chat7 records the result in `Docs/LocalBuildArchive.md` |
| B2 | `A6` | yes | `none` | `A6` is accepted source-art prep only and is not integrated into the playable runtime. | `none` | completed | no build output expected | `No build required` |
| B3 | `M5` | yes | `PackageGame.cmd` | `M5` warranted package verification originally, but there is no active dependency blocking the decision now and packaged verification is not needed for the current stage. `chat2` has canceled current-stage package recovery instead of leaving `B3` in review churn. | reopen only if `chat2` creates a new explicit package milestone later | canceled | no current-stage package output expected | No active package task. If packaged verification becomes necessary later, create a new explicit milestone instead of reviving `B3`. |
| B28 | `GP-A-P2` | yes | `none` | `GP-A-P2` is an accepted grouped source-art-only shell-resilience and service-state package. It is not runtime-integrated, so no build is warranted. | none | completed | no build output expected | `No build required` |

## Queue rules

- only items explicitly set by `chat2` belong here
- only the topmost `active` build or deterministic verification item is in scope for `chat7`
- `ready`, `queued`, `blocked`, `completed`, and `future` are planning states, not implicit permission to run commands
- if there is no `active` build or deterministic verification item, `chat7` should wait
- `chat7` records completed build or scripted verification results in `Docs/LocalBuildArchive.md`
- `chat7` should attach or reference the exact logs and artifacts needed for `chat13` acceptance whenever the task is a deterministic proof run
- `chat7` records real build or scripted verification blockers in `Docs/LocalBuildBlockers.md`
