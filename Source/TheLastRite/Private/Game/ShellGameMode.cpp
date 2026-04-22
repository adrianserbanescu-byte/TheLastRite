#include "Game/ShellGameMode.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/SpectatorPawn.h"
#include "Game/ShellHUD.h"
#include "Game/ShellPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"

namespace
{
    const TCHAR* GetShellFlowStateName(EShellFlowState FlowState)
    {
        switch (FlowState)
        {
        case EShellFlowState::Title:
            return TEXT("Title");
        case EShellFlowState::Lobby:
            return TEXT("Lobby");
        case EShellFlowState::Briefing:
            return TEXT("Briefing");
        case EShellFlowState::ToolGrab:
            return TEXT("ToolGrab");
        case EShellFlowState::CaseLaunch:
            return TEXT("CaseLaunch");
        default:
            return TEXT("Unknown");
        }
    }
}

AShellGameMode::AShellGameMode()
{
    DefaultPawnClass = ASpectatorPawn::StaticClass();
    PlayerControllerClass = AShellPlayerController::StaticClass();
    HUDClass = AShellHUD::StaticClass();
    bStartPlayersAsSpectators = true;

    FlowState = EShellFlowState::Title;
    RefreshShellText();
}

void AShellGameMode::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(
        LogTemp,
        Display,
        TEXT("GP-S-P1 SHELL_MARKER previous=\"None\" current=\"%s\""),
        GetShellFlowStateName(FlowState));

    if (FParse::Param(FCommandLine::Get(), TEXT("AutoVerifyShellFlow")))
    {
        RunAutomatedShellVerification();
    }
}

EShellFlowState AShellGameMode::GetFlowState() const
{
    return FlowState;
}

FText AShellGameMode::GetTitleText() const
{
    return TitleText;
}

FText AShellGameMode::GetSubtitleText() const
{
    return SubtitleText;
}

FText AShellGameMode::GetPrimaryPromptText() const
{
    return PrimaryPromptText;
}

FText AShellGameMode::GetSecondaryPromptText() const
{
    return SecondaryPromptText;
}

FText AShellGameMode::GetStatusText() const
{
    return StatusText;
}

void AShellGameMode::AdvanceFlow()
{
    const EShellFlowState PreviousState = FlowState;

    if (FlowState == EShellFlowState::Title)
    {
        FlowState = EShellFlowState::Lobby;
        RefreshShellText();
    }
    else if (FlowState == EShellFlowState::Lobby)
    {
        FlowState = EShellFlowState::Briefing;
        RefreshShellText();
    }
    else if (FlowState == EShellFlowState::Briefing)
    {
        FlowState = EShellFlowState::ToolGrab;
        RefreshShellText();
    }
    else if (FlowState == EShellFlowState::ToolGrab)
    {
        FlowState = EShellFlowState::CaseLaunch;
        RefreshShellText();
    }
    else
    {
        StartSession();
        return;
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("GP-S-P1 SHELL_MARKER previous=\"%s\" current=\"%s\""),
        GetShellFlowStateName(PreviousState),
        GetShellFlowStateName(FlowState));
}

void AShellGameMode::ReturnToPrevious()
{
    if (FlowState == EShellFlowState::CaseLaunch)
    {
        FlowState = EShellFlowState::ToolGrab;
    }
    else if (FlowState == EShellFlowState::ToolGrab)
    {
        FlowState = EShellFlowState::Briefing;
    }
    else if (FlowState == EShellFlowState::Briefing)
    {
        FlowState = EShellFlowState::Lobby;
    }
    else
    {
        FlowState = EShellFlowState::Title;
    }

    RefreshShellText();
}

void AShellGameMode::StartSession()
{
    UE_LOG(
        LogTemp,
        Display,
        TEXT("GP-S-P1 SHELL_MARKER previous=\"%s\" current=\"Apartment302\" handoff_game_mode=\"TheLastRiteGameMode\""),
        GetShellFlowStateName(FlowState));

    UGameplayStatics::OpenLevel(this, TEXT("Apartment302"), true, TEXT("game=/Script/TheLastRite.TheLastRiteGameMode"));
}

void AShellGameMode::RunAutomatedShellVerification()
{
    UE_LOG(
        LogTemp,
        Display,
        TEXT("GP-S-P1 SHELL_AUTO_VERIFY marker=\"start\" initial_state=\"%s\""),
        GetShellFlowStateName(FlowState));

    while (FlowState != EShellFlowState::CaseLaunch)
    {
        AdvanceFlow();
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("GP-S-P1 SHELL_AUTO_VERIFY marker=\"handoff_ready\" final_shell_state=\"%s\""),
        GetShellFlowStateName(FlowState));

    AdvanceFlow();
}

