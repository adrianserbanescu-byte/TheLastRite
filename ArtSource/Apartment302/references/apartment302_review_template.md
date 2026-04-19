# Apartment 302 Review Template

Use this template when posting a GitHub-visible room-dressing or packaged-content review note for Apartment 302.

Keep the labels and shot ids exactly aligned with:

- `apartment302_import_manifest.md`
- `apartment302_screenshot_checklist.md`

## Copy-ready template

```text
Apartment 302 review

pass name: <short pass name>
branch: codex/content
scope: <room dressing / package-ready content / screenshot review>
blockers: <none / exact blocker>
next in-scope task: <one short line or none>

screenshot set:
- A302_Shot_01_EntryWide: <short read result>
- A302_Shot_02_NurseryCluster: <short read result>
- A302_Shot_03_NurseryAnchor: <short read result>
- A302_Shot_04_MirrorTrap: <short read result>
- A302_Shot_05_WindowFalseLead: <short read result>
- A302_Shot_06_PawnFalseLead: <short read result>
- A302_Shot_07_EntryToAnswerLine: <short read result>

label checks:
- nursery clue cluster: <ok / issue>
- correct nursery anchor: <ok / issue>
- mirror trap: <ok / issue>
- window false lead: <ok / issue>
- pawn false lead: <ok / issue>
- entry-to-answer sightline: <ok / issue>

import labels seen:
- A302_Clue_CradleHalo: <seen / missing / unclear>
- A302_Clue_CardCrown: <seen / missing / unclear>
- A302_Clue_BabyMonitor: <seen / missing / unclear>
- A302_Anchor_NurserySigil: <seen / missing / unclear>
- A302_Trap_MirrorCircle: <seen / missing / unclear>
- A302_FalseLead_WindowLatch: <seen / missing / unclear>
- A302_FalseLead_PawnPouch: <seen / missing / unclear>

false-lead balance:
- salience order:
  1. <first read>
  2. <second read>
  3. <third read>
  4. <fourth read>
  5. <fifth read>
- mirror trap still secondary: <yes / no>
- window beat still peripheral: <yes / no>
- pawn beat still discoverable-late: <yes / no>

read verdict:
- first read: <what reads first>
- correct answer still reads: <yes / no>
- mirror trap over-dominant: <yes / no>
- false leads too clustered: <yes / no>

next pass:
- <smallest next in-scope adjustment>
```

## Usage rule

If a review note names a shot or object, prefer the exact shot ids and labels from the checklist and import manifest.

That keeps screenshot feedback, package notes, and future placement passes talking about the same things.

If the note is specifically about salience drift, include the `false-lead balance` block and compare it against `apartment302_false_lead_balance_guide.md`.

## Keep out of scope

Do not use this template to request:

- gameplay prompt changes
- ritual logic changes
- interaction-range tuning
- cross-lane ownership changes
