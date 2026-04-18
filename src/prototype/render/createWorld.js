import * as THREE from "three";

function createHumanTarget(color) {
  const group = new THREE.Group();
  const material = new THREE.MeshStandardMaterial({ color });

  const body = new THREE.Mesh(new THREE.CapsuleGeometry(0.45, 1.15, 6, 10), material);
  body.position.y = 1.45;
  group.add(body);

  const head = new THREE.Mesh(new THREE.SphereGeometry(0.35, 18, 18), material.clone());
  head.position.y = 2.42;
  head.material.color.offsetHSL(0, -0.03, 0.06);
  group.add(head);

  return group;
}

function createObjectTarget(color) {
  const group = new THREE.Group();
  const pedestal = new THREE.Mesh(
    new THREE.CylinderGeometry(0.55, 0.72, 1.2, 12),
    new THREE.MeshStandardMaterial({ color: "#7a6a5a", roughness: 0.82 })
  );
  pedestal.position.y = 0.6;
  group.add(pedestal);

  const object = new THREE.Mesh(
    new THREE.BoxGeometry(0.7, 0.45, 0.55),
    new THREE.MeshStandardMaterial({ color })
  );
  object.position.y = 1.42;
  object.rotation.y = Math.PI / 5;
  group.add(object);

  return group;
}

function buildProp(definition) {
  let geometry;

  switch (definition.shape) {
    case "cylinder":
      geometry = new THREE.CylinderGeometry(definition.radiusTop ?? 0.5, definition.radiusBottom ?? 0.5, definition.height ?? 1, 12);
      break;
    case "sphere":
      geometry = new THREE.SphereGeometry(definition.radius ?? 0.6, 18, 18);
      break;
    default:
      geometry = new THREE.BoxGeometry(...definition.size);
      break;
  }

  const material = new THREE.MeshStandardMaterial({
    color: definition.color,
    roughness: 0.78,
    metalness: 0.12
  });
  const mesh = new THREE.Mesh(geometry, material);

  mesh.position.fromArray(definition.position);
  if (definition.rotationY) {
    mesh.rotation.y = definition.rotationY;
  }

  mesh.castShadow = true;
  mesh.receiveShadow = true;
  return mesh;
}

function createAnchorMesh(color) {
  const group = new THREE.Group();
  const ring = new THREE.Mesh(
    new THREE.TorusGeometry(0.65, 0.11, 10, 20),
    new THREE.MeshStandardMaterial({
      color,
      emissive: color,
      emissiveIntensity: 0.28,
      roughness: 0.42
    })
  );
  ring.rotation.x = Math.PI / 2;
  group.add(ring);

  const stand = new THREE.Mesh(
    new THREE.CylinderGeometry(0.12, 0.16, 0.8, 10),
    new THREE.MeshStandardMaterial({ color: "#65594f", roughness: 0.9 })
  );
  stand.position.y = 0.4;
  group.add(stand);

  return group;
}

