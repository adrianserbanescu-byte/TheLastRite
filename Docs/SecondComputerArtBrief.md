# Second Computer Art Brief

This computer is the Unreal build machine. The second computer should be the art/reference machine.

Do not install Unreal tonight. Do not edit C++ files. Do not touch packaged builds.

## What The Game Is

`TheLastRite` is a first-person horror investigation game.

The player is not a soldier. The player is an occult investigator/exorcist who enters a small haunted location, reads evidence, identifies the demon pattern, and performs the correct rite before the haunting gets worse.

The first playable prototype is intentionally tiny:

1. The player starts inside one room.
2. The room has several suspicious objects.
3. Some objects are true supernatural clues.
4. Some objects are false leads.
5. After enough true clues, the player chooses a ritual anchor.
6. Correct anchor means the rite succeeds.
7. Wrong anchor means the demon wins.

This is not an action game right now. It is closer to:

- walk
- look
- inspect
- understand
- choose the correct ritual

## First Case Story

Case name: `Apartment 302`

Setup:

- A babysitter vanished during a locked-room incident.
- The apartment nursery feels wrong, quiet, and staged.
- The surviving person, `Nanny Eliza`, refuses to leave the nursery.
- The player is investigating what kind of demon is attached to the room.

The case should feel like:

- small apartment
- cheap furniture
- nursery corner
- evidence of panic, but not a gore scene
- soft domestic objects made scary by placement and symbols
- quiet, clinical dread

The player should feel:

- "I am reading a room."
- "This object is a clue."
- "This other object is a trap or distraction."
- "I can make a wrong ritual choice."

## First Demon

Demon name: `Hollow Saint`

The Hollow Saint should feel like:

- pale
- quiet
- ritualistic
- beautiful at first glance, wrong after one second
- connected to halo/crown/circle imagery
- connected to nursery objects in this case
- religious in mood, but not copied from real religion

Specific signals:

- color family: pale ash, old gold, sickly candle white
- sound idea: reversed choir, hymn fragments, soft breath through a baby monitor
- shape idea: a shrine-like pose with arms open too wide
- clue idea: circles, halos, crowns, dry symmetrical marks, nursery objects turned ceremonial

Avoid:

- gore-first design
- random monster teeth
- generic red devil
- copyrighted characters
- exact real religious symbols

Use invented symbols. The look can remind the player of ritual/religious language, but it must not copy actual sacred imagery.

## Goal

Create useful art direction and simple source assets for the first playable case:

- Case: Apartment 302
- Victim: missing babysitter
- Demon: Hollow Saint
- Mood: small apartment, wrong quiet, ritual evidence, nursery horror

The art should help the Unreal prototype stop looking like plain cubes.

## Work Rules

- Work only in new folders under `ArtSource/`.
- Use PNG, JPG, SVG, or Markdown.
- Keep files small enough for GitHub.
- Do not commit generated Unreal folders: `Binaries`, `Intermediate`, `Saved`, `DerivedDataCache`, `Packaged`.
- Do not edit C++.
- Commit and push when a batch is useful.

## Folder Layout To Create

```text
ArtSource/
  Apartment302/
    references/
    props/
    materials/
    ui/
    README.md
```

## Asset Batch 1: Prop Concepts

Create simple concept images, reference boards, SVG mockups, or visual notes for these props:

- `cradle_halo_handprints`
- `prayer_cards_crown_circle`
- `baby_monitor_hymn`
- `broken_window_latch_false_lead`
- `pawn_ticket_pouch_false_lead`
- `nursery_sigil_correct_ritual`
- `mirror_circle_wrong_ritual`

For each prop, include:

- one image/mockup
- one short note saying what it is
- one short note saying how the player should read it in-game

Prop meanings:

- `cradle_halo_handprints`: true clue. A crib/cradle with ash-white handprints arranged like a halo. The player should think "Hollow Saint."
- `prayer_cards_crown_circle`: true clue. Old prayer cards or child-safe devotional-looking cards, altered into a crown/ring shape. Do not copy real religious cards exactly.
- `baby_monitor_hymn`: true clue. A baby monitor emitting a reversed hymn or impossible choir sound.
- `broken_window_latch_false_lead`: false lead. It suggests mundane violence or forced entry, not the demon.
- `pawn_ticket_pouch_false_lead`: false lead. Ordinary greed/money trouble, not the demon.
- `nursery_sigil_correct_ritual`: correct ritual spot. Pale chalk, halo language, nursery connection.
- `mirror_circle_wrong_ritual`: wrong ritual spot. Looks occult, but it is a trap/distraction.

## Asset Batch 2: Materials

Create flat texture/reference images for:

- old apartment wall
- dark floor
- pale chalk ritual mark
- ash-white handprint
- dirty nursery cloth
- old paper prayer card

These can be rough. They are for direction first, not final shipping art.

## Asset Batch 3: UI Mood

Create reference images or mockups for:

- objective text style
- clue found notification
- win ending
- fail ending

Keep the UI readable. No tiny text.

## First Push Target

The first useful push from the second computer should contain:

- `ArtSource/Apartment302/README.md`
- at least `3` prop images/mockups
- at least `2` material references
- at least `1` UI mood mockup

That is enough for the Unreal computer to pull and use.

## Prompt For The Other Codex

Use the full prompt that the main Codex gives in chat. This file is the source of truth after the repo is pulled.
