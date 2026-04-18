import { createMission } from "./createMission.js";

const MAX_CONTAMINATION = 100;

const phaseObjectiveMap = {
  briefing: "Choose a case from the board.",
  intake: "Approach the target and perform the first inspection.",
  investigation: "Sweep the room, collect evidence, and rule out false leads.",
  diagnosis: "Open the case board and lock the demon diagnosis.",
  ritualPrep: "Return to the room and activate the ritual anchors in order.",
  exorcism: "Finish the banishment before the site breaks open.",
  debrief: "Review the mission result and launch the next case.",
  failed: "The site is lost. Reset and try again."
};

export function createMissionStore({ demons, cases }) {
  const listeners = new Set();

  const state = {
    demons,
    cases,
    mission: null,
    phase: "briefing",
    contamination: 0,
    discoveredEvidence: [],
    statusText: "Choose a case from the board.",
    promptText: "Click Start Mission to begin.",
    ui: {
      overlayVisible: true,
      boardOpen: false
    },
    currentInteractableId: null,
    result: null
  };

  function emit() {
    const snapshot = getSnapshot();
    listeners.forEach((listener) => listener(snapshot));
  }

  function setStatus(text) {
    state.statusText = text;
  }

  function setPrompt(text) {
    state.promptText = text;
  }

  function startMission(caseId) {
    const seed = Date.now();
    state.mission = createMission({ demons, cases, caseId, seed });
    state.phase = "intake";
    state.contamination = 10;
    state.discoveredEvidence = [];
    state.ui.overlayVisible = false;
    state.ui.boardOpen = false;
    state.result = null;
    state.currentInteractableId = null;

    setStatus(state.mission.caseFile.objectiveText);
    setPrompt("Click the scene, move with WASD, and inspect the target with E.");
    emit();
  }

  function showStartOverlay(copy) {
    state.ui.overlayVisible = true;
    state.ui.boardOpen = false;
    state.phase = "briefing";
    state.result = null;
    setStatus(copy ?? phaseObjectiveMap.briefing);
    setPrompt("Click Start Mission to launch a case.");
    emit();
  }

  function showBoard() {
    if (!state.mission) {
      return;
    }

    state.ui.boardOpen = true;
    setPrompt("Board open. Review the clues and submit a diagnosis.");
    emit();
  }

  function hideBoard() {
    state.ui.boardOpen = false;
    if (state.phase === "ritualPrep" || state.phase === "exorcism") {
      setPrompt(getCurrentRitualPrompt());
    } else {
      setPrompt("Press E on a highlighted prop to inspect it.");
    }
    emit();
  }

  function inspectTarget() {
    if (!state.mission || (state.phase !== "intake" && state.phase !== "investigation" && state.phase !== "diagnosis")) {
      return;
    }

    if (state.mission.targetInspected) {
      setPrompt("The target is already catalogued. Sweep the room for supporting evidence.");
      emit();
      return;
    }

    state.mission.targetInspected = true;
    state.phase = "investigation";
    addEvidence(state.mission.targetClues);
    setStatus("Target inspected. Cross-check the room clues before you lock the rite.");
    setPrompt("The target yielded signature tells. Search the room and open the Case Board when ready.");
    emit();
  }

  function collectClue(clueId) {
    if (!state.mission || (state.phase !== "investigation" && state.phase !== "diagnosis")) {
      return;
    }

    const clue = state.mission.placedClues.find((entry) => entry.id === clueId);
    if (!clue || state.mission.discoveredClueIds.includes(clueId)) {
      return;
    }

    state.mission.discoveredClueIds.push(clueId);
    addEvidence([clue]);

    if (clue.falseLead) {
      modifyContamination(5);
      setStatus("False lead logged. The room is destabilizing.");
    } else {
      setStatus(`Evidence secured: ${clue.displayName}.`);
    }

    if (state.discoveredEvidence.length >= 4) {
      state.phase = "diagnosis";
      setPrompt("You have enough evidence to lock a diagnosis on the Case Board.");
    }

    emit();
  }

  function addEvidence(clues) {
    for (const clue of clues) {
      if (state.discoveredEvidence.some((entry) => entry.id === clue.id)) {
        continue;
      }

      state.discoveredEvidence.push(clue);
    }
  }

  function submitDiagnosis(demonId) {
    if (!state.mission || !["investigation", "diagnosis"].includes(state.phase)) {
      return;
    }

    state.mission.diagnosis = demonId;
    state.phase = "ritualPrep";
    state.ui.boardOpen = false;

    const correct = demonId === state.mission.demon.id;
    if (!correct) {
      if (!modifyContamination(16)) {
        return;
      }
      setStatus("The rite feels wrong. The site is pushing back.");
    } else {
      setStatus("Diagnosis locked. The anchors are primed.");
    }

    setPrompt(getCurrentRitualPrompt());
    emit();
  }

  function activateAnchor(anchorId) {
    if (!state.mission || !["ritualPrep", "exorcism"].includes(state.phase)) {
      return;
    }

    const currentStep = state.mission.ritualSteps.find((step) => !step.complete);
    if (!currentStep) {
      return;
    }

    if (anchorId !== currentStep.anchorId) {
      if (!modifyContamination(8)) {
        return;
      }
      setStatus("Wrong anchor. The demon lashes out through the room.");
      setPrompt(getCurrentRitualPrompt());
      emit();
      return;
    }

    currentStep.complete = true;
    state.phase = state.mission.ritualSteps.some((step) => !step.complete) ? "exorcism" : "debrief";

    if (state.phase === "debrief") {
      finishMission(state.mission.diagnosis === state.mission.demon.id);
      return;
    }

    setStatus(`Ritual step complete: ${currentStep.prompt}`);
    setPrompt(getCurrentRitualPrompt());
    emit();
  }

  function finishMission(succeeded) {
    const diagnosisCorrect = state.mission?.diagnosis === state.mission?.demon.id;

    state.phase = succeeded ? "debrief" : "failed";
    state.result = {
      succeeded,
      diagnosisCorrect,
      contamination: Math.round(state.contamination),
      demonName: state.mission?.demon.displayName ?? "Unknown",
      caseName: state.mission?.caseFile.displayName ?? "Unknown Case"
    };
    state.ui.overlayVisible = true;
    state.ui.boardOpen = false;
    state.currentInteractableId = null;

    if (succeeded) {
      setStatus("The demon is out. Debrief and queue the next containment run.");
      setPrompt("Mission complete.");
    } else {
      setStatus("Containment failed. Reset the scene and try again.");
      setPrompt("Mission failed.");
    }

    emit();
  }

  function modifyContamination(delta) {
    state.contamination = Math.max(0, Math.min(MAX_CONTAMINATION, state.contamination + delta));
    if (state.contamination >= MAX_CONTAMINATION) {
      finishMission(false);
      return false;
    }

    return true;
  }

  function tick(dt) {
    if (!state.mission || state.ui.overlayVisible) {
      return;
    }

    if (state.phase === "intake" || state.phase === "investigation" || state.phase === "diagnosis") {
      modifyContamination(dt * 1.9);
    } else if (state.phase === "ritualPrep" || state.phase === "exorcism") {
      modifyContamination(dt * 4.5);
    }
  }

  function setHoveredInteractable(interactableId, prompt) {
    state.currentInteractableId = interactableId;
    if (!interactableId) {
      if (state.phase === "ritualPrep" || state.phase === "exorcism") {
        setPrompt(getCurrentRitualPrompt());
      } else if (state.ui.boardOpen) {
        setPrompt("Board open. Review the evidence.");
      } else {
        setPrompt("Press E on a highlighted prop to inspect it.");
      }
      emit();
      return;
    }

    setPrompt(prompt);
    emit();
  }

  function getCurrentRitualPrompt() {
    if (!state.mission) {
      return "No ritual loaded.";
    }

    const step = state.mission.ritualSteps.find((entry) => !entry.complete);
    return step ? `${step.prompt} (E)` : "The rite is complete.";
  }

  function subscribe(listener) {
    listeners.add(listener);
    listener(getSnapshot());
    return () => listeners.delete(listener);
  }

  function getSnapshot() {
    return {
      phase: state.phase,
      contamination: state.contamination,
      discoveredEvidence: [...state.discoveredEvidence],
      mission: state.mission ? structuredClone(state.mission) : null,
      ui: { ...state.ui },
      statusText: state.statusText,
      promptText: state.promptText,
      currentInteractableId: state.currentInteractableId,
      result: state.result ? { ...state.result } : null,
      objectiveText: phaseObjectiveMap[state.phase] ?? state.statusText
    };
  }

  return {
    subscribe,
    getSnapshot,
    showStartOverlay,
    showBoard,
    hideBoard,
    startMission,
    inspectTarget,
    collectClue,
    submitDiagnosis,
    activateAnchor,
    setHoveredInteractable,
    tick
  };
}
