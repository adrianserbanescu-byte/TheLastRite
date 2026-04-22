# The Last Rite

## Master Game Design Document

Last updated: 2026-04-21

## 1. Product Summary

**The Last Rite** is a first-person co-op exorcism sim built in Unreal Engine for:

- solo play
- **2-4 player co-op** as the primary mode
- friend-group sessions
- streamer-readable moments
- short replayable haunted runs

The intended tone is **absurdist occult horror**, but that tone is a systems output, not an authored aesthetic. Players should laugh, panic, argue, and improvise through a haunted scene while the investigation and ritual logic stay sincere. The design does not write jokes. The design writes mechanics that break in funny, physical, retellable ways.

The current real build is still the **Apartment 302 / Hollow Saint** Unreal vertical slice. That slice remains the first serious production target. It is the baseline that proves the readable clue-to-ritual loop before the broader co-op party structure is built around it.

That broader co-op party structure is the **Early Access build target**, not a distant post-launch ambition.

## 2. Product And Business Framing

This product sits inside a specific business frame:

- proven category: co-op horror and party-survival games already have market proof
- differentiation requirement: the game cannot rely on generic horror alone
- organic growth requirement: the game needs friend-group retellability and creator-friendly moments
- risk rule: Early Access should be narrow, strong, and replayable instead of broad and expensive

The key planning rule is:

- the Early Access build itself must already be the absurdist solo-plus-2-4-player party game
- the current solo slice is only the baseline on the way there

That means the product should optimize for:

- clear social interaction
- readable chaos
- funny failures and arguments
- one strong flagship location before broad content expansion
- systems that increase replay value faster than they increase asset cost

## 3. Vision

The game should feel like:

- a readable social exorcism sim
- a friend-group horror comedy generator
- a first-person ritual game where players argue about what the room means
- a short-session game that is fun to replay and fun to watch

The intended session fantasy is:

1. Enter a cursed space with friends, or alone if needed.
2. Read the scene fast.
3. Inspect evidence and obvious bait.
4. Debate the meaning of the room.
5. Commit to a ritual plan under pressure.
6. Either pull off a clean exorcism or collapse into a loud, memorable failure.

## 4. Design Pillars

### 4.1 Comedy is a systems output, not a writer's input

The game does not author jokes. It authors systems that break in physical, legible, retellable ways. Voice chat, tool scarcity, timing windows, and shared failure states generate the comedy. The fiction stays sincere.

If a design decision tries to be funny on its own terms, cut it. If a design decision produces a breakdown that players will retell in their group chat, keep it.

### 4.2 Legible physical failure

Players and spectators should be able to tell:

- what the team is trying to do
- what objects matter
- who is doing what
- why a ritual is succeeding or failing
- what just went wrong, and whose fault it was

Failure has to be physical and visible. No silent invalidations. No blank loss screens. Every failure should read as a chain of cause and effect that the team can replay in conversation afterward.

### 4.3 Social deduction over lonely scavenging

The loop should produce conversation:

- "this clue matters"
- "that is bait"
- "you are reading the room wrong"
- "we are committing to the wrong ritual"

The game should encourage blame, confidence, confusion, and last-second agreement. The team should argue before they commit, and commit before they are ready.

### 4.4 One location, layered variance

The first commercial package is built around one flagship apartment. Replay does not come from adding rooms. Replay comes from layering variance axes on the same room:

- which demon is haunting this run
- which evidence slots are active
- which teammates you are playing with

One apartment, three demons at Early Access, randomized evidence subsets per run. Same walls, different game.

### 4.5 First-session clarity drives virality

The first 10 to 20 minutes has to work because:

- co-op discovery depends on a new player understanding the loop fast
- Steam refund pressure punishes weak first sessions
- stream viewers need to understand the joke and the stakes quickly

If the first session is confusing for bad reasons, the concept will not carry itself.

## 5. Current Production Baseline

The current implemented baseline is still:

- **Case title:** Apartment 302
- **Target demon:** Hollow Saint
- **Perspective:** first-person
- **Engine:** Unreal Engine 5.4.x
- **Platform target:** packaged Windows build
- **Current real mode:** solo playable vertical slice

What this means:

- the repo already contains a functioning clue to ritual to outcome loop
- the current build is still proving readability more than co-op behavior
- the slice is the baseline. Do not throw it out because the business frame widened

## 6. Apartment 302: Fixed Location, Three Demons

Apartment 302 reads as five linked zones:

1. body / babysitter area
2. nursery / child-facing evidence area
3. mirror / bathroom bait area
4. ritual area
5. exit path

