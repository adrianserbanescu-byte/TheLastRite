# Local Roadmap Milestones

Last updated: 2026-04-22
Milestone owner: long-range local roadmap
Primary consumer: `chat2`

Use this file to translate the long-range concept into short-range queues without losing the updated product frame:

- one apartment
- three demons
- 4-hand ritual
- possession cascade
- solo and 2-4 player co-op

Current live roadmap position:

- `SD-VS-1` is accepted
- the project is now in `Game Prototype`
- the first prototype proof gates are now accepted:
  - `GP-S-P1` has accepted deterministic shell traversal proof
  - `GP-C1` has accepted deterministic ritual-flow proof
  - the first shell-art handoff package `GP-A-P3` is accepted and closed
- the active short-range packages are now:
  - `GP-C2` parked behind `EXD-UHTG1` on the fresh compile-side rename failure

## Vertical Slice

### VS-1 Runtime Truth And Slice Baseline

- objective: close ambiguity around the current playable slice and protect Apartment 302 / Hollow Saint as the baseline for all later work
- gameplay scope: keep the current clue -> ritual-choice -> outcome loop runnable and understandable
- art/content scope: no new content beyond small readability and presentation support needed to stabilize the slice
- technical scope: finish runtime-path confirmation, explicit fallback-path documentation, and local launch-helper alignment; do not require active package-lane recovery if package work is explicitly parked behind a later named stage gate
- verification or quality gate: one accepted current runtime path plus one precise package blocker statement, or one explicit current-stage cancellation/non-active decision if packaging is not being worked right now
- likely owner lane: `chat3 code`

### VS-2 Live Validation Pass

- objective: verify the slice in motion and ensure the room reads as tense, fair, and legible instead of only textually correct on paper
- gameplay scope: live runs across onboarding, clue chain, bait temptation, ritual choice, fail path, and win path; fix only bounded slice-critical issues
- art/content scope: small support changes only if they directly improve in-motion read or tone clarity
- technical scope: finish the live sweep and one or two bounded fixes without widening into broader systems; if an older live-replay milestone is canceled, any renewed live-validation need must be reopened as a new explicit milestone rather than revived implicitly
- verification or quality gate: at least one validated fail run and one validated win run using the accepted runtime path
- likely owner lane: `chat3 code`

### VS-3 Scale-Up Contract

- objective: define how the current slice scales into three-demon variance, the 4-hand ritual, and co-op state without rewriting product logic from scratch
- gameplay scope: document clue ownership, shared progress, wrong-anchor consequences, team fail visibility, and ritual-readiness semantics
- art/content scope: identify which current visuals already support later multi-demon and multiplayer readability and which need reinforcement
- technical scope: lock the slice semantics that later ritual, networking, and possession work must preserve
- verification or quality gate: one written baseline contract exists and is consistent with current slice behavior
- likely owner lane: `shared`

### VS-4 Vertical Slice Acceptance

- objective: accept Apartment 302 / Hollow Saint as the finished first production target and move into actual Early Access concept prototyping from a stable base
- gameplay scope: confirm the slice stands on its own as a readable solo baseline
- art/content scope: confirm the current Hollow Saint visual language is stable enough to carry into the broader apartment package
- technical scope: publish the post-slice prototype entry point without rewriting short-range worker queues inside this file, and carry forward any intentionally parked package-lane work as a named later-stage requirement instead of a silent unresolved debt
- verification or quality gate: `chat2` can point to one accepted slice baseline and one clear next-stage prototype path
- likely owner lane: `chat2 planning`

## Game Prototype

### GP-1 Session Shell And Tool Grab

- objective: turn the slice into a real session product instead of a standalone room load
- gameplay scope: add lobby, tool grab, briefing, debrief, and replay flow around Apartment 302
- art/content scope: create only the shell content needed to make the session feel readable, intentional, and commercially legible at a stylized co-op-sim level
- technical scope: session-state flow, tool-assignment flow, case result handoff, and replay/requeue support
- verification or quality gate: one front-to-back session loop can be run without manual dev intervention, and its shell states read clearly in screenshots and moment-to-moment play
- likely owner lane: `shared`
- current activation note: the first shell-session proof gate is now accepted through deterministic traversal evidence. The next safe shell-facing work is a later debrief/replay package, not another proof-path blocker loop on the already accepted start-to-case-launch flow.

