# The Last Rite: Browser Prototype

Browser-first vertical-slice scaffold for **The Last Rite: Final Inspection**.

## Direction

- **Browser-first** is for design validation
- **Unreal remains the shipping path**
- The browser build exists to prove the inspection, diagnosis, contamination, and ritual loop before the team pays the full UE production cost

## What is in this workspace

- A browser prototype scaffold built around:
  - portable JSON content files in `DesignData/`
  - shared simulation logic in `src/prototype/shared/simulation/`
  - a first-person Three.js scene in `src/prototype/render/`
  - DOM HUD and case-board UI in `src/prototype/ui/`
  - a no-dependency Node static server in `scripts/static-server.mjs`
- A mission flow that covers:
  - safehouse board
  - case start
  - target inspection
  - room clue collection
  - demon diagnosis
  - ordered ritual anchors
  - contamination failure
  - debrief overlay

## Running it

1. Open a terminal in this workspace.
2. Run:

```powershell
node .\scripts\static-server.mjs
```

3. Open:

```text
http://127.0.0.1:4173
```

## Architecture

- `DesignData/`
  Portable content definitions intended to survive into Unreal as data assets or imported data tables.
- `src/prototype/shared/simulation/`
  Engine-agnostic mission generation and state transitions.
- `src/prototype/render/`
  Browser-only scene, camera, and interactable presentation.
- `src/prototype/ui/`
  HUD, board, overlay, and diagnosis interactions.
- `scripts/`
  Local validation and serving helpers.

## Important limitation

This is a **browser validation scaffold**, not the final runtime architecture. What should survive into Unreal:

- demon taxonomy
- case structure
- evidence rules
- ritual sequencing
- contamination pacing
- visual direction

What should not be expected to survive unchanged:

- Three.js scene code
- browser pointer-lock controls
- DOM UI implementation
- local static-server workflow

## Next useful step

The right next implementation pass is to harden the browser slice with one stronger mission map, better interactions, and then add a real authoritative multiplayer server once the solo loop feels correct.