At Early Access, three demons share this apartment. Each demon inverts which rooms are truth and which are bait. The team does not know which demon they are facing at the start of the run.

### 6.1 Hollow Saint (baseline)

- **Truth zone:** nursery
- **Bait zone:** mirror
- **Correct anchor:** the child's rattle
- **Tell:** religious iconography defaced with baby teeth marks

### 6.2 Weeping Matron

- **Truth zone:** mirror
- **Bait zone:** nursery
- **Correct anchor:** a broken hand mirror
- **Tell:** every reflective surface shows a figure one second delayed

### 6.3 The Appetite

- **Truth zone:** body / babysitter area
- **Bait zone:** ritual area (a pre-drawn fake circle is on the floor)
- **Correct anchor:** a tooth pulled from under the floorboard
- **Tell:** eaten household objects, chewed picture frames, teeth marks in drywall

### 6.4 Why three demons, not one

Three demons sharing one apartment gives the team a real deduction problem without tripling art cost. The same hallway tells three different stories. The mirror is bait, unless it is not. The nursery is the truth, unless it is not. Players have to read the room instead of pattern-matching the building.

## 7. Current Slice Design: Apartment 302 / Hollow Saint Baseline

This is what is built right now. It is the Hollow Saint case as it exists in the repo, and it is the evidence structure that the rest of the variance model extends.

### 7.1 Current room read

Apartment 302 reads as five linked zones:

1. body / babysitter area
2. nursery / child-facing evidence area
3. mirror bait area
4. ritual area
5. exit path

### 7.2 Current evidence structure (Hollow Saint case)

The active slice uses a fixed structure:

