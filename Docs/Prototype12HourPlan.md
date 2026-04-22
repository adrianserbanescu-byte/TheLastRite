# Prototype Development Focus

Last updated: 2026-04-21
Runtime target: Unreal vertical slice for Apartment 302 / Hollow Saint
Execution model: local-only on this laptop

## Purpose

This file keeps the product-facing work focus. The active operating workflow is now defined by the `LocalDev*` files under `Docs/`.

## Goal

Turn the current slice from a functional prototype into a readable, teachable, replayable case run without expanding scope beyond the single-case Unreal build.

## Guardrails

- keep the local workspace buildable
- stay inside the Unreal vertical slice
- prefer changes that improve first-minute clarity, clue-to-ritual reasoning, ritual outcome clarity, or package reliability
- verify meaningful gameplay checkpoints with `BuildGame.cmd`

## Current work blocks

### Block 1 - Onboarding and room-reading clarity

- tighten the first-minute HUD and prompt flow
- make the opening sweep order obvious without hard-locking exploration
- reduce early ambiguity about what matters in the room

### Block 2 - Evidence-to-ritual reasoning

- strengthen the chain from body to nursery to ritual
- make the child-facing pattern read as the real convergence point
- teach why the mirror path is bait

### Block 3 - Ritual and exit presentation

- sharpen anchor prompts and outcome feedback
- make the win handoff to the front door clean
- make the fail state educational

### Block 4 - Reliability and package verification

- clean up confusing text or interaction noise found during testing
- verify the current build locally
- verify the packaged build and launcher behavior locally

## Active execution order

1. onboarding and early guidance
2. clue-to-ritual reasoning
3. ritual and exit clarity
4. package verification

Track live execution in:

- `Docs/LocalDevPlan.md`
- `Docs/LocalDevQueue.md`
- `Docs/LocalDevStatus.md`
