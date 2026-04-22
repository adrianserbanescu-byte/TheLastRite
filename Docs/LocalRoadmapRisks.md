# Local Roadmap Risks

Last updated: 2026-04-22
Risk owner: long-range local roadmap
Primary consumer: `chat2`

This file captures the major product, execution, and business risks for the updated concept:

- one apartment
- three demons
- 4-hand ritual
- possession cascade
- solo and 2-4 player co-op

## Major Risks By Stage

### Vertical Slice

- package-lane truth is still unresolved, but current-stage package recovery is not an active lane and must not quietly re-enter the slice without an explicit reopen decision
- the current slice is still a one-demon solo baseline, so the team can overestimate how well it will scale into the actual EA concept
- the new concept can become aspirational marketing language if the current slice is not validated in motion first
- Apartment 302 still risks reading too serious, too quiet, or too text-dependent for the intended social product

### Game Prototype Transition

- the project has now crossed into `Game Prototype`, so the main risk is no longer slice ambiguity alone but whether the first parallel prototype wave stays disciplined
- the first gameplay, shell, and shell-art lanes can create false progress if they drift outside their split ownership boundaries
- if the first ritual prototype and first shell entry flow do not meet cleanly, the project can regress into coordination churn instead of proving the product jump from slice to session
- now that the first proof gates are accepted, the main coordination risk becomes reopening old proof churn instead of carrying the accepted ritual and shell/session baseline forward into the next bounded packages

### Game Prototype

- three-demon variance may feel thin if zone inversion and evidence-subset changes do not create real deduction pressure
- the 4-hand ritual may be significantly harder to make legible and fun than the current final-click ritual choice
- networking clue state, ritual phase state, and possession state may be materially harder than the current slice suggests
- tool scarcity can create friction that is funny in theory but frustrating in practice if first-session clarity is weak
- the project can regress if `GP-C2` widens beyond its bounded gameplay/session-role problem and starts consuming support or framework ownership prematurely
- the project can also regress if accepted `GP-C1` and `GP-S-P1` proof is treated as permission to skip future bounded acceptance gates on later gameplay or post-case shell packages
- the art direction can fail commercially if the game stays visually closer to muddy indie horror than to a readable stylized co-op-sim storefront bar

### Alpha

- voice recognition or voice-linked ritual behavior may be noisy, brittle, or hard to support across machines
- possession cascade may be funny in theory but unfair in practice if the wrong-anchor cause-and-effect is weak
- asset and finish cost can spike once the team tries to sell three demon identities inside one apartment at commercial quality
- the team can overbuild shell and support features instead of protecting the actual apartment session loop

### Beta

- network stability problems can emerge late and consume the whole stage
- the first session may still be weak even if the feature set is complete, which is dangerous in a refund-heavy market
- performance and packaging issues may remain hidden until repeated packaged group sessions are tested
- creator-readability can remain weak if the game is only evaluated from an internal player perspective
- screenshots and trailers can undersell the game if the apartment remains too dark, too busy, or too materially noisy to communicate the hook fast

### Early Access Release

- Steam saturation means a merely competent build may still disappear
- copycat risk increases if the hook is clear but the shipped product is still shallow
- platform fees and refund pressure punish weak first-session clarity and technical instability
- if the apartment does not hold enough replay value across the three demons, friend groups may churn after one night
- if the co-op ritual and possession systems are not robust, the business brief is not actually being met

## Dependency Chains

### Chain 1: Runtime Truth

- explicit current runtime truth
- one precise package blocker plus one explicit non-active or reopened state for packaging work
- live slice validation
- Vertical Slice acceptance
- controlled handoff into the first Game Prototype wave

If this chain is weak, later ritual and co-op planning is built on uncertain reality.

### Chain 2: Apartment Replayability

- Hollow Saint baseline stays strong
- Weeping Matron and The Appetite are layered onto the same apartment
- evidence subset and manifestation pacing variance work
- session loop supports fast restart and replay

If this chain fails, the team may overreact by trying to add a second location too early.

### Chain 3: Ritual Signature

- 4-hand ritual prototype works
- ritual phases are readable
- solo adaptation is viable
- co-op authority model is viable
- final ritual is stable enough for launch

If this chain fails, the core hook of the game is weakened.

Right now the immediate weak link in this chain is not first-pass proof anymore; it is disciplined carry-forward. The first ritual proof gate is accepted, so the next failure mode is widening follow-on gameplay work before the accepted ritual baseline is turned into repeatable multiplayer-ready progression.

### Chain 4: Failure Signature

- wrong-anchor logic is clear
- circle inversion is readable
- possession cascade works
- teammates can react, salvage, or wipe in a way that feels fair

If this chain fails, the product loses one of its strongest story generators.

### Chain 5: Commercial Readiness

- Alpha integration gate
- network stability
- first-session readability and comedy polish
- packaging/performance readiness
- release candidate build

If this chain is compressed too hard, the game may ship into a hostile market before it is sticky enough to survive.

## Likely Blockers

- inaccessible or corrupted Unreal intermediates that keep blocking package or build truth
- letting a canceled or non-active package problem become invisible debt before Beta release-readiness work begins
- underestimating the work needed to replicate inspectables, clue state, ritual progress, and possession state cleanly
- no clear authority model for host/client ritual resolution
- voice-recognition brittleness or environment sensitivity
- weak first-session onboarding once multiple players act out of order
- overdependence on future maps instead of solving replay value on Apartment 302
- lack of enough dedicated design and coding capacity to solve both party feel and network reliability
- asset and finish burden rising faster than the team can support
- art polish spending going into realism density instead of readability, silhouette, lighting clarity, and storefront presentation

## Scope-Cut Candidates

Cut these first if schedule pressure rises:

- a second full location before Early Access
- broad procedural apartment generation
- large hub or safehouse simulation
- deep progression systems or unlock trees
- more than three launch demons
- more than 4 players
- complex inventory management
- cinematic narrative layers that do not improve the first session
- advanced cosmetic systems
- realism-heavy material passes that do not improve evidence read, ritual read, or screenshot quality

## What Can Be Deferred Until After Early Access

- a second or third major location
- more demons beyond Hollow Saint, Weeping Matron, and The Appetite
- broader campaign framing
- larger progression layers
- console and wider platform expansion
- advanced creator tools
- higher-complexity systemic variance beyond what is needed for apartment replayability

## What Must Exist Before Early Access

- one reliable packaged Windows build path
- one polished flagship location: Apartment 302
- three playable demons sharing that apartment
- one working 4-hand ritual
- one working possession-cascade failure flow
- solo support plus stable 2-4 player sessions
- a clear lobby -> tool grab -> briefing -> play -> debrief -> replay loop
- synchronized clue, ritual, fail, and win state across the session
- first-session onboarding that works for both players and spectators
- enough apartment replay variation that friend groups want more than one run
- enough absurdist identity that the game does not read as generic co-op horror
- acceptable performance and stability in repeated packaged sessions
- one explicit deferred backlog so launch scope stays narrow

## Business-Aware Risk Management Rules

- optimize the first 10-20 minutes harder than the twentieth feature
- do not add features that fail to improve replay value, social readability, or commercial viability
- solve replayability on the flagship apartment before assuming more content will save retention
- treat network stability, ritual readability, and first-session clarity as business-critical
- treat readable stylized presentation as commercial viability work, not optional garnish
- if packaging is inactive in the current stage, keep that inactivity explicit and reopen it deliberately instead of letting it drift back by implication
- cut optional systems early when they threaten the launch package
- use local docs to record scope decisions and blockers so the team does not drift by memory