- 5 true clues
- 3 false leads
- 1 correct ritual anchor (the child's rattle)
- 1 wrong ritual anchor (the bathroom mirror shard)

### 7.3 Evidence ramp

The readability target for the current slice:

- 0-1 true clues: pattern unclear
- 2-3 true clues: nursery side gains weight
- 4 true clues: mirror increasingly reads as bait
- 5 true clues: ritual choice is earned

### 7.4 Why this slice still matters

Apartment 302 in its Hollow Saint configuration already proves:

- readable space segmentation
- real clue-vs-bait logic
- ritual consequence
- short-session pacing

Those are the correct foundation for the co-op product. The Weeping Matron and The Appetite cases in Section 6 extend the same evidence-and-ramp structure with their own truth and bait zones. The variance model in Section 8 describes how they share the apartment without invalidating this baseline.

## 8. Variance Model

Three axes of randomness, layered on a fixed apartment.

**Axis 1: Demon identity**. One of three per run. Determines which zone is truth and which anchor is correct. Team must deduce which demon before committing.

**Axis 2: Evidence subset**. Each demon has 8 evidence slots. Each run spawns 6 of the 8. Teams commit to a read with incomplete information. This is the friendslop rule. The team never has the full picture.

**Axis 3: Manifestation pacing**. Light randomization on when and where the demon first appears, first aggresses, and first locks doors. Not a gameplay pillar, but enough to break memorization.

Combined, this produces enough run-to-run differentiation that the apartment stays fresh across 8 to 12 sessions, which is the Early Access review window.

## 9. The Ritual: Detailed Spec

This is the game. Everything upstream of this minute exists to deliver this minute.

### 9.1 What a ritual is

The ritual is a 60 to 90 second synchronized group action performed in the ritual area. It requires **four distinct hands** working in parallel. In solo play, one player rotates through all four with time penalties.

The ritual runs in three phases. The demon escalates across phases. Team mistakes are visible and audible. Voice chat is not optional, it is a game mechanic.

### 9.2 The four hands

**1. The Reader**

- Holds the Holy Book.
- Reads three escalating Latin verses into the microphone.
- Voice recognition listens for key phrases. Stumbles, hesitation, or wrong pronunciation stalls the verse.
- A stalled verse lets the demon close one step of distance.
- Bonus: the Reader's voice is audible to the entire team over voice chat. Fumbling in Latin is the primary comedy generator of the ritual.

**2. The Anchor**

- Places the correct ritual anchor object in the center of the chalk circle on the correct verse beat.
- If the wrong anchor is placed, the circle inverts. See Section 11.
- If the correct anchor is placed late, the binding weakens and the demon gets more windows to attack in Phase 3.

**3. The Censer**

- Swings the censer to purify the four cardinal points of the ritual area, in order.
- Each point requires a 2-second hold.
- Any point skipped or held too short lets the demon cross that side of the room.
- The Censer's route is the most physically demanding of the four hands.

**4. The Guard**

- Holds the crucifix.
- Actively blocks the demon's path to the Reader.
- Can raise the crucifix for a 1-second stagger of the demon, with a 6-second cooldown.
- Cannot do anything else during the ritual. Cannot investigate. Cannot help others. Cannot move the anchor. Only guards.

### 9.3 The three phases

**Phase 1 Summoning (20 seconds)**

- Demon appears at maximum distance.
- Reader starts Latin.
- Censer begins cardinal cleanse.
- Anchor places object on beat.
- Guard watches approach.
- Low pressure. Setup phase.

**Phase 2 Binding (30 seconds)**

- Demon closes to mid-range and actively interferes.
- Environmental effects kick in: flickering lights, tipped furniture, whispered Latin overlapping the Reader.
- Guard becomes critical. Censer completes cardinal cleanse.
- The room gets loud. Voice chat degrades into panic coordination.

**Phase 3 Sealing (15 to 30 seconds)**

- Demon is at close range, throwing environmental attacks.
- All four hands must hold position for the final Latin verse.
- This is where teams fall apart.
- Successful Phase 3 completion is the win state. Ritual seals. Demon dissolves. Lights snap back.

### 9.4 Why this ritual is correct for friendslop

- Four hands means four distinct camera angles. Streamable.
- Voice recognition on Latin means the Reader audibly performs for the group. Clippable.
- The Guard doing nothing but guarding creates a player who is forced to sit still and watch chaos, which is its own friendslop archetype.
- Any of the four hands failing produces a visible, attributable breakdown. Blame is clear.
- The ritual rewards coordination but punishes over-planning. A team that has rehearsed is still going to lose hands to Phase 3 pressure.

### 9.5 Solo adaptation

Solo is strictly harder in coordination, strictly simpler in blame.

- Reader duty plays from a pre-recorded tape loop the solo player starts and maintains.
- Solo player must physically run between Anchor placement, Censer points, and Guard position.
- Verse speed is reduced 25 percent to allow physical movement.
- Demon aggression is reduced to match solo capability.
- Solo is playable and winnable. Solo is not the target fantasy. Group play is.

## 10. Tool Scarcity and Role Formation

There are no character classes. There are no stats. Roles form through object scarcity.

Four tools spawn in the apartment. One of each:

- **Holy Book**: enables the Reader role. Required for the final Latin. Anyone can carry it.
- **Censer**: required to purify cardinal points during the ritual. Also reveals smoke-based bait during investigation.
- **Crucifix**: enables the Guard role. Usable as a one-shot demon stagger outside the ritual.
- **Film Camera**: required to reveal at least 2 of the 8 evidence slots per run. Without it, the team is missing investigation coverage.

Because there is exactly one of each, players self-sort through grabbing. Lobby arguments over who gets which tool are a feature, not a bug. The player who grabs last gets whatever is left.

In 2-player and 3-player teams, hands must double up. This is not balanced. Shorter teams are harder. That is the intended scaling.

## 11. Signature Failure Cascade: The Possession

The team committing to the wrong ritual anchor does not produce a quiet failure. It produces a genre switch.

The moment the wrong anchor hits the circle:

1. The chalk circle turns from white to red. One beat of silence.
2. The apartment lights cut. Only flashlights and candles remain.
3. One player is silently possessed. The game picks the player standing closest to the circle at the moment of placement.
4. The possessed player's screen is unchanged. Nothing visibly tells them they are possessed.
5. The possessed player's voice chat output is pitched down 20 percent to their teammates but sounds normal to them.
6. The possessed player's objective flips to hunting. They keep all their tools.
7. The remaining team has 90 seconds to perform an alternative banishing rite, which requires they identify, contain, and counter-exorcise the possessed teammate.
8. Full wipe if they fail. Partial success and reduced reward if they succeed.

This cascade:

- reuses existing apartment geometry
- reuses existing tools
- flips the social dynamic from coordination to suspicion mid-session
- produces the pitch-shifted voice moment every trailer needs
- forces the possessed player into improv acting, which is the best kind of friendslop
- creates a clean clip every single time it happens

## 12. Three Authored Streamer Moments

These three moments are the thumbnails. Lighting, audio, and animation passes should protect them.

**1. The Mirror Crack**

The first time the Hollow Saint's face appears through a crack in the bathroom mirror. Hand-animated, not procedural. Happens exactly once per Hollow Saint run. It is the first hard confirmation of which demon the team faces.

**2. The Pitch Drop**

The voice pitch shift the moment a teammate gets possessed. No visual tell to the possessed player. Only their friends hear it. This is the moment that justifies voice chat as a mechanic.

**3. The Clean Exorcism**

All four hands holding position, final Latin verse closing, circle sealing in white light, demon dissolving. Full camera stillness for 3 seconds. Silence except the breath of the Reader. This is the moment that gets players to queue a second run.

## 13. Target Session Loop

The target loop for the shipping concept:

1. Lobby and tool grab.
2. Team briefing with case summary.
3. Enter Apartment 302.
4. Split attention across evidence, bait, danger, and ritual prep.
5. Argue over demon identity and anchor choice.
6. Commit to the ritual plan.
7. Perform the 4-hand ritual under pressure.
8. Succeed, fail into possession cascade, or partially salvage.
9. Debrief with scored outcomes.
10. Requeue fast.

Solo mode supports the same loop with reduced verse speed and collapsed roles.

## 14. Co-op Translation Rules

The current slice scales into co-op under these rules:

- clue logic must stay understandable when multiple players inspect different things out of order
- ritual progress must be legible to the whole team, not only the interacting player
- wrong decisions must be funny and teachable, not random and silent
- tool scarcity creates roles. Do not add classes.
- the possession cascade must feel fair. Wrong anchor cause and effect must be visible in hindsight.
- solo uses the same clue and ritual logic, not a different game

## 15. Tone Direction

The target tone, delivered through systems:

- absurdist, by way of physical breakdown
- occult, by way of sincere fictional logic
- playful under pressure, by way of voice chat and tool scarcity
- readable, by way of legible failure cascades
- socially combustible, by way of forced coordination under time pressure

The target tone is not:

- authored comedy
- pure bleak horror
- ultra-serious demonology sim
- muddy realism
- cynical parody with no stakes

The occult logic stays straight. The players make it funny.

## 16. UX And HUD Direction

The HUD communicates, in order:

1. team or player objective
2. current case state and demon hypothesis
3. ritual phase and which hand each player is performing
4. available interaction
5. why the room currently points one way and not another

For the broader product, UX should support:

- shared team understanding of ritual phase
- blame-friendly clarity on which hand failed
- fast restart and replay
- readable debrief outcomes
- post-run scoring broken down by hand performance

## 17. Art Direction Summary

The visual target:

- stylized 3D
- readable silhouettes
- exaggerated but intentional shapes
- low clutter
- strong occult accent colors
- room states that read in motion
- a commercial readability bar closer to a stylized co-op sim than to muddy prestige horror

Added for friendslop readability:

- visually legible cause-and-effect during ritual failures
- clear color shift for circle inversion (white to red)
- stream-readable lighting in both success and possession states
- the Hollow Saint, Weeping Matron, and Appetite must read differently at silhouette distance
- screenshots and clips must communicate the hook without long explanation

## 18. Near-Term Product Priorities

The next practical priorities:

- finish the Apartment 302 slice as a trustworthy baseline with Hollow Saint
- implement the 4-hand ritual spec in the solo slice, with placeholder AI holding 3 hands
- prototype voice recognition on Latin verse and test failure modes
- prototype the possession cascade in the existing slice before scaling to co-op
- add Weeping Matron and The Appetite as variance demons sharing Apartment 302
- define the co-op networking and authority model for ritual sync

## 19. Early Access Target

The realistic Early Access package:

- one polished flagship location: Apartment 302
- three demons sharing that location: Hollow Saint, Weeping Matron, The Appetite
- 4-hand ritual fully implemented
- possession cascade as signature failure
- voice recognition Latin reading
- solo and 2-4 player support
- lightweight session shell: start, briefing, play, debrief, fast replay
- commercial-grade tone, UI, audio, and presentation finish

Not optional for Early Access:

- solo support
- 2-4 player co-op support
- three demons in one apartment
- the 4-hand ritual
- the possession cascade
- voice chat integration with Latin
- a replayable session loop

A second location before Early Access is a scope option, not a default assumption. Launching with one apartment and three demons is the correct bet.

## 20. Explicit Non-Goals Before Early Access

Do not treat these as required before first release:

- big campaign structure
- multiple full locations
- procedural generation of apartments
- large hub simulation
- deep progression trees
- character classes or RPG systems
- broad feature sprawl that does not improve the first session

## 21. Source-Of-Truth Notes

- the Unreal vertical slice remains the active build target
- the browser prototype remains reference material, not the runtime target
- `DesignData/*.json` remains useful for case and demon logic
- the 4-hand ritual spec in Section 9 is now the canonical definition of the ritual
- the possession cascade in Section 11 is now the canonical failure state
- this document updates when the ritual spec, variance model, or Early Access scope changes materially

---

**Next step**: prototype the 4-hand ritual in the existing solo slice with three AI-controlled hands, so the human player can feel what the Reader role actually plays like before voice recognition is wired in.
