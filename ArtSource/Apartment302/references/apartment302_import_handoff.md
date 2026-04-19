# Apartment 302 Import And Room-Dressing Handoff

This note turns the current Apartment 302 source boards into a simple Unreal room-dressing pass for the laptop 2 lane.

## Goal

Keep the room readable from the first entry sweep through ritual choice without touching gameplay-code ownership.

## Placement order

1. Build the nursery clue cluster first.
2. Place the correct ritual anchor in the same visual language.
3. Add false leads only after the true clue read is stable.
4. Tune lighting last so true clues stay readable from entry.

## Priority 1: nursery clue cluster

Use these together as the first readable cluster:

- `../props/cradle_halo_handprints.svg`
- `../props/prayer_cards_crown_circle.svg`
- `../props/baby_monitor_hymn.svg`
- `../props/nursery_sigil_correct_ritual.svg`

Intent:

- the player should read these as one connected family of evidence
- pale circle shapes and ash-white marks should recur across the cluster
- old-gold accents should support the read without turning the room into a bright puzzle marker set

## Priority 2: false-lead spacing

Use these as quieter misreads after the nursery cluster is stable:

- `../props/broken_window_latch_false_lead.svg`
- `../props/pawn_ticket_pouch_false_lead.svg`
- `../props/mirror_circle_wrong_ritual.svg`

Intent:

- keep the window latch and pawn-ticket read physically believable and ordinary
- keep the mirror circle tempting, but colder and less evidence-connected than the nursery anchor
- avoid stacking all false leads in one hot spot; they should pull attention away in different directions

## Priority 3: material pass

Use these to keep the apartment domestic first and ritualized second:

- `../materials/old_apartment_wall.svg`
- `../materials/dark_floor.svg`
- `../materials/dirty_nursery_cloth.svg`
- `../materials/old_paper_prayer_card.svg`
- `../materials/ash_white_handprint.svg`
- `../materials/pale_chalk_ritual_mark.svg`

Intent:

- walls and floor stay cheap and tired, not theatrical
- ash-white handprint and chalk language should feel dry and deliberate
- cloth and paper should support the nursery and debt-story reads without overpowering the clue logic

## Board usage

- `apartment302_room_dressing_board.svg`: overall composition and prop relationships
- `apartment302_interaction_readability_board.svg`: entry sweep order, clue grouping, and ritual-anchor clarity
- `apartment302_lighting_readability_board.svg`: focal control, brightness hierarchy, and false-lead suppression
- `hollow_saint_clue_language_board.svg`: true-clue versus false-lead language guardrails

## Readability checks

Before calling the room-dressing pass usable:

1. From the first player entry angle, the nursery side reads as the main evidence zone.
2. The correct ritual anchor feels connected to the true clues without extra UI help.
3. The mirror trap reads as a possible answer, but not the strongest answer.
4. False leads do not block sightlines to the nursery clue cluster.
5. Lighting keeps the true clues pale and legible instead of muddy or over-contrasted.

## Out of scope

- gameplay prompts or clue logic changes
- ritual selection code
- interaction range tuning
- any edits under `Source/`
