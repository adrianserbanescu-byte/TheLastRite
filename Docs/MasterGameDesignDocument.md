# The Last Rite

## Master Game Design Document

Last updated: 2026-04-19

## 1. Product Summary

**The Last Rite** is a first-person occult investigation game built in Unreal Engine. The player enters a contained haunted space, inspects people and objects, separates true evidence from theatrical bait, identifies the correct ritual solution, and closes the case before the scene collapses.

The current shipping target is **not** a large multi-case game. The active target is a **single polished vertical slice** built around one case: **Apartment 302 / Hollow Saint**.

This document is the source of truth for the current playable design direction. Browser prototype material and JSON content files remain useful reference and migration inputs, but the Unreal vertical slice is the active product target.

## 2. Vision

The game should feel like:

- a readable first-person investigation
- a short horror case with clear escalation
- a deduction game where the player learns the room instead of solving a menu puzzle
- a ritual game where success comes from understanding the scene, not random guessing

The intended experience is:

1. Enter a cursed scene.
2. Learn how to read it.
3. Collect and reject evidence.
4. Commit to a ritual interpretation.
5. Either seal the case cleanly or fail because the room fooled you.

## 3. Design Pillars

### 3.1 Readability first

The player must always be able to tell:

- where to go next
- what can be inspected
- what progress has been made
- whether the room is pointing toward a conclusion

Horror should not come from hiding the game state. The room can be tense, strange, and hostile, but it must stay legible.

### 3.2 Investigation over scavenging

The player is not searching for random loot. Each inspectable exists to answer one of two questions:

- is this real evidence?
- is this staged bait?

Every interaction should sharpen the player’s read of the room.

### 3.3 Small spaces, strong pacing

The current game structure favors one compact space with clear sub-zones instead of a large environment with filler traversal.

Each space should naturally break into:

- target/body area
- true-evidence area
- false-lead area
- ritual area
- exit path

### 3.4 Ritual choice must feel earned

The correct solution should become understandable from the clues. The player should feel:

- "I see why this is the right anchor"

not:

- "I picked the glowing thing that looked most important"

### 3.5 Short runs, replayable reading

The current target is a short replayable run rather than a long-form campaign. Even in a tiny slice, the game should support:

- a clean first completion
- a faster second run
- understanding through repetition

## 4. Current Vertical Slice Scope

### 4.1 Included

The current playable slice is one complete micro-case:

- **Case title:** Apartment 302
- **Target demon:** Hollow Saint
- **Setting:** apartment / nursery / mirror-bait scene
- **Perspective:** first-person
- **Platform target:** packaged Windows build
- **Engine target:** Unreal Engine 5.4.x

### 4.2 Not included

This slice explicitly does **not** include:

- a second case
- multiplayer
- inventory systems
- save/load systems
- procedural case generation in Unreal
- generic content pipeline work beyond what protects the vertical slice
- final art/audio production

## 5. Core Player Loop

The current loop is:

1. Spawn into Apartment 302.
2. Read the opening objective.
3. Move through the room and inspect labeled objects with `E`.
4. Build a read of the scene by collecting true clues and discarding false leads.
5. Unlock the ritual decision once enough real evidence has been found.
6. Choose a ritual anchor.
7. Correct anchor:
   - seal holds
   - front door unlocks
   - player exits and closes the case
8. Wrong anchor:
   - immediate fail state
   - no exit step
9. Press `R` to restart.

## 6. Current Controls and Interaction Contract

### 6.1 Controls

- `WASD` move
- Mouse look
- `E` inspect / use
- `R` restart run
- `Esc` quit

### 6.2 Interaction rules

- Interactables are readable at a glance through world labels, silhouette, placement, or color contrast.
- Re-checking an object does **not** create duplicate progress.
- Each clue is counted once.
- False leads are allowed and important, but they must not break progression.

## 7. Current Case Design: Apartment 302

### 7.1 Narrative premise

The player arrives at a nursery-centered occult incident. The visible drama of the room is not trustworthy. Some evidence points toward the real demonic pattern; some exists to misdirect the player.

The target interpretation is:

- the **Hollow Saint** has marked the child-facing space
- the **mirror path is bait**

### 7.2 Spatial reading

Apartment 302 should read as five linked zones:

1. **Body / babysitter area**
   - the player’s first strong clue cluster
   - introduces the target’s state and the room’s tone
