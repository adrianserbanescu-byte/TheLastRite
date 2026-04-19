# Package Validation Note Template

Use this when posting a GitHub-visible package-lane checkpoint or verification note from `codex/content`.

Pair it with:

- `ValidatePackageLane.cmd`
- `SummarizePackagedBuild.cmd -Concise`

## Copy-ready template

```text
package validation note

time: <local time>
branch: codex/content
scope: <package lane / launcher helper / package docs / shipped content>
command: <exact command run>
result: <pass / fail>
concise verdict: <paste output from SummarizePackagedBuild.cmd -Concise or note why unavailable>
logs:
- shipping build log: <path or n/a>
- uat log dir: <path or n/a>
- uat saved dir: <path or n/a>
- stage dir: <path or n/a>

notes:
- <short note>
- <short note>

blockers: <none / exact blocker>
needs coordinator: <yes / no>
```

## Usage rule

If the validation run passed, prefer pasting the concise verdict exactly once instead of paraphrasing it into a weaker summary.

If the validation run failed, keep the exact failing command and the most relevant log path in the note.
