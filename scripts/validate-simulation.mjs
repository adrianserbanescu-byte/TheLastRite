import { readFile } from "node:fs/promises";

import { createMission } from "../src/prototype/shared/simulation/createMission.js";

const [demonsRaw, casesRaw] = await Promise.all([
  readFile(new URL("../DesignData/demon_archetypes.json", import.meta.url), "utf8"),
  readFile(new URL("../DesignData/case_templates.json", import.meta.url), "utf8")
]);

const demons = JSON.parse(demonsRaw);
const cases = JSON.parse(casesRaw);
const mission = createMission({ demons, cases, caseId: cases[0].id, seed: 42 });

console.log(JSON.stringify({
  caseId: mission.caseFile.id,
  demonId: mission.demon.id,
  placedClues: mission.placedClues.length,
  targetClues: mission.targetClues.length,
  ritualSteps: mission.ritualSteps.map((step) => step.anchorId)
}, null, 2));