export function createWorld({ mount, missionStore }) {
  const scene = new THREE.Scene();
  scene.background = new THREE.Color("#17191b");
  scene.fog = new THREE.Fog("#17191b", 10, 35);

  const camera = new THREE.PerspectiveCamera(68, mount.clientWidth / mount.clientHeight, 0.1, 100);
  const renderer = new THREE.WebGLRenderer({ antialias: true });
  renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
  renderer.setSize(mount.clientWidth, mount.clientHeight);
  renderer.shadowMap.enabled = true;
  mount.appendChild(renderer.domElement);

  const ambient = new THREE.AmbientLight("#fff2d9", 0.8);
  scene.add(ambient);

  const keyLight = new THREE.DirectionalLight("#f8f4e6", 1.4);
  keyLight.position.set(8, 12, 6);
  keyLight.castShadow = true;
  scene.add(keyLight);

  const dangerLight = new THREE.PointLight("#cf6a4f", 0.45, 16);
  dangerLight.position.set(-4, 3.2, -3);
  scene.add(dangerLight);

  const activeMeshes = {
    room: [],
    clues: [],
    anchors: [],
    target: null
  };

  const raycaster = new THREE.Raycaster();
  const center = new THREE.Vector2(0, 0);
  let pulseTime = 0;
  let lastHoveredUuid = null;
  let lastBuiltMissionSeed = null;

  function markInteractable(group, interactableData) {
    group.userData = interactableData;
    group.traverse((child) => {
      child.userData = interactableData;
    });
  }

  function findInteractableRoot(object) {
    let current = object;
    while (current) {
      if (current.userData?.interactableType) {
        return current;
      }
      current = current.parent;
    }

    return null;
  }

  function clearScene() {
    for (const mesh of [...activeMeshes.room, ...activeMeshes.clues, ...activeMeshes.anchors]) {
      scene.remove(mesh);
    }

    if (activeMeshes.target) {
      scene.remove(activeMeshes.target);
    }

    activeMeshes.room = [];
    activeMeshes.clues = [];
    activeMeshes.anchors = [];
    activeMeshes.target = null;
  }

  function buildMission(snapshot) {
    if (!snapshot.mission) {
      return;
    }

    clearScene();

    const { caseFile, demon } = snapshot.mission;
    const width = caseFile.bounds.maxX - caseFile.bounds.minX;
    const depth = caseFile.bounds.maxZ - caseFile.bounds.minZ;
    const centerX = (caseFile.bounds.minX + caseFile.bounds.maxX) / 2;
    const centerZ = (caseFile.bounds.minZ + caseFile.bounds.maxZ) / 2;

    const floor = new THREE.Mesh(
      new THREE.BoxGeometry(width, 0.5, depth),
      new THREE.MeshStandardMaterial({ color: "#6e706d", roughness: 0.92 })
    );
    floor.position.set(centerX, -0.25, centerZ);
    floor.receiveShadow = true;
    scene.add(floor);
    activeMeshes.room.push(floor);

    const wallMaterial = new THREE.MeshStandardMaterial({ color: "#d2d0c9", roughness: 0.95 });
    const walls = [
      { size: [width, 4, 0.45], position: [centerX, 2, caseFile.bounds.minZ] },
      { size: [width, 4, 0.45], position: [centerX, 2, caseFile.bounds.maxZ] },
      { size: [0.45, 4, depth], position: [caseFile.bounds.minX, 2, centerZ] },
      { size: [0.45, 4, depth], position: [caseFile.bounds.maxX, 2, centerZ] }
    ];
    for (const definition of walls) {
      const wall = new THREE.Mesh(new THREE.BoxGeometry(...definition.size), wallMaterial);
      wall.position.fromArray(definition.position);
      wall.receiveShadow = true;
      scene.add(wall);
      activeMeshes.room.push(wall);
    }

    caseFile.roomProps.forEach((prop) => {
      const mesh = buildProp(prop);
      scene.add(mesh);
      activeMeshes.room.push(mesh);
    });

    const targetMesh = caseFile.targetType === "Human"
      ? createHumanTarget(caseFile.target.color)
      : createObjectTarget(caseFile.target.color);
    targetMesh.position.fromArray(caseFile.target.position);
    markInteractable(targetMesh, {
      interactableType: "target",
      prompt: `Inspect ${caseFile.targetLabel} (E)`
    });
    scene.add(targetMesh);
    activeMeshes.target = targetMesh;

    const clueMeshes = [...caseFile.clueProps, ...caseFile.falseLeads].map((clue) => {
      const mesh = buildProp(clue.prop);
      markInteractable(mesh, {
        interactableType: "clue",
        clueId: clue.id,
        prompt: `Inspect ${clue.displayName} (E)`,
        falseLead: clue.falseLead ?? false
      });
      scene.add(mesh);
      return mesh;
    });
    activeMeshes.clues = clueMeshes;

    activeMeshes.anchors = caseFile.anchors.map((anchor) => {
      const mesh = createAnchorMesh(demon.signatureColor);
      mesh.position.fromArray(anchor.position);
      mesh.visible = false;
      markInteractable(mesh, {
        interactableType: "anchor",
        anchorId: anchor.id,
        prompt: `${anchor.label} (E)`
      });
      scene.add(mesh);
      return mesh;
    });

    lastBuiltMissionSeed = snapshot.mission.seed;
  }

  function syncFromState(snapshot) {
    if (snapshot.mission && (!activeMeshes.target || snapshot.mission.seed !== lastBuiltMissionSeed)) {
      buildMission(snapshot);
    }

    if (!snapshot.mission) {
      return;
    }

    const discoveredIds = new Set(snapshot.discoveredEvidence.map((clue) => clue.id));
    const activeAnchorId = snapshot.mission.ritualSteps.find((step) => !step.complete)?.anchorId;
    const ritualActive = snapshot.phase === "ritualPrep" || snapshot.phase === "exorcism";

    if (activeMeshes.target) {
      const targetLit = snapshot.mission.targetInspected;
      activeMeshes.target.traverse((child) => {
        if (child.isMesh) {
          child.material.emissive = new THREE.Color(targetLit ? snapshot.mission.demon.signatureColor : "#000000");
          child.material.emissiveIntensity = targetLit ? 0.12 : 0;
        }
      });
    }

    for (const mesh of activeMeshes.clues) {
      const collected = discoveredIds.has(mesh.userData.clueId);
      mesh.material.emissive = new THREE.Color(collected ? "#ffffff" : "#000000");
      mesh.material.emissiveIntensity = collected ? 0.16 : 0;
      mesh.material.opacity = collected ? 0.65 : 1;
      mesh.material.transparent = collected;
    }

    for (const mesh of activeMeshes.anchors) {
      const isActive = ritualActive;
      mesh.visible = isActive;
      mesh.traverse((child) => {
        if (!child.isMesh) {
          return;
        }

        const isCurrent = mesh.userData.anchorId === activeAnchorId;
        child.material.emissiveIntensity = isCurrent ? 0.5 : 0.18;
        child.material.opacity = isActive ? 1 : 0.25;
        child.material.transparent = true;
      });
    }
  }

  function update({ dt, snapshot }) {
    pulseTime += dt;

    const contaminationFactor = snapshot.contamination / 100;
    scene.fog.color.setStyle(contaminationFactor > 0.65 ? "#2b1618" : "#17191b");
    scene.background.setStyle(contaminationFactor > 0.65 ? "#271517" : "#17191b");
    dangerLight.intensity = 0.35 + contaminationFactor * 1.4;

    if (activeMeshes.target) {
      activeMeshes.target.position.y = snapshot.mission?.caseFile.target.position[1] + Math.sin(pulseTime * 1.7) * 0.02;
    }

    if (snapshot.ui.overlayVisible || snapshot.ui.boardOpen) {
      if (lastHoveredUuid !== null) {
        lastHoveredUuid = null;
        missionStore.setHoveredInteractable(null, "");
      }
      return;
    }

    const hovered = getHoveredInteractable();
    if (hovered) {
      if (hovered.uuid !== lastHoveredUuid) {
        lastHoveredUuid = hovered.uuid;
        missionStore.setHoveredInteractable(hovered.uuid, hovered.userData.prompt);
      }
    } else if (lastHoveredUuid !== null) {
      lastHoveredUuid = null;
      missionStore.setHoveredInteractable(null, "");
    }
  }

  function getHoveredInteractable() {
    raycaster.setFromCamera(center, camera);
    const hits = raycaster.intersectObjects(
      [activeMeshes.target, ...activeMeshes.clues, ...activeMeshes.anchors].filter(Boolean),
      true
    );

    for (const hit of hits) {
      if (hit.distance > 3.2) {
        continue;
      }

      const root = findInteractableRoot(hit.object);
      if (root) {
        return root;
      }
    }

    return null;
  }

  window.addEventListener("keydown", (event) => {
    if (event.code !== "KeyE") {
      return;
    }

    const hovered = getHoveredInteractable();
    if (!hovered) {
      return;
    }

    const type = hovered.userData.interactableType;
    if (type === "target") {
      missionStore.inspectTarget();
    } else if (type === "clue") {
      missionStore.collectClue(hovered.userData.clueId);
    } else if (type === "anchor") {
      missionStore.activateAnchor(hovered.userData.anchorId);
    }
  });

  window.addEventListener("resize", () => {
    camera.aspect = mount.clientWidth / mount.clientHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(mount.clientWidth, mount.clientHeight);
  });

  return {
    camera,
    renderer,
    syncFromState,
    update,
    render: () => renderer.render(scene, camera)
  };
}
