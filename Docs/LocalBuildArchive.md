# Local Build Archive

Last updated: 2026-04-22
Archive owner: `chat7`

Use newest-first entries.

## Entries

```text
time:
2026-04-22 03:27:28 +03:00 -> 2026-04-22 03:34:41 +03:00
source milestone:
GP-C1 / GP-S1
build task:
B17
command run:
exact-path handle/attribute recovery on `Intermediate\Build\SourceFileCache.bin` and its immediate parent, then `cmd /c BuildEditor.cmd`
result:
fail
log path:
C:\dev\TheLastRite\Saved\UBT-TheLastRiteEditor-121512185.log
output path:
none
archive note:
No obvious Unreal/editor toolchain processes were running. The exact-path recovery could not clear the `P` attribute on either `Intermediate\Build\SourceFileCache.bin` or its immediate parent directory because both returned `Access denied`, and the single allowed `BuildEditor.cmd` retry then failed again on the same exact path with exit code `6`.
next likely step:
Wait for chat2 to issue one explicit machine-level or admin-level recovery step for the exact `Intermediate\Build\SourceFileCache.bin` denial before any further `BuildEditor.cmd` retry.

time:
2026-04-22 03:10:27 +03:00 -> 2026-04-22 03:13:29 +03:00
source milestone:
GP-S1
build task:
B14
command run:
exact-path rename attempt on `Intermediate\Build\SourceFileCache.bin`, then `cmd /c BuildEditor.cmd`
result:
fail
log path:
C:\dev\TheLastRite\Saved\UBT-TheLastRiteEditor-796828805.log
output path:
none
archive note:
No obvious Unreal/editor toolchain processes were running, but `SourceFileCache.bin` and its immediate parent still carried the unusual `P` attribute state. The exact-path rename attempt on `Intermediate\Build\SourceFileCache.bin` failed immediately with `UnauthorizedAccessException`, and the single allowed `BuildEditor.cmd` retry then failed on the same exact path with exit code `6`.
next likely step:
Wait for chat2 to issue one explicit machine-level or admin-level recovery step for the exact `Intermediate\Build\SourceFileCache.bin` denial before any further `BuildEditor.cmd` retry.

time:
2026-04-22 01:22:44 +03:00 -> 2026-04-22 01:25:50 +03:00
source milestone:
M9
build task:
B12
command run:
planner-owned exact-path rename/remove recovery for `Binaries\Win64\TheLastRite.pdb`, `Intermediate\Build\Win64\x64\UnrealGame\Development\TheLastRite\TheLastRiteHUD.cpp.dep.json`, `Intermediate\Build\Win64\x64\UnrealGame\Development\TheLastRite\TheLastRiteGameMode.cpp.dep.json`, and `Intermediate\Build\SourceFileCache.bin`, then `cmd /c BuildGame.cmd`
result:
success
log path:
C:\dev\TheLastRite\Saved\UBT-TheLastRiteGame-190953980.log
output path:
C:\dev\TheLastRite\Binaries\Win64\TheLastRite.exe
archive note:
The planner-owned recovery converted the blocked exact paths into a removable state by renaming the four inaccessible files out of their produced-item names, removing the renamed artifacts, and rerunning `BuildGame.cmd` once. UnrealBuildTool then regenerated the missing outputs, compiled `TheLastRiteHUD.cpp` and `TheLastRiteGameMode.cpp`, linked `TheLastRite.exe`, and rewrote `TheLastRite.pdb` successfully.
next likely step:
`M9` can now be accepted as build-verified. The next planner decision is whether `M6` still needs a named manual/playtest reassignment before `SD-VS-1`, or whether that remaining managed blocker can be canceled or parked behind a later gate.

time:
2026-04-22 00:57:13 +03:00 -> 2026-04-22 01:02:47 +03:00
source milestone:
M9
build task:
B11
command run:
exact-path recovery attempt on `Binaries\Win64\TheLastRite.pdb`, `Intermediate\Build\Win64\x64\UnrealGame\Development\TheLastRite\TheLastRiteHUD.cpp.dep.json`, `Intermediate\Build\Win64\x64\UnrealGame\Development\TheLastRite\TheLastRiteGameMode.cpp.dep.json`, and `Intermediate\Build\SourceFileCache.bin`, then `cmd /c BuildGame.cmd`
result:
fail
log path:
C:\dev\TheLastRite\Saved\UBT-TheLastRiteGame-1510131415.log
output path:
none
archive note:
The exact-path recovery could not normalize or delete any of the four targeted files because each one returned `Access is denied`. The single allowed `BuildGame.cmd` retry then failed on the same exact paths, including `TheLastRite.pdb`, both `.dep.json` files, and `SourceFileCache.bin`.
next likely step:
Wait for chat2 to issue one explicit machine-level or admin-level recovery decision for the exact blocked M9 paths before any further build-lane retry.

time:
2026-04-22 00:47:42 +03:00
source milestone:
M7
build task:
B10
command run:
cmd /c BuildGame.cmd
result:
success
log path:
C:\dev\TheLastRite\Saved\UBT-TheLastRiteGame-1223431498.log
output path:
C:\dev\TheLastRite\Binaries\Win64\TheLastRite.exe
archive note:
After normalizing the unusual `P` attribute and deleting only `TheLastRite.deps`, `TheLastRite.uhtmanifest`, and `TheLastRite.uhtpath` under the exact blocked dev-intermediate path, the follow-up build completed successfully and reported the target up to date.
next likely step:
`M7` can now be accepted as build-verified. Any remaining follow-up belongs to the gameplay lane unless `chat2` later reopens package work under `M4`.

time:
2026-04-22 00:36:33 +03:00 -> 2026-04-22 00:38:40 +03:00
source milestone:
M7
build task:
B9
command run:
targeted exact-path recovery attempt for `Intermediate\Build\Win64\TheLastRite\Development\TheLastRite.{deps,uhtmanifest,uhtpath}`, then `cmd /c BuildGame.cmd`
result:
fail
log path:
C:\dev\TheLastRite\Saved\UBT-TheLastRiteGame-1034023342.log
C:\dev\TheLastRite\Saved\UBT-TheLastRiteGame-1040230954.log
output path:
none
archive note:
The explicit B9 recovery pass did not clear the standing manifest-path denial. Both `BuildGame.cmd` retries still failed with the same `UnauthorizedAccessException`, and the exact-path recovery command was blocked before it could mutate the three targeted dev-intermediate files.
next likely step:
Wait for chat2 to issue one explicit environment-level recovery decision before any further build-lane retry on `TheLastRite.uhtmanifest`.

time:
2026-04-22 00:12:52 +03:00 -> 2026-04-22 00:13:27 +03:00
source milestone:
M7
build task:
B8
command run:
cmd /c BuildGame.cmd
result:
fail
log path:
C:\dev\TheLastRite\Saved\UBT-TheLastRiteGame-53764228.log
C:\dev\TheLastRite\Saved\UBT-TheLastRiteGame-545122066.log
output path:
none
archive note:
Main attempt and one narrow retry both failed with the same UnauthorizedAccessException while UnrealBuildTool tried to write `Intermediate\Build\Win64\TheLastRite\Development\TheLastRite.uhtmanifest`.
next likely step:
Wait for chat2 to decide the smallest safe environment-level recovery for the standing `TheLastRite.uhtmanifest` write denial before any further build-lane retry.

time:
2026-04-21 20:50:58 +03:00 -> 2026-04-21 20:51:16 +03:00
source milestone:
M3
build task:
B1
command run:
cmd /c BuildGame.cmd
result:
success
log path:
C:\dev\TheLastRite\Saved\UBT-TheLastRiteGame-3140526200.log
output path:
C:\dev\TheLastRite\Binaries\Win64\TheLastRite.exe
archive note:
Clean chat7-owned catch-up build archive entry recorded for accepted runtime milestone M3.
next likely step:
Wait for chat2 to activate the next build-lane item.
```
