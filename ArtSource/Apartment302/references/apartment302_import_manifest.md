# Apartment 302 Import Manifest

This manifest turns the current Apartment 302 source batch into a proposed import bundle for a future Unreal room-dressing pass.

Use it with `apartment302_import_handoff.md` when the room moves from reference-only staging into actual content imports.

## Goal

Keep the import pass readable and reversible:

- import the true-clue nursery bundle first
- keep false leads separated by zone and read priority
- use stable labels so screenshots, package notes, and room-dressing feedback refer to the same objects

## Suggested content buckets

These are proposed bucket names only. They are meant to keep the pass organized, not to force gameplay ownership changes.

- `Apartment302/Props/TrueClues`
- `Apartment302/Props/FalseLeads`
- `Apartment302/Props/RitualAnchors`
- `Apartment302/Materials/Support`
- `Apartment302/Reference`

## Bundle 1: nursery true-clue set

Import and stage these first so the main evidence family reads from entry before any false-lead pass:

| Source file | Suggested label | Zone | Read priority | Notes |
| --- | --- | --- | --- | --- |
| `../props/cradle_halo_handprints.svg` | `A302_Clue_CradleHalo` | nursery corner | first read | Keep the halo silhouette readable before the cradle details. |
| `../props/prayer_cards_crown_circle.svg` | `A302_Clue_CardCrown` | nursery shelf or side table | second read | Keep the ring logic visible, but quieter than the cradle halo. |
| `../props/baby_monitor_hymn.svg` | `A302_Clue_BabyMonitor` | nursery side table | second read | Support the clue family without stealing the first silhouette read. |
| `../props/nursery_sigil_correct_ritual.svg` | `A302_Anchor_NurserySigil` | nursery floor | answer read | It should feel connected to the clue cluster before prompts explain it. |

## Bundle 2: false-lead set

Place these only after the nursery bundle already reads as the strongest explanation:

| Source file | Suggested label | Zone | Read priority | Notes |
| --- | --- | --- | --- | --- |
| `../props/broken_window_latch_false_lead.svg` | `A302_FalseLead_WindowLatch` | exterior wall or window line | peripheral | Ordinary panic read only; do not let it dominate entry sightlines. |
| `../props/pawn_ticket_pouch_false_lead.svg` | `A302_FalseLead_PawnPouch` | table, drawer edge, or floor scatter | peripheral | Feels human and believable, but unrelated to the ritual answer. |
| `../props/mirror_circle_wrong_ritual.svg` | `A302_Trap_MirrorCircle` | mirror wall | tempting | Strong enough to tempt, colder and less evidence-linked than the nursery answer. |

## Bundle 3: support-material set

These should make the room feel domestic first and ritualized second:

| Source file | Suggested label | Use | Notes |
| --- | --- | --- | --- |
| `../materials/old_apartment_wall.svg` | `A302_Mat_WallAgedPaint` | wall direction | Keep it cheap and lived-in, not stylized. |
| `../materials/dark_floor.svg` | `A302_Mat_DarkFloor` | floor direction | Hold contrast without becoming glossy. |
| `../materials/dirty_nursery_cloth.svg` | `A302_Mat_NurseryCloth` | cloth direction | Supports the nursery tone around the true-clue zone. |
| `../materials/old_paper_prayer_card.svg` | `A302_Mat_PrayerCardPaper` | paper direction | Yellowed, invented, and non-sacred. |
| `../materials/ash_white_handprint.svg` | `A302_Mat_AshHandprint` | decal or surface mark direction | Dry and powdery, not wet or bloody. |
| `../materials/pale_chalk_ritual_mark.svg` | `A302_Mat_ChalkRitual` | ritual mark direction | Broken pale ring language for the correct anchor. |

## Reference-only assets

These should stay reference-side even if the room-dressing pass moves forward:

| Source file | Suggested label | Use |
| --- | --- | --- |
| `apartment302_room_dressing_board.svg` | `A302_Ref_RoomDressingBoard` | overall composition and relationships |
| `apartment302_interaction_readability_board.svg` | `A302_Ref_ReadabilityBoard` | entry sweep and clue clustering |
| `apartment302_lighting_readability_board.svg` | `A302_Ref_LightingBoard` | focal and contrast control |
| `apartment302_room_dressing_pass_order.svg` | `A302_Ref_PassOrder` | staging sequence guardrail |
| `apartment302_read_distance_board.svg` | `A302_Ref_ReadDistance` | far, mid, and near clue certainty |
| `hollow_saint_clue_language_board.svg` | `A302_Ref_ClueLanguage` | true-clue versus false-lead language |

## Staging checklist

Before calling an import pass coherent:

1. The four nursery true-clue labels are present and readable as one family.
2. The mirror trap is visible from a useful angle, but it is not the strongest evidence cluster.
3. The window latch and pawn pouch pull in different directions instead of stacking into one noisy corner.
4. Any screenshots or package notes use the suggested labels consistently.
5. The pass still stays inside packaging/content ownership with no gameplay-code changes.

## Out of scope

- clue logic or prompt behavior
- interaction range changes
- case progression changes
- gameplay ownership edits under `Source/`
