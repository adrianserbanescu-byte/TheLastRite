# The Last Rite

## Product direction

**The Last Rite** is a first-person exorcism simulator built in Unreal Engine for:

- solo play
- **2-4 player co-op** as the primary mode
- friend-group sessions
- streamer-readable moments
- short replayable haunted runs

The target **Early Access** build is:

- one flagship location: **Apartment 302**
- **three demons** sharing that apartment:
  - Hollow Saint
  - Weeping Matron
  - The Appetite
- a **4-hand ritual** built around Reader, Anchor, Censer, and Guard
- a signature wrong-choice failure state: the **possession cascade**
- lightweight session flow: lobby, briefing, play, debrief, fast replay

The current real build is still a **solo vertical slice** for **Apartment 302 / Hollow Saint**. That slice is the production baseline, not the final product by itself.

## Current Unreal playable slice

Current verified prototype runtime path:

```text
C:\dev\TheLastRite\Binaries\Win64\TheLastRite.exe
```

Packaged build status:

```text
C:\dev\TheLastRite\Packaged\Windows\TheLastRite.exe
```

The packaged path is not the current source of truth right now. It is older than the latest dev executable, and the package lane is blocked under `M4` by stale inaccessible Shipping intermediates. Use the dev executable above as the current prototype runtime path until the package lane is verified again.

Helper-script note:

- `PlayPackagedGame.cmd` still launches the packaged executable only for manual package-lane checks.
- While `M4` remains blocked, do not treat that helper or the packaged executable as the normal slice launch path.
- For current slice validation and normal runtime use, launch `C:\dev\TheLastRite\Binaries\Win64\TheLastRite.exe`.

Current controls in the slice:

- `WASD` moves the player
- Mouse looks around
- `E` inspects the thing under the green crosshair
- `R` restarts the case
- `Esc` quits

Current slice objective:

1. Walk around the apartment room.
2. Inspect evidence and reject bait.
3. Identify the Hollow Saint pattern.
4. Choose the correct ritual anchor.
5. Correct anchor seals the case and unlocks the front door.
6. Wrong anchor fails the case immediately.

Important:

- use `Binaries\Win64\TheLastRite.exe` as the current verified prototype runtime path
- do not treat the packaged build in `Packaged\Windows` as current source of truth while `M4` remains blocked
- to make a packaged build, run `PackageGame.cmd`
- `PlayPackagedGame.cmd` is only for explicit packaged-build checks after or during package-lane investigation; it is not the default launch path while `M4` is blocked
- to compile only the Unreal editor target, run `BuildEditor.cmd`
- to compile the game target, run `BuildGame.cmd`
- if Unreal is installed somewhere other than `C:\Program Files\Epic Games\UE_5.4`, set `UE_ROOT` before running the helper scripts

## Current source-of-truth docs

- [Docs/MasterGameDesignDocument.md](Docs/MasterGameDesignDocument.md)
- [Docs/ArtDirection.md](Docs/ArtDirection.md)
- [Docs/LocalRoadmapPlan.md](Docs/LocalRoadmapPlan.md)
- [Docs/LocalRoadmapMilestones.md](Docs/LocalRoadmapMilestones.md)
- [Docs/LocalRoadmapRisks.md](Docs/LocalRoadmapRisks.md)

## Browser prototype

The browser prototype remains historical design support, not the shipping runtime.

It is still useful for:

- content-schema reference
- ritual and evidence rule reference
- migration inputs for Unreal systems

It is not the product target.

## What should guide the next work

The next useful product direction is:

- finish and verify the current Hollow Saint slice baseline
- prototype the **4-hand ritual** in the existing apartment
- prototype the **possession cascade**
- extend Apartment 302 from one demon to **three-demon variance**
- build toward the actual Early Access package instead of assuming a second location is required
