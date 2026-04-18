import { createFirstPersonController } from "./input/createFirstPersonController.js";
import { createWorld } from "./render/createWorld.js";
import { createMissionStore } from "./shared/simulation/createMissionStore.js";
import { loadDesignData } from "./shared/content/loadDesignData.js";
import { createHud } from "./ui/createHud.js";

export async function bootstrapPrototype() {
  const designData = await loadDesignData();
  const viewport = document.getElementById("viewport");

  if (!viewport) {
    throw new Error("Missing viewport container.");
  }

  const missionStore = createMissionStore(designData);
  const world = createWorld({
    mount: viewport,
    missionStore
  });
  const controls = createFirstPersonController({
    camera: world.camera,
    domElement: world.renderer.domElement,
    getBounds: () => missionStore.getSnapshot().mission?.caseFile.bounds ?? {
      minX: -14,
      maxX: 14,
      minZ: -14,
      maxZ: 14
    }
  });

  createHud({
    missionStore,
    cases: designData.cases,
    demons: designData.demons
  });

  let lastTime = performance.now();
  let lastMissionSeed = null;
  const frame = (now) => {
    const dt = Math.min((now - lastTime) / 1000, 0.05);
    lastTime = now;

    controls.update(dt);
    missionStore.tick(dt);
    world.update({
      dt,
      snapshot: missionStore.getSnapshot(),
      playerPosition: controls.position
    });
    world.render();

    requestAnimationFrame(frame);
  };

  missionStore.subscribe((snapshot) => {
    controls.setUiOpen(snapshot.ui.boardOpen || snapshot.ui.overlayVisible);
    if (snapshot.mission && snapshot.mission.seed !== lastMissionSeed) {
      controls.setSpawn(snapshot.mission.caseFile.startPosition);
      lastMissionSeed = snapshot.mission.seed;
    }
    world.syncFromState(snapshot);
  });

  missionStore.showStartOverlay();
  requestAnimationFrame(frame);
}