### GP-2 Three-Demon Apartment Variance

- objective: make Apartment 302 support Hollow Saint, Weeping Matron, and The Appetite in prototype form
- gameplay scope: implement demon identity selection, truth-zone inversion, bait-zone inversion, anchor identity, and per-demon tell logic
- art/content scope: create only the minimum demon-differentiation support needed for readable prototype runs, with strong silhouette, color, and zone-read separation between demons
- technical scope: data/config and case logic support for multi-demon use of one fixed apartment
- verification or quality gate: repeat sessions can swap demons without rebuilding the map or breaking case logic
- likely owner lane: `shared`

### GP-3 Four-Hand Ritual Prototype

- objective: make the ritual into the real centerpiece of the game instead of a simple final click choice
- gameplay scope: prototype Reader, Anchor, Censer, and Guard roles with real phase sequencing and visible team pressure
- art/content scope: readable ritual tools, phase indicators, and circle-state support only at prototype quality, but already aiming at bold co-op-sim readability rather than tiny horror-detail read
- technical scope: ritual phase state, role assignment, role-specific success/failure handling, and solo placeholder adaptation
- verification or quality gate: a player or team can complete a prototype ritual with visible cause-and-effect for mistakes
- likely owner lane: `chat3 code`
- current activation note: the first ritual proof gate is now accepted through deterministic runtime markers. The next short-range gameplay gate is `GP-C2`, which should build on the accepted ritual/session baseline rather than re-open old proof-path churn.

### GP-4 Possession Cascade Prototype

- objective: prototype the wrong-anchor failure that flips the session into a social suspicion state
- gameplay scope: wrong-anchor inversion, one-player possession selection, altered objectives, and partial-salvage fail state
- art/content scope: readable inversion state, emergency contrast, and suspicion-supporting feedback
- technical scope: fail-state transition, player-state flip, team objective update, and debrief payload support
- verification or quality gate: a wrong-anchor run can trigger and complete the possession-cascade prototype without silent failure
- likely owner lane: `shared`

### GP-5 Co-op Authority And Session Viability

- objective: make the apartment case loop actually viable for 2-4 players
- gameplay scope: synchronize inspectables, clue progress, ritual readiness, ritual phases, and outcomes across players
- art/content scope: no broad new art targets beyond multiplayer readability support
- technical scope: host/client replication and authority rules for case-critical and ritual-critical systems
- verification or quality gate: 2-4 players can complete a prototype session without desync breaking the case outcome
- likely owner lane: `chat3 code`

### GP-6 Prototype Acceptance

- objective: declare the game a real party-game prototype, not just a solid solo room
- gameplay scope: confirm solo and co-op sessions both work inside the same apartment package
- art/content scope: identify what content is production-carry-forward versus placeholder
- technical scope: lock the prototype baseline that Alpha will finish rather than rebuild
- verification or quality gate: `chat2` can plan Alpha from one accepted apartment-based prototype instead of disconnected experiments
- likely owner lane: `chat2 planning`

## Alpha

### AL-1 Early Access Scope Lock

- objective: lock the exact first commercial package and cut the rest early
- gameplay scope: lock the final Apartment 302, three-demon, 4-hand ritual, possession-cascade, and session-loop bar while keeping solo plus 2-4 player support mandatory
- art/content scope: lock what is required for the one-apartment launch package and explicitly cut any second-location assumption
- technical scope: lock which launch systems are mandatory beyond the already-mandatory concept frame, and which extras are deferred
- verification or quality gate: one written Early Access scope lock exists and is reflected in downstream planning
- likely owner lane: `chat2 planning`

### AL-2 Apartment 302 Production Finish

- objective: bring the flagship apartment to final Early Access quality across all three demons
- gameplay scope: final pacing, clue readability, demon differentiation, bait fairness, ritual stakes, fail/win teaching, and replay tuning
- art/content scope: final dressing, final emphasis, and per-demon readability support at a commercial stylized co-op-sim bar, with clean materials, bold props, readable evidence clusters, and screenshot-friendly composition
- technical scope: remove prototype shortcuts that would undermine session quality
- verification or quality gate: Apartment 302 is stable, readable, replayable, commercially presentable in both solo and co-op, and no longer depends on grim-horror mud to carry atmosphere
- likely owner lane: `shared`

### AL-3 Ritual, Voice, And Possession Completion

