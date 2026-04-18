export async function loadDesignData() {
  const [demonResponse, caseResponse] = await Promise.all([
    fetch("/DesignData/demon_archetypes.json"),
    fetch("/DesignData/case_templates.json")
  ]);

  if (!demonResponse.ok || !caseResponse.ok) {
    throw new Error("Unable to load browser prototype design data.");
  }

  const [demons, cases] = await Promise.all([
    demonResponse.json(),
    caseResponse.json()
  ]);

  return { demons, cases };
}
