Shared shell-facing UI source-art kit started under `VS-A10`.

# Shared UI Source-Art Notes

This folder contains reusable shell-facing visual language that can later support title, board, briefing, report, and results surfaces.

Current file:

- `sharedui_shell_motif_kit.svg`
  - What it is: one source-art sheet covering palette, label chips, verdict bars, tab treatments, stamps, and typography intent for shell-facing UI.
  - Intended carry-forward: use as the shared UI companion to `ArtSource/ProductShell/references/productshell_preproduction_direction_pack.svg`.

- `sharedui_shell_card_status_components.svg`
  - What it is: one component-focused shell sheet covering title bars, case cards, role chips, verdict strips, and replay actions for `GP-A1`.
  - Intended carry-forward: use as the concrete shared shell companion to the first implementation-carry-forward surface set in `ArtSource/ProductShell/ui/`.

- `sharedui_shell_report_transition_components.svg`
  - What it is: one component-focused shell sheet covering report bars, state banners, progress strips, and retry/replay transition pieces for `GP-A2`.
  - Intended carry-forward: use as the concrete shared shell companion to report and transition surfaces in `ArtSource/ProductShell/ui/`.

- `sharedui_shell_role_ready_components.svg`
  - What it is: one component-focused shell sheet covering tool labels, role chips, ready strips, wait cards, and confirmation banners for `GP-A4`.
  - Intended carry-forward: use as the concrete shared shell companion to tool-grab and role-confirmation surfaces in `ArtSource/ProductShell/ui/`.

- `sharedui_shell_brief_launch_components.svg`
  - What it is: one component-focused shell sheet covering brief lock banners, readiness rows, launch actions, and handoff cards for `GP-A5`.
  - Intended carry-forward: use as the concrete shared shell companion to briefing lock-in and launch-confirmation surfaces in `ArtSource/ProductShell/ui/`.

- `sharedui_shell_archive_replay_components.svg`
  - What it is: one component-focused shell sheet covering archive banners, follow-up strips, replay rows, and next-case cards for `GP-A6`.
  - Intended carry-forward: use as the concrete shared shell companion to archive-routing and replay/requeue surfaces in `ArtSource/ProductShell/ui/`.

- `sharedui_shell_support_recovery_components.svg`
  - What it is: one component-focused shell sheet covering help banners, recovery rows, support actions, and return cards for `GP-A7`.
  - Intended carry-forward: use as the concrete shared shell companion to settings/help and recovery-routing surfaces in `ArtSource/ProductShell/ui/`.

- `sharedui_shell_party_reconnect_components.svg`
  - What it is: one component-focused shell sheet covering member rows, join/pending strips, invite notices, and return cards for `GP-A8`.
  - Intended carry-forward: use as the concrete shared shell companion to party-state and invite/reconnect surfaces in `ArtSource/ProductShell/ui/`.

- `sharedui_shell_permissions_handoff_components.svg`
  - What it is: one component-focused shell sheet covering permission rows, privacy-state tags, host-transfer banners, and confirmation cards for `GP-A9`.
  - Intended carry-forward: use as the concrete shared shell companion to session-permissions and host-handoff surfaces in `ArtSource/ProductShell/ui/`.

- `sharedui_shell_leave_exit_components.svg`
  - What it is: one component-focused shell sheet covering leave banners, return rows, exit cards, and confirmation strips for `GP-A10`.
  - Intended carry-forward: use as the concrete shared shell companion to leave-party and return/exit surfaces in `ArtSource/ProductShell/ui/`.

- `sharedui_shell_disconnect_retry_components.svg`
  - What it is: one component-focused shell sheet covering disconnect banners, service-hold strips, retry-later cards, and reconnect status pieces for `GP-A11`.
  - Intended carry-forward: use as the concrete shared shell companion to disconnect-state and unavailable-service surfaces in `ArtSource/ProductShell/ui/`.

- `sharedui_shell_version_update_components.svg`
  - What it is: one component-focused shell sheet covering mismatch banners, update-required strips, incompatible-session cards, and alignment pieces for `GP-A12`.
  - Intended carry-forward: use as the concrete shared shell companion to version-mismatch and update-required surfaces in `ArtSource/ProductShell/ui/`.

- `sharedui_shell_maintenance_restore_components.svg`
  - What it is: one component-focused shell sheet covering maintenance banners, queue strips, return-window cards, and restoration pieces for `GP-A13`.
  - Intended carry-forward: use as the concrete shared shell companion to maintenance-window and queue/restoration surfaces in `ArtSource/ProductShell/ui/`.

- `sharedui_shell_grouped_handoff_sheet.svg`
  - What it is: one grouped handoff sheet that maps the accepted SharedUI families into one implementation-facing routing view.
  - Intended carry-forward: use during `GP-A-P3` and later shell implementation handoff when the goal is to find the right family sheet quickly without re-reading every micro-task.

- `sharedui_shell_postcase_result_requeue_components.svg`
  - What it is: one grouped post-case component sheet covering result banners, debrief route rows, recap cards, and next-case routing pieces for `GP-A-P4`.
  - Intended carry-forward: use as the concrete shared shell companion to grouped post-case result, debrief, replay, and requeue surfaces in `ArtSource/ProductShell/ui/`.

Boundary:

- This is not a finished HUD.
- This is not a final widget library.
- This is not an Unreal integration task.
- Use it to keep later shell work consistent and bounded.

Quick find:

- baseline shell motif direction:
  - `sharedui_shell_motif_kit.svg`
- `GP-A1` component carry-forward:
  - `sharedui_shell_card_status_components.svg`
- `GP-A2` component carry-forward:
  - `sharedui_shell_report_transition_components.svg`
- `GP-A4` component carry-forward:
  - `sharedui_shell_role_ready_components.svg`
- `GP-A5` component carry-forward:
  - `sharedui_shell_brief_launch_components.svg`
- `GP-A6` component carry-forward:
  - `sharedui_shell_archive_replay_components.svg`
- `GP-A7` component carry-forward:
  - `sharedui_shell_support_recovery_components.svg`
- `GP-A8` component carry-forward:
  - `sharedui_shell_party_reconnect_components.svg`
- `GP-A9` component carry-forward:
  - `sharedui_shell_permissions_handoff_components.svg`
- `GP-A10` component carry-forward:
  - `sharedui_shell_leave_exit_components.svg`
- `GP-A11` component carry-forward:
  - `sharedui_shell_disconnect_retry_components.svg`
- `GP-A12` component carry-forward:
  - `sharedui_shell_version_update_components.svg`
- `GP-A13` component carry-forward:
  - `sharedui_shell_maintenance_restore_components.svg`
- grouped handoff carry-forward:
  - `sharedui_shell_grouped_handoff_sheet.svg`
- `GP-A-P4` component carry-forward:
  - `sharedui_shell_postcase_result_requeue_components.svg`
