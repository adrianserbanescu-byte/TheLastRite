# Old Chat Summary

Status: historical reference only as of 2026-04-21

Use the `Docs/LocalDev*.md` files for active coordination. This note is preserved only for older project context.

This note captures the key project context carried over from the earlier chat before work continued in `C:\dev\TheLastRite`.

## Repo and runtime

- The project was moved from OneDrive into `C:\dev\TheLastRite`.
- This workspace is now the active repo location.
- `main` must stay runnable.
- The playable target is the packaged Windows build, not the raw Unreal binary under `Binaries\Win64`.
- The expected playable executable path is:

```text
C:\dev\TheLastRite\Packaged\Windows\TheLastRite.exe
```

- `PlayPackagedGame.cmd` exists to launch that packaged build directly.

## Current game target

- The active product target is not a broad multi-case game.
- The current target is a single polished Unreal vertical slice.
- That vertical slice is **Apartment 302**.
- The target demon for the case is **Hollow Saint**.

## Core loop

The intended gameplay loop is:

```text
investigate -> deduce -> ritual -> exit
```

The player should:

1. Read the room.
2. Separate true clues from theatrical bait.
3. Commit to the correct ritual interpretation.
4. Exit after a successful seal.

For the current case direction:

- the mirror path is bait
- the child-facing / nursery pattern is the real read

## Documents and source of truth

- `Docs\MasterGameDesignDocument.md` is intended to be the source-of-truth design document for the current game direction.
- Older browser-prototype material still exists in the repo.
- That prototype material is now reference material, not the active shipping target.

## Machine split

- **Laptop 1** is the gameplay / integration machine.
- **Laptop 2** is the content / packaging machine.
- Laptop 1 waits until laptop 2 is ready before certain integration steps.

## Practical focus

The current emphasis is on keeping these areas coherent in the Unreal slice:

- packaging
- launch flow
- onboarding
- clue readability
- case logic

## New Chat Verification Checklist

When opening a fresh chat in the `TheLastRite` project, use this checklist to confirm the new chat is attached to the correct repo and understands the current state.

Ask the new chat to verify:

1. current working directory
2. current git branch
3. latest commit hash and commit message
4. `git status`
5. whether these files exist:
   - `C:\dev\TheLastRite\Docs\MasterGameDesignDocument.md`
   - `C:\dev\TheLastRite\Docs\OldChatSummary.md`
   - `C:\dev\TheLastRite\README.md`
   - `C:\dev\TheLastRite\PlayPackagedGame.cmd`
6. whether the repo appears to be the moved version from OneDrive to `C:\dev\TheLastRite`
7. whether the packaged build path is:

```text
C:\dev\TheLastRite\Packaged\Windows\TheLastRite.exe
```

Also ask the new chat to verify:

- `C:\dev\TheLastRite\TheLastRite.uproject` exists
- `BuildEditor.cmd` exists
- `BuildGame.cmd` exists
- `PackageGame.cmd` exists
- all local modified or untracked files
- whether `README.md` matches the current repo reality
- whether it understands:
  - laptop 1 = gameplay / integration
  - laptop 2 = content / packaging
  - `main` must stay runnable
  - two-laptop execution waits until laptop 2 is ready

Then ask it to report:

1. what it is confident about
2. what it is unsure about
3. what the next safe action should be

## Verification Snapshot

Verification run date: 2026-04-19

Fresh-chat verification against the current workspace produced this result:

- Current working directory: `C:\dev\TheLastRite`
- Current git branch: `codex/gameplay`
- Latest commit:
  - `9951851b29659f04fffaf5a1ce643c42f2b53b3f`
  - `Stabilize packaging and case feedback`
- `git status` at verification time:
  - modified: `PlayPackagedGame.cmd`
  - modified: `README.md`
  - modified: `Source/TheLastRite/Private/TheLastRiteGameMode.cpp`
  - modified: `Source/TheLastRite/Private/TheLastRiteHUD.cpp`
  - untracked: `Docs/MasterGameDesignDocument.md`
  - untracked: `Docs/OldChatSummary.md`
  - untracked: `claimr_deck_extracted_text.txt`
- Verified present:
  - `C:\dev\TheLastRite\Docs\MasterGameDesignDocument.md`
  - `C:\dev\TheLastRite\Docs\OldChatSummary.md`
  - `C:\dev\TheLastRite\README.md`
  - `C:\dev\TheLastRite\PlayPackagedGame.cmd`
  - `C:\dev\TheLastRite\TheLastRite.uproject`
  - `C:\dev\TheLastRite\BuildEditor.cmd`
  - `C:\dev\TheLastRite\BuildGame.cmd`
  - `C:\dev\TheLastRite\PackageGame.cmd`
  - `C:\dev\TheLastRite\Packaged\Windows\TheLastRite.exe`
- The repo appears to be the moved version from OneDrive now living at `C:\dev\TheLastRite`.
- No tracked-file `OneDrive` references were found during verification.
- `README.md` mostly matches repo reality:
  - the Unreal packaged-build guidance is accurate
  - the browser prototype material also still exists in the tree
  - the file mixes active Unreal direction with older prototype context, so it is accurate but not tightly focused

### Confidence

- This is the correct repo rooted at `C:\dev\TheLastRite`.
- The packaged Windows build is the intended playable target.
- The active game target is the Unreal vertical slice for Apartment 302 / Hollow Saint.
- The expected launcher and build scripts are present.

### Uncertainty

- The workspace supports the claim that the repo was moved from OneDrive, but the move history itself is not proven from git metadata alone.
- Laptop readiness is a process-state question, not something the repo can verify directly.
- The untracked docs may still be in-progress and not yet part of the committed project baseline.

### Next Safe Action

- Keep `main` runnable.
- Continue work on `codex/gameplay` until the current docs and gameplay/package changes are intentionally reviewed.
- Before any packaging or integration step, decide whether the current untracked docs and modified launch/readme files are meant to be preserved as the official handoff state.