void AShellGameMode::RefreshShellText()
{
    TitleText = NSLOCTEXT("ShellGameMode", "Title", "THE LAST RITE");

    if (FlowState == EShellFlowState::Title)
    {
        SubtitleText = NSLOCTEXT(
            "ShellGameMode",
            "TitleSubtitle",
            "Prototype shell bring-up. Launch now enters a support-owned shell path instead of dropping straight into the Apartment 302 case runtime.");
        PrimaryPromptText = NSLOCTEXT(
            "ShellGameMode",
            "TitlePrimaryPrompt",
            "Press Enter or Space to continue to the lobby.");
        SecondaryPromptText = NSLOCTEXT(
            "ShellGameMode",
            "TitleSecondaryPrompt",
            "Press Esc to quit.");
        StatusText = NSLOCTEXT(
            "ShellGameMode",
            "TitleStatus",
            "Shell state: title screen. The case runtime is still gated behind an explicit start action.");
        return;
    }

    if (FlowState == EShellFlowState::Lobby)
    {
        SubtitleText = NSLOCTEXT(
            "ShellGameMode",
            "LobbySubtitle",
            "Lobby state: the session path is now broken into briefing, tool confirmation, and explicit case launch steps.");
        PrimaryPromptText = NSLOCTEXT(
            "ShellGameMode",
            "LobbyPrimaryPrompt",
            "Press Enter or Space to continue into the briefing.");
        SecondaryPromptText = NSLOCTEXT(
            "ShellGameMode",
            "LobbySecondaryPrompt",
            "Press Backspace to return to the title screen. Press Esc to quit.");
        StatusText = NSLOCTEXT(
            "ShellGameMode",
            "LobbyStatus",
            "Shell state: lobby ready. The case runtime stays gated until the support-owned briefing and launch steps are completed.");
        return;
    }

    if (FlowState == EShellFlowState::Briefing)
    {
        SubtitleText = NSLOCTEXT(
            "ShellGameMode",
            "BriefingSubtitle",
            "Briefing state: the player is warned that Apartment 302 is the current prototype case and that the shell owns the handoff up to launch.");
        PrimaryPromptText = NSLOCTEXT(
            "ShellGameMode",
            "BriefingPrimaryPrompt",
            "Press Enter or Space to confirm the briefing and continue to tool check.");
        SecondaryPromptText = NSLOCTEXT(
            "ShellGameMode",
            "BriefingSecondaryPrompt",
            "Press Backspace to return to the lobby. Press Esc to quit.");
        StatusText = NSLOCTEXT(
            "ShellGameMode",
            "BriefingStatus",
            "Shell state: briefing. Gameplay has not started yet, and the shell still owns the flow.");
        return;
    }

    if (FlowState == EShellFlowState::ToolGrab)
    {
        SubtitleText = NSLOCTEXT(
            "ShellGameMode",
            "ToolGrabSubtitle",
            "Tool check state: confirm the investigation kit before the shell hands off to the live case runtime.");
        PrimaryPromptText = NSLOCTEXT(
            "ShellGameMode",
            "ToolGrabPrimaryPrompt",
            "Press Enter or Space to lock in the tool check and continue to case launch.");
        SecondaryPromptText = NSLOCTEXT(
            "ShellGameMode",
            "ToolGrabSecondaryPrompt",
            "Press Backspace to return to the briefing. Press Esc to quit.");
        StatusText = NSLOCTEXT(
            "ShellGameMode",
            "ToolGrabStatus",
            "Shell state: tool check. The next step is the final launch gate into Apartment 302.");
        return;
    }

    SubtitleText = NSLOCTEXT(
        "ShellGameMode",
        "CaseLaunchSubtitle",
        "Case launch state: this is the final support-owned confirmation before handoff into the existing Apartment 302 gameplay runtime.");
    PrimaryPromptText = NSLOCTEXT(
        "ShellGameMode",
        "CaseLaunchPrimaryPrompt",
        "Press Enter or Space to launch Apartment 302.");
    SecondaryPromptText = NSLOCTEXT(
        "ShellGameMode",
        "CaseLaunchSecondaryPrompt",
        "Press Backspace to return to tool check. Press Esc to quit.");
    StatusText = NSLOCTEXT(
        "ShellGameMode",
        "CaseLaunchStatus",
        "Shell state: case launch armed. Starting now hands off to the existing Apartment 302 gameplay runtime without changing gameplay-owned files.");
}