2. **Nursery / child-facing evidence area**
   - the true pattern gains weight here
   - the player should increasingly understand this area matters
3. **Mirror bait area**
   - visually tempting
   - should feel plausible but wrong
4. **Ritual area**
   - the final interpretation is tested here
5. **Front door exit path**
   - blocked until the correct rite succeeds

### 7.3 Evidence structure

The active Unreal slice uses a fixed evidence structure:

- **5 true clues**
- **3 false leads**
- **1 correct ritual anchor**
- **1 wrong ritual anchor**

The evidence ramp should work like this:

- **0-1 true clues:** pattern unclear
- **2-3 true clues:** nursery side gains weight
- **4 true clues:** mirror increasingly reads as bait
- **5 true clues:** ritual decision is earned and unlocked

### 7.4 Win / fail structure

**Correct ritual**

- player identifies the child-facing pattern
- seal succeeds
- room calms
- front door becomes the final action
- case closes on exit

**Wrong ritual**

- player trusts the staged bait
- fail state triggers immediately
- ending makes clear why the interpretation was wrong

## 8. Demon Design: Hollow Saint

### 8.1 Identity

The Hollow Saint is defined by:

- false sanctity
- forced symmetry
- devotional imagery twisted into bait
- reversed or corrupted prayer signals

### 8.2 Readability cues

From the repo data and current slice direction, Hollow Saint should consistently read through:

- pale halo / saint imagery
- mirrored bodily marks
- crown / ring / iconography
- child-facing devotional corruption

### 8.3 Ritual fantasy

The fantasy of defeating Hollow Saint is **unmasking false holiness**, not overpowering a monster.

The player wins by understanding:

- what in the room is truly marked
- what is decorative sacrilege
- where the ritual weight actually converges

## 9. UX and HUD Direction

### 9.1 HUD priorities

The HUD must communicate, in order:

1. current objective
2. current progress
3. current read / deduction
4. available interaction prompt
5. evidence notes / checklist

### 9.2 First-minute clarity

Within the first 30-60 seconds, the player should understand:

- this is an investigation
- some objects are important
- the game is tracking notes
- the room can be read
- there is a later ritual choice

If the player spends the first minute merely wandering, the build has failed its onboarding goal.

### 9.3 End-state clarity

The ending must answer:

- did I win or fail?
- why?
- what did the room actually mean?

The final report is not flavor text only. It is part of the learning loop.

## 10. Art Direction for the Vertical Slice

The active visual target is:

- stylized 3D
- readable silhouettes
- exaggerated but clean geometry
- bold occult accent colors
- low clutter
- strong zone contrast

The room must be readable in motion. Important objects should not disappear into grime, darkness, or visual noise.

### 10.1 Current environmental goals

Apartment 302 should feel:

- eerie
- staged
- unstable
- readable

It should **not** feel:

- photoreal
- muddy
- over-detailed
- visually random

## 11. Near-Term Development Goals

The next practical development target is to turn the current micro-case into a stronger finished slice.

### 11.1 Gameplay goals

- clearer onboarding and phased objectives
- stronger evidence readability
- stronger clue-to-ritual reasoning
- more obvious ritual readiness
- stronger success/fail presentation
- more satisfying exit closure

### 11.2 Content goals

- clearer room zoning
- stronger body / nursery / mirror / exit staging
- better visual emphasis on useful objects
- better route guidance without hand-holding arrows

### 11.3 Production goals

- keep `main` runnable
- validate via packaged Windows build
- avoid adding systems that do not directly improve the slice

## 12. Future Expansion Direction

If the current slice proves itself, the broader game can later expand into:

- more demons
- more case templates
- ritual variants
- stronger audiovisual identity
- case board / safehouse framing
- richer evidence combination logic

But those expansions are downstream. They are not the current priority. The current priority is one readable, satisfying, replayable case.

## 13. Success Criteria for the Current Game

The current vertical slice is considered successful when:

- the packaged build launches reliably
- the player understands the first minute without outside explanation
- the player can inspect, deduce, and reach an ending
- the room teaches the difference between true evidence and bait
- the ritual outcome feels caused by player understanding
- the player can describe what happened and why

## 14. Source-of-Truth Notes

- The Unreal vertical slice is the active build target.
- `DesignData/*.json` remains useful for demon/case logic and future migration.
- Browser prototype code is reference material, not the runtime target.
- This document should be updated whenever the core loop, evidence model, or case structure changes materially.
