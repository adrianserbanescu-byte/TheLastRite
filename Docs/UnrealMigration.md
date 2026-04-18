# Unreal Migration Notes

## Source of truth to preserve

Keep these data contracts stable between browser and Unreal:

- `demon_archetypes.json`
- `case_templates.json`

These should later become:

- Unreal `PrimaryDataAsset` types
- or imported data tables plus authored Blueprint wrappers

## Browser to Unreal mapping

- `DesignData/*.json` -> UE data assets / tables
- `createMission.js` -> mission generation C++ system
- `createMissionStore.js` -> replicated mission state / game state logic
- `createWorld.js` -> UE level scripting, actor spawning, and visual presentation
- `createHud.js` -> UMG widgets and interaction flows

## Migration rule

Port the **rules and content schema**, not the browser rendering code.

