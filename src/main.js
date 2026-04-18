import { bootstrapPrototype } from "./prototype/bootstrapPrototype.js";

bootstrapPrototype().catch((error) => {
  console.error(error);

  const overlay = document.getElementById("overlay-screen");
  const copy = document.getElementById("overlay-copy");
  overlay?.classList.remove("overlay--hidden");

  if (copy) {
    copy.textContent = `The prototype failed to boot: ${error.message}`;
  }
});

