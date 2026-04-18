import * as THREE from "three";

const MOVE_SPEED = 7.6;
const LOOK_SPEED = 0.0025;

export function createFirstPersonController({ camera, domElement, getBounds }) {
  const direction = new THREE.Vector3();
  const velocity = new THREE.Vector3();
  const keys = {
    forward: false,
    backward: false,
    left: false,
    right: false
  };

  let yaw = 0;
  let pitch = 0;
  let uiOpen = true;

  camera.position.set(0, 1.68, 9.5);
  syncRotation();

  window.addEventListener("keydown", (event) => {
    switch (event.code) {
      case "KeyW":
        keys.forward = true;
        break;
      case "KeyS":
        keys.backward = true;
        break;
      case "KeyA":
        keys.left = true;
        break;
      case "KeyD":
        keys.right = true;
        break;
      default:
        break;
    }
  });

  window.addEventListener("keyup", (event) => {
    switch (event.code) {
      case "KeyW":
        keys.forward = false;
        break;
      case "KeyS":
        keys.backward = false;
        break;
      case "KeyA":
        keys.left = false;
        break;
      case "KeyD":
        keys.right = false;
        break;
      default:
        break;
    }
  });

  domElement.addEventListener("click", () => {
    if (!uiOpen && document.pointerLockElement !== domElement) {
      domElement.requestPointerLock();
    }
  });

  document.addEventListener("mousemove", (event) => {
    if (document.pointerLockElement !== domElement || uiOpen) {
      return;
    }

    yaw -= event.movementX * LOOK_SPEED;
    pitch -= event.movementY * LOOK_SPEED;
    pitch = Math.max(-1.2, Math.min(1.2, pitch));
    syncRotation();
  });

  function syncRotation() {
    camera.rotation.order = "YXZ";
    camera.rotation.y = yaw;
    camera.rotation.x = pitch;
  }

  function update(dt) {
    velocity.set(0, 0, 0);

    if (!uiOpen && document.pointerLockElement === domElement) {
      if (keys.forward) velocity.z -= 1;
      if (keys.backward) velocity.z += 1;
      if (keys.left) velocity.x -= 1;
      if (keys.right) velocity.x += 1;
    }

    if (velocity.lengthSq() > 0) {
      velocity.normalize().multiplyScalar(MOVE_SPEED * dt);
      direction.set(velocity.x, 0, velocity.z);
      direction.applyAxisAngle(new THREE.Vector3(0, 1, 0), yaw);
      camera.position.add(direction);
    }

    const bounds = getBounds();
    camera.position.x = Math.max(bounds.minX + 1.2, Math.min(bounds.maxX - 1.2, camera.position.x));
    camera.position.z = Math.max(bounds.minZ + 1.2, Math.min(bounds.maxZ - 1.2, camera.position.z));
    camera.position.y = 1.68;
  }

  function setUiOpen(value) {
    uiOpen = value;
    if (uiOpen && document.pointerLockElement === domElement) {
      document.exitPointerLock();
    }
  }

  function setSpawn(position) {
    camera.position.fromArray(position);
    yaw = 0;
    pitch = 0;
    syncRotation();
  }

  return {
    camera,
    position: camera.position,
    update,
    setUiOpen,
    setSpawn
  };
}