- objective: finish the systems that make the product feel like The Last Rite instead of generic co-op horror
- gameplay scope: complete the 4-hand ritual, possession cascade, and ritual-failure readability
- art/content scope: final ritual presentation, inversion state, and signature beat support
- technical scope: complete voice-recognition/voice-linked Reader behavior, voice-chat-linked possession support as approved, and ritual phase stability
- verification or quality gate: the signature ritual and wrong-choice failure both work end-to-end in the launch package
- likely owner lane: `shared`

### AL-4 Session Systems Completion

- objective: complete the player-facing session systems Early Access actually needs
- gameplay scope: start flow, tool grab, ready flow, result flow, replay flow, restart flow, and approved scoring/debrief structure
- art/content scope: shell UI, report/debrief presentation, and session-state feedback assets at a readable commercial sim bar, with strong icon/state clarity
- technical scope: settings baseline, host/client flow hardening, re-entry behavior, and any approved persistence decisions
- verification or quality gate: no required session system remains placeholder-only
- likely owner lane: `chat3 code`

### AL-5 Alpha Integration Gate

- objective: reach content-complete status for the actual one-apartment Early Access package
- gameplay scope: the entire approved launch scope can be played end-to-end
- art/content scope: all required content exists, even if still needing finish
- technical scope: all required systems are implemented and integrated
- verification or quality gate: Alpha build is feature-complete for the locked scope
- likely owner lane: `shared`

## Beta

### BE-1 Network Stability And Session Reliability

- objective: make the co-op build dependable enough for outside players
- gameplay scope: no new features except fixes for severe blockers
- art/content scope: no broad content expansion beyond fixes
- technical scope: desync fixes, host/client ritual recovery, session startup reliability, reconnect or graceful-failure handling as approved, and repeat-session stability
- verification or quality gate: repeated local co-op sessions complete without core-state failure
- likely owner lane: `chat3 code`

### BE-2 Readability, Comedy, And Spectator Polish

- objective: make the game easier to understand, funnier to fail at, and better to watch
- gameplay scope: tune callouts, blame clarity, ritual payoff, possession reveal, fail spectacle, and debrief readability
- art/content scope: lighting, FX, UI, sound, and composition polish that improve multiplayer readability and absurdist identity while sustaining a stylized commercial storefront/screenshot bar
- technical scope: targeted presentation support only; no feature-sprawl
- verification or quality gate: the first 10-20 minutes communicate the hook clearly in both solo and group play, and still read from clips and static screenshots
- likely owner lane: `shared`

### BE-3 Performance, Packaging, And Release Readiness

- objective: make the build packageable, performant, and supportable as an Early Access product
- gameplay scope: bug-fix only
- art/content scope: optimize or trim only what is needed to hit the release bar
- technical scope: packaged-build repeatability, install/launch checks, restart behavior, settings persistence, performance work, and any packaging work that must be explicitly reopened for release readiness after the current stage
- verification or quality gate: repeatable packaged smoke runs and acceptable performance on validation hardware
- likely owner lane: `chat3 code`

### BE-4 Beta Freeze And Release Triage

- objective: freeze scope and separate launch blockers from post-launch work
- gameplay scope: lock the approved one-apartment Early Access package
- art/content scope: lock required launch content except bug-fix edits
- technical scope: classify open issues into release blockers, day-one fixes, and deferred backlog
- verification or quality gate: `chat2` signs off on the freeze and the blocker list is short and explicit
- likely owner lane: `chat2 planning`

## Early Access Release

### EA-1 Release Candidate Build

- objective: produce the first build that honestly represents the concept in public
- gameplay scope: solo and 2-4 player sessions meet the approved launch bar across all three demons
- art/content scope: all required text, media, and in-build presentation content are present
- technical scope: release candidate packaging, release notes, known-issues list, and launch checklist
- verification or quality gate: full install, launch, session, and replay smoke pass succeeds on the release candidate
- likely owner lane: `shared`

### EA-2 Launch Package And Store Readiness

- objective: finish the local deliverables needed to support the first commercial release
- gameplay scope: bug-fix only
- art/content scope: final key art, screenshots, clips, and launch-facing presentation assets that honestly reflect the in-game stylized readability bar
- technical scope: final package naming, archives, changelog, versioning, and support checklist
- verification or quality gate: one locally archived launch package and one clear release-support checklist exist
- likely owner lane: `shared`
