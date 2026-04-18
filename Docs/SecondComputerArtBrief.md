# Second Computer Art Brief

This computer is the Unreal build machine. The second computer should be the art/reference machine.

Do not install Unreal tonight. Do not edit C++ files. Do not touch packaged builds.

## Goal

Create useful art direction and simple source assets for the first playable case:

- Case: Apartment 302
- Victim: missing babysitter
- Demon: Hollow Saint
- Mood: small apartment, wrong quiet, ritual evidence, nursery horror

The art should help the Unreal prototype stop looking like plain cubes.

## Work Rules

- Work only in new folders under `ArtSource/`.
- Use PNG or JPG for images.
- Use Markdown for notes.
- Keep files small enough for GitHub.
- Do not commit generated Unreal folders: `Binaries`, `Intermediate`, `Saved`, `DerivedDataCache`, `Packaged`.
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

Create simple concept images or reference boards for these props:

- `cradle_halo_handprints`
- `prayer_cards_crown_circle`
- `baby_monitor_hymn`
- `broken_window_latch_false_lead`
- `pawn_ticket_pouch_false_lead`
- `nursery_sigil_correct_ritual`
- `mirror_circle_wrong_ritual`

For each prop, include:

- one image
- one short note saying what it is
- one short note saying how the player should read it in-game

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

## The Hollow Saint

The demon should feel like:

- pale
- quiet
- religious without using a real religion directly
- halo imagery
- babysitter/nursery case connection
- beautiful at first glance, wrong after one second

Avoid:

- gore-first design
- random monster teeth
- generic red devil
- copyrighted characters
- real religious symbols copied exactly

## First Push Target

The first useful push from the second computer should contain:

- `ArtSource/Apartment302/README.md`
- at least `3` prop images
- at least `2` material references
- at least `1` UI mood mockup

That is enough for the Unreal computer to pull and use.

## Prompt For The Other Codex

Use this prompt on the second computer:

```text
You are helping with TheLastRite, an Unreal horror investigation prototype.

Do not install Unreal. Do not edit C++.

Pull the latest GitHub repo, create ArtSource/Apartment302, and make an art/reference batch for the first case. Follow Docs/SecondComputerArtBrief.md exactly.

Create small PNG/JPG concept or reference images and Markdown notes for the props, materials, and UI mood. Commit and push your work to GitHub when the first useful batch is ready.
```
