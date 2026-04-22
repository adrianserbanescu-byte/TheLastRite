# Local Build Blockers

Last updated: 2026-04-21
Blocker owner: `chat7`

Use newest-first entries.

## Entries

```text
time:
2026-04-22 03:34:41 +03:00
source milestone:
GP-C1 / GP-S1
build task:
B17
blocked step:
One exact-path handle/attribute recovery pass targeted only at `C:\dev\TheLastRite\Intermediate\Build\SourceFileCache.bin` and its immediate parent directory, followed by one `BuildEditor.cmd` retry.
what I tried:
1. Re-read the build-lane docs and confirmed `B17` was the only active task.
2. Inspected `C:\dev\TheLastRite\Intermediate\Build\SourceFileCache.bin`, its immediate parent directory, and the relevant ACL/attribute state. Both paths still showed the unusual `P` attribute.
3. Checked for obvious local Unreal/editor build processes and found no running Unreal/editor toolchain process that explained the denial.
4. Attempted the explicit exact-path recovery by clearing the `P` attribute on only the parent directory and only `SourceFileCache.bin`. Both commands returned `Access denied`, so the path never became writable enough for exact-file removal.
5. Ran the single allowed `cmd /c BuildEditor.cmd` retry. UnrealBuildTool failed again on the same exact `SourceFileCache.bin` path and exited with code `6`.
visible error:
`System.UnauthorizedAccessException: Access to the path 'C:\dev\TheLastRite\Intermediate\Build\SourceFileCache.bin' is denied.`
smallest decision needed from chat2:
Decide the next explicit machine-level or admin-level recovery step for the exact `SourceFileCache.bin` path before reassigning chat7 to retry `BuildEditor.cmd`.

time:
2026-04-22 03:13:29 +03:00
source milestone:
GP-S1
build task:
B14
blocked step:
One explicit environment-recovery pass targeted only at `C:\dev\TheLastRite\Intermediate\Build\SourceFileCache.bin`, followed by one `BuildEditor.cmd` retry.
what I tried:
1. Re-read the build-lane docs and confirmed `B14` was the only active task.
2. Inspected `C:\dev\TheLastRite\Intermediate\Build\SourceFileCache.bin` and its immediate parent. Both still showed the unusual `P` attribute state, while the ACL still granted modify access.
3. Checked for obvious local Unreal/editor toolchain processes and found no running Unreal/editor build processes that explained the denial.
4. Attempted one exact-path recovery by renaming only `C:\dev\TheLastRite\Intermediate\Build\SourceFileCache.bin`. The rename failed immediately with `UnauthorizedAccessException`.
5. Ran the single allowed `cmd /c BuildEditor.cmd` retry. UnrealBuildTool failed again on the same exact `SourceFileCache.bin` path and exited with code `6`.
visible error:
`System.UnauthorizedAccessException: Access to the path 'C:\dev\TheLastRite\Intermediate\Build\SourceFileCache.bin' is denied.`
smallest decision needed from chat2:
Decide the next explicit machine-level or admin-level recovery step for the exact `SourceFileCache.bin` path before reassigning chat7 to retry `BuildEditor.cmd`.

time:
2026-04-22 01:02:47 +03:00
source milestone:
M9
build task:
B11
blocked step:
One explicit environment-recovery pass targeted only at `C:\dev\TheLastRite\Binaries\Win64\TheLastRite.pdb`, `C:\dev\TheLastRite\Intermediate\Build\Win64\x64\UnrealGame\Development\TheLastRite\TheLastRiteHUD.cpp.dep.json`, `C:\dev\TheLastRite\Intermediate\Build\Win64\x64\UnrealGame\Development\TheLastRite\TheLastRiteGameMode.cpp.dep.json`, and `C:\dev\TheLastRite\Intermediate\Build\SourceFileCache.bin`, followed by one `BuildGame.cmd` retry.
what I tried:
1. Re-read the build-lane docs and confirmed `B11` was the only active task.
2. Inspected the four exact blocked paths and confirmed they all still existed with the same unusual attribute state.
3. Ran the one allowed exact-path recovery command to normalize attributes and delete only those four files. Every targeted file returned `Access denied`, so no exact-path cleanup occurred.
4. Ran the single allowed `cmd /c BuildGame.cmd` retry. UnrealBuildTool again failed to delete `Binaries\Win64\TheLastRite.pdb`, the compiler could not open both targeted `.dep.json` files, and UnrealBuildTool then failed on `Intermediate\Build\SourceFileCache.bin`.
visible error:
`Access is denied` on `C:\dev\TheLastRite\Binaries\Win64\TheLastRite.pdb`, `C:\dev\TheLastRite\Intermediate\Build\Win64\x64\UnrealGame\Development\TheLastRite\TheLastRiteHUD.cpp.dep.json`, `C:\dev\TheLastRite\Intermediate\Build\Win64\x64\UnrealGame\Development\TheLastRite\TheLastRiteGameMode.cpp.dep.json`, and `C:\dev\TheLastRite\Intermediate\Build\SourceFileCache.bin`, followed by build exit code `6`.
smallest decision needed from chat2:
Decide the next explicit machine-level or admin-level recovery step for the exact blocked M9 file set before reassigning chat7 to retry `BuildGame.cmd`.

time:
2026-04-22 00:38:40 +03:00
source milestone:
M7
build task:
B9
blocked step:
One explicit environment-recovery pass targeted only at `C:\dev\TheLastRite\Intermediate\Build\Win64\TheLastRite\Development\TheLastRite.uhtmanifest`, followed by one `BuildGame.cmd` retry.
what I tried:
1. Re-read the current build/dev state and confirmed `B9` was the only active build-lane task.
2. Inspected the immediate parent path again and confirmed the exact dev-intermediate artifact set was still only `TheLastRite.deps`, `TheLastRite.uhtmanifest`, and `TheLastRite.uhtpath`.
3. Attempted the allowed exact-path recovery by deleting only those three targeted dev-intermediate files. The recovery command was blocked before any filesystem mutation occurred, so the artifact set stayed in place.
4. Ran the required `BuildGame.cmd` retry and then one intended clean rerun after the blocked recovery command. Both retries failed with the same `UnauthorizedAccessException` on `TheLastRite.uhtmanifest`.
visible error:
`System.UnauthorizedAccessException: Access to the path 'C:\dev\TheLastRite\Intermediate\Build\Win64\TheLastRite\Development\TheLastRite.uhtmanifest' is denied.`
smallest decision needed from chat2:
Decide the next explicit environment-level recovery step for the standing `TheLastRite.uhtmanifest` denial, because the exact-path build-lane recovery pass did not clear it and further retries would loop.

time:
2026-04-22 00:13:27 +03:00
source milestone:
M7
build task:
B8
blocked step:
`BuildGame.cmd` verification while UnrealBuildTool writes `C:\dev\TheLastRite\Intermediate\Build\Win64\TheLastRite\Development\TheLastRite.uhtmanifest`
what I tried:
1. Ran `cmd /c BuildGame.cmd` once and got exit code `6` with `UnauthorizedAccessException` on `TheLastRite.uhtmanifest`.
2. Checked the manifest path attributes and ACL. The file was not read-only, `attrib` reported `A P`, and the ACL still showed write/modify access.
3. Ran one narrow retry with `cmd /c BuildGame.cmd`; the same `UnauthorizedAccessException` reproduced on the same manifest path.
visible error:
`System.UnauthorizedAccessException: Access to the path 'C:\dev\TheLastRite\Intermediate\Build\Win64\TheLastRite\Development\TheLastRite.uhtmanifest' is denied.`
smallest decision needed from chat2:
Decide the smallest safe environment-level recovery for the standing manifest write denial before reassigning chat7 to retry `BuildGame.cmd`.
```
