import { createSeededRandom, pickOne } from "./random.js";

function clone(value) {
  return structuredClone(value);
}

export function createMission({ demons, cases, caseId, seed = Date.now() }) {
  const random = createSeededRandom(seed);
  const caseFile = clone(caseId ? cases.find((entry) => entry.id === caseId) ?? cases[0] : pickOne(cases, random));
  const demon = clone(pickOne(demons, random));

  const placedClues = [
    ...caseFile.clueProps.map((clue) => ({ ...clone(clue), falseLead: false })),
    ...caseFile.falseLeads.map((clue) => ({ ...clone(clue), falseLead: true }))
  ];

  const targetClues = [
    ...caseFile.target.inspectClues.map((clue) => ({ ...clone(clue), falseLead: false })),
    ...demon.signatureClues.map((clue) => ({ ...clone(clue), falseLead: false }))
  ];

  return {
    seed,
    caseFile,
    demon,
    placedClues,
    targetClues,
    ritualSteps: demon.ritualRecipe.steps.map((step) => ({ ...clone(step), complete: false })),
    discoveredClueIds: [],
    targetInspected: false,
    diagnosis: null
  };
}

