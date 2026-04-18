export function createHud({ missionStore, cases, demons }) {
  const phaseChip = document.getElementById("phase-chip");
  const objectiveChip = document.getElementById("objective-chip");
  const contaminationChip = document.getElementById("contamination-chip");
  const promptText = document.getElementById("prompt-text");
  const boardPanel = document.getElementById("board-panel");
  const boardTitle = document.getElementById("board-title");
  const caseHook = document.getElementById("case-hook");
  const evidenceList = document.getElementById("evidence-list");
  const diagnosisButtons = document.getElementById("diagnosis-buttons");
  const ritualSteps = document.getElementById("ritual-steps");
  const boardToggle = document.getElementById("board-toggle");
  const boardClose = document.getElementById("board-close");
  const overlay = document.getElementById("overlay-screen");
  const overlayCopy = document.getElementById("overlay-copy");
  const startButtons = document.getElementById("start-buttons");

  boardToggle?.addEventListener("click", () => {
    const snapshot = missionStore.getSnapshot();
    if (!snapshot.mission || snapshot.ui.overlayVisible) {
      return;
    }

    if (snapshot.ui.boardOpen) {
      missionStore.hideBoard();
    } else {
      missionStore.showBoard();
    }
  });

  boardClose?.addEventListener("click", () => missionStore.hideBoard());

  window.addEventListener("keydown", (event) => {
    if (event.code !== "Tab") {
      return;
    }

    event.preventDefault();
    const snapshot = missionStore.getSnapshot();
    if (!snapshot.mission || snapshot.ui.overlayVisible) {
      return;
    }

    if (snapshot.ui.boardOpen) {
      missionStore.hideBoard();
    } else {
      missionStore.showBoard();
    }
  });

  startButtons.innerHTML = "";
  for (const caseFile of cases) {
    const button = document.createElement("button");
    button.className = "action action--primary";
    button.type = "button";
    button.innerHTML = `${caseFile.displayName}<small>${caseFile.hook}</small>`;
    button.addEventListener("click", () => missionStore.startMission(caseFile.id));
    startButtons.appendChild(button);
  }

  missionStore.subscribe((snapshot) => {
    phaseChip.textContent = snapshot.phase.toUpperCase();
    objectiveChip.textContent = snapshot.statusText || snapshot.objectiveText;
    contaminationChip.textContent = `Contamination ${Math.round(snapshot.contamination)}%`;
    promptText.textContent = snapshot.promptText;

    if (snapshot.ui.boardOpen) {
      boardPanel?.classList.remove("panel--hidden");
      boardPanel?.setAttribute("aria-hidden", "false");
    } else {
      boardPanel?.classList.add("panel--hidden");
      boardPanel?.setAttribute("aria-hidden", "true");
    }

    if (snapshot.ui.overlayVisible) {
      overlay?.classList.remove("overlay--hidden");
    } else {
      overlay?.classList.add("overlay--hidden");
    }

    if (!snapshot.mission) {
      boardTitle.textContent = "No active case";
      caseHook.textContent = "Choose a mission from the safehouse board.";
      evidenceList.innerHTML = "";
      diagnosisButtons.innerHTML = "";
      ritualSteps.innerHTML = "";
      return;
    }

    boardTitle.textContent = snapshot.mission.caseFile.displayName;
    caseHook.textContent = snapshot.result
      ? snapshot.result.succeeded
        ? `${snapshot.result.caseName} is contained. Final contamination ${snapshot.result.contamination}%.`
        : `${snapshot.result.caseName} collapsed. The true demon was ${snapshot.result.demonName}.`
      : snapshot.mission.caseFile.hook;

    if (overlayCopy) {
      overlayCopy.textContent = snapshot.result
        ? snapshot.result.succeeded
          ? `${snapshot.result.caseName} is contained. Diagnosis: ${snapshot.result.demonName}. Final contamination ${snapshot.result.contamination}%.`
          : `${snapshot.result.caseName} failed. The true demon was ${snapshot.result.demonName}. Final contamination ${snapshot.result.contamination}%.`
        : "Investigate the possession, name the demon, and perform the banishment before contamination overruns the site.";
    }

    evidenceList.innerHTML = "";
    if (snapshot.discoveredEvidence.length === 0) {
      const item = document.createElement("li");
      item.textContent = "No evidence logged yet.";
      evidenceList.appendChild(item);
    } else {
      snapshot.discoveredEvidence.forEach((clue) => {
        const item = document.createElement("li");
        item.innerHTML = `
          <span class="evidence-name">${clue.displayName}</span>
          <span class="evidence-meta ${clue.falseLead ? "evidence-warning" : ""}">${clue.summary}</span>
        `;
        evidenceList.appendChild(item);
      });
    }

    diagnosisButtons.innerHTML = "";
    demons.forEach((demon) => {
      const button = document.createElement("button");
      button.className = "action diagnosis-card";
      button.type = "button";
      button.disabled = !["investigation", "diagnosis"].includes(snapshot.phase);
      button.style.borderColor = demon.signatureColor;
      button.innerHTML = `${demon.displayName}<small>${demon.silhouetteCue}</small>`;
      button.addEventListener("click", () => missionStore.submitDiagnosis(demon.id));
      diagnosisButtons.appendChild(button);
    });

    ritualSteps.innerHTML = "";
    const activeStepIndex = snapshot.mission.ritualSteps.findIndex((entry) => !entry.complete);
    snapshot.mission.ritualSteps.forEach((step, index) => {
      const item = document.createElement("li");
      item.textContent = step.prompt;
      item.style.color = step.complete ? "#8de4b8" : index === activeStepIndex ? "#f3efe6" : "#b7b2a8";
      ritualSteps.appendChild(item);
    });
  });
}
