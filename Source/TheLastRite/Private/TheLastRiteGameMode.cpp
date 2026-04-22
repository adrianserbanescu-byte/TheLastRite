#include "TheLastRiteGameMode.h"

#include "CaseExit.h"
#include "InspectableProp.h"
#include "RitualAnchor.h"
#include "SessionToolPickup.h"
#include "TheLastRiteCharacter.h"
#include "TheLastRiteHUD.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/PointLight.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"

namespace
{
    constexpr int32 RitualHandStepCount = 4;

    const TCHAR* GetSessionToolMarkerName(ETheLastRiteSessionTool ToolType)
    {
        switch (ToolType)
        {
        case ETheLastRiteSessionTool::HolyBook:
            return TEXT("holy_book");
        case ETheLastRiteSessionTool::FilmCamera:
            return TEXT("film_camera");
        case ETheLastRiteSessionTool::Censer:
            return TEXT("censer");
        case ETheLastRiteSessionTool::Crucifix:
            return TEXT("crucifix");
        default:
            return TEXT("unknown");
        }
    }

    bool DoesEvidenceRequireFilmCamera(const FString& EvidenceSummary)
    {
        return EvidenceSummary.Equals(TEXT("the baby monitor - hymn repeating on every channel"), ESearchCase::CaseSensitive)
            || EvidenceSummary.Equals(TEXT("the nursery wallpaper - child's sun turned into a halo"), ESearchCase::CaseSensitive);
    }

    bool TryGetRequiredToolForRitualHand(int32 CompletedSteps, ETheLastRiteSessionTool& OutToolType)
    {
        switch (CompletedSteps)
        {
        case 0:
            OutToolType = ETheLastRiteSessionTool::HolyBook;
            return true;
        case 2:
            OutToolType = ETheLastRiteSessionTool::Censer;
            return true;
        case 3:
            OutToolType = ETheLastRiteSessionTool::Crucifix;
            return true;
        default:
            return false;
        }
    }

    ASessionToolPickup* FindSessionToolPickupByType(const TArray<TObjectPtr<ASessionToolPickup>>& ToolPickups, ETheLastRiteSessionTool ToolType)
    {
        for (ASessionToolPickup* ToolPickup : ToolPickups)
        {
            if (ToolPickup != nullptr && ToolPickup->GetToolType() == ToolType)
            {
                return ToolPickup;
            }
        }

        return nullptr;
    }

    int32 CountClaimedToolPickups(const TArray<TObjectPtr<ASessionToolPickup>>& ToolPickups)
    {
        int32 ClaimedCount = 0;
        for (ASessionToolPickup* ToolPickup : ToolPickups)
        {
            if (ToolPickup != nullptr && ToolPickup->IsClaimed())
            {
                ++ClaimedCount;
            }
        }

        return ClaimedCount;
    }

    const TCHAR* GetCasePhaseMarkerName(ETheLastRiteCasePhase Phase)
    {
        switch (Phase)
        {
        case ETheLastRiteCasePhase::Investigating:
            return TEXT("Investigating");
        case ETheLastRiteCasePhase::RiteReady:
            return TEXT("RiteReady");
        case ETheLastRiteCasePhase::RitualInProgress:
            return TEXT("RitualInProgress");
        case ETheLastRiteCasePhase::SealedAwaitingExit:
            return TEXT("SealedAwaitingExit");
        case ETheLastRiteCasePhase::ClosedWin:
            return TEXT("ClosedWin");
        case ETheLastRiteCasePhase::ClosedFail:
            return TEXT("ClosedFail");
        default:
            return TEXT("Unknown");
        }
    }

    struct FReportEvidenceEntry
    {
        const TCHAR* EvidenceLine;
        const TCHAR* ReportSummary;
    };

    struct FInspectableGuidanceEntry
    {
        const TCHAR* EvidenceSummary;
        const TCHAR* InspectLesson;
        const TCHAR* RecheckLesson;
    };

    constexpr FReportEvidenceEntry TrueClueReportEntries[] =
    {
        { TEXT("TRUE - Nanny Eliza - mirrored wrist marks"), TEXT("Nanny Eliza showed mirrored wrist marks.") },
        { TEXT("TRUE - the cradle - halo of ash-white handprints"), TEXT("the cradle carried ash-white halo prints.") },
        { TEXT("TRUE - the prayer cards - fused into a crown"), TEXT("the prayer cards were fused into a crown.") },
        { TEXT("TRUE - the baby monitor - hymn repeating on every channel"), TEXT("the monitor repeated the hymn on every channel.") },
        { TEXT("TRUE - the nursery wallpaper - child's sun turned into a halo"), TEXT("the nursery mural had been repainted into a halo.") }
    };

    constexpr FReportEvidenceEntry FalseLeadReportEntries[] =
    {
        { TEXT("FALSE - the broken window latch - forced from outside"), TEXT("broken window latch.") },
        { TEXT("FALSE - the pawn ticket pouch - ordinary greed"), TEXT("pawn ticket pouch.") },
        { TEXT("FALSE - the kitchen knife - grease, not offering blood"), TEXT("kitchen knife.") }
    };

    constexpr FInspectableGuidanceEntry TrueClueGuidanceEntries[] =
    {
        { TEXT("Nanny Eliza - mirrored wrist marks"), TEXT("It matters because the pose is ritual symmetry, not panic."), TEXT("It still supports the staged saint-pose, not a random death.") },
        { TEXT("the cradle - halo of ash-white handprints"), TEXT("It matters because the child-facing space is being framed as the altar."), TEXT("It still points the room's weight back toward the child-facing altar.") },
        { TEXT("the prayer cards - fused into a crown"), TEXT("It matters because devotion has been arranged into a crown, not left as ordinary clutter."), TEXT("It still reads as forced sanctity built around the nursery side.") },
        { TEXT("the baby monitor - hymn repeating on every channel"), TEXT("It matters because the room's voice loops back toward the nursery."), TEXT("It still says the room's voice belongs to the child-facing pattern.") },
        { TEXT("the nursery wallpaper - child's sun turned into a halo"), TEXT("It matters because the mural turns the child's sun into a false halo."), TEXT("It still marks the nursery wall as the room's real devotional center.") }
    };

    constexpr FInspectableGuidanceEntry FalseLeadGuidanceEntries[] =
    {
        { TEXT("the broken window latch - forced from outside"), TEXT("It is intrusion and damage, not devotion."), TEXT("It is still just forced damage, not ritual intent.") },
        { TEXT("the pawn ticket pouch - ordinary greed"), TEXT("It is ordinary greed and motive noise, not occult structure."), TEXT("It is still ordinary greed, not part of the saint-mask.") },
        { TEXT("the kitchen knife - grease, not offering blood"), TEXT("It is kitchen grime, not sacrificial blood."), TEXT("It is still kitchen mess, not an offering.") }
    };

    const FInspectableGuidanceEntry* FindGuidanceEntry(const FString& EvidenceSummary, const FInspectableGuidanceEntry* Entries, int32 EntryCount)
    {
        for (int32 Index = 0; Index < EntryCount; ++Index)
        {
            if (EvidenceSummary.Equals(Entries[Index].EvidenceSummary, ESearchCase::CaseSensitive))
            {
                return &Entries[Index];
            }
        }

        return nullptr;
    }

    int32 GetOpeningSweepStepIndexFromSummary(const FString& EvidenceSummary)
    {
        if (EvidenceSummary.Equals(TEXT("Nanny Eliza - mirrored wrist marks"), ESearchCase::CaseSensitive))
        {
            return 0;
        }

        if (EvidenceSummary.Equals(TEXT("the cradle - halo of ash-white handprints"), ESearchCase::CaseSensitive))
        {
            return 1;
        }

        if (EvidenceSummary.Equals(TEXT("the prayer cards - fused into a crown"), ESearchCase::CaseSensitive))
        {
            return 2;
        }

        return INDEX_NONE;
    }

    int32 GetCurrentOpeningSweepStepIndex(const ATheLastRiteGameMode* GameMode)
    {
        if (GameMode == nullptr)
        {
            return INDEX_NONE;
        }

        if (!GameMode->HasEvidenceLine(TEXT("TRUE - Nanny Eliza - mirrored wrist marks")))
        {
            return 0;
        }

        if (!GameMode->HasEvidenceLine(TEXT("TRUE - the cradle - halo of ash-white handprints")))
        {
            return 1;
        }

        if (!GameMode->HasEvidenceLine(TEXT("TRUE - the prayer cards - fused into a crown")))
        {
            return 2;
        }

        return INDEX_NONE;
    }

    static UStaticMesh* LoadBasicMesh(const TCHAR* AssetPath)
    {
        return LoadObject<UStaticMesh>(nullptr, AssetPath);
    }
}

ATheLastRiteGameMode::ATheLastRiteGameMode()
{
    DefaultPawnClass = ATheLastRiteCharacter::StaticClass();
    PlayerControllerClass = APlayerController::StaticClass();
    HUDClass = ATheLastRiteHUD::StaticClass();
    bStartPlayersAsSpectators = false;

    RequiredTrueClues = 5;
    FoundTrueClues = 0;
    FoundFalseLeads = 0;
    TotalFalseLeads = 3;
    CompletedRitualHandSteps = 0;
    CasePhase = ETheLastRiteCasePhase::Investigating;
    RecentEventTimeSeconds = -1000.0f;
    RecentEventDurationSeconds = 5.0f;
    PhasePulseStartedAtSeconds = -1000.0f;
    PhasePulseDurationSeconds = 0.0f;
    PhasePulseColor = FLinearColor::Transparent;

    PlayerSpawnLocation = FVector(0.0f, 0.0f, 140.0f);
    PlayerSpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

    CaseTitleText = NSLOCTEXT(
        "TheLastRite",
        "CaseTitle",
        "CASE 01 - Apartment 302");
    TargetText = NSLOCTEXT(
        "TheLastRite",
        "TargetTitle",
        "Target demon: Hollow Saint");
    ObjectiveText = NSLOCTEXT(
        "TheLastRite",
        "Objective",
        "Apartment 302 is an investigation. Work the opening sweep in order: body -> cradle -> prayer mess, then form roles through the one-of-each tools, confirm the remaining signs, perform the rite, and leave through the front door.");
    StatusText = NSLOCTEXT(
        "TheLastRite",
        "StatusInitial",
        "White labels mark usable objects. First pass: body -> cradle -> prayer mess. Four unique tools in the apartment form Reader, Camera, Censer, and Guard coverage.");
    CurrentObjectiveText = NSLOCTEXT(
        "TheLastRite",
        "CurrentObjectiveInitial",
        "Current objective: inspect Nanny Eliza's body.");
    RecentEventText = FText::GetEmpty();
    ProgressText = FText::GetEmpty();
    DeductionText = NSLOCTEXT(
        "TheLastRite",
        "DeductionInitial",
        "Read of the room: no clean pattern yet. Build the first read in order: body -> cradle -> prayer mess.");
    RitualReadText = NSLOCTEXT(
        "TheLastRite",
        "RitualReadInitial",
        "Ritual read: unresolved. Do not trust any altar until body -> cradle -> prayer mess is logged.");
    NextMoveText = NSLOCTEXT(
        "TheLastRite",
        "NextMoveInitial",
        "Opening route: body -> cradle -> prayer mess. Inspect Nanny Eliza's body first, then start claiming the one-of-each tools before the rite.");
    FocusZoneText = NSLOCTEXT(
        "TheLastRite",
        "FocusZoneInitial",
        "Focus zone: body first. Then stay on cradle -> prayer mess before you test anything louder in the room.");
    TheoryChainText = NSLOCTEXT(
        "TheLastRite",
        "TheoryChainInitial",
        "Theory chain: build the pattern in order: body pose, cradle halo, prayer crown, monitor hymn, mural halo.");
    RiskText = NSLOCTEXT(
        "TheLastRite",
        "RiskInitial",
        "Risk read: the room will try to pull you toward ugly spectacle before the child-facing pattern is proven.");
    CorrectAnchorReadText = NSLOCTEXT(
        "TheLastRite",
        "CorrectAnchorReadInitial",
        "Nursery sigil read: not ready to call yet. Finish the opening sweep before naming the altar.");
    WrongAnchorReadText = NSLOCTEXT(
        "TheLastRite",
        "WrongAnchorReadInitial",
        "Mirror circle read: treat it as suspect until the room gives you more than spectacle.");
    EndingText = FText::GetEmpty();
    EndingDetailText = FText::GetEmpty();
    ActiveRitualAnchor = nullptr;
}

void ATheLastRiteGameMode::BeginPlay()
{
    Super::BeginPlay();

    BuildRoom();
    BuildSetDressing();
    BuildCaseContent();
    SpawnLights();
    UpdateRitualAnchors();
    UpdateCasePhaseFromEvidence();
    RefreshCasePresentation(true);

    if (FParse::Param(FCommandLine::Get(), TEXT("AutoVerifyRitualFlow")))
    {
        RunAutomatedRitualVerification();
    }
}

void ATheLastRiteGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    Super::HandleStartingNewPlayer_Implementation(NewPlayer);

    if (NewPlayer != nullptr && NewPlayer->GetPawn() != nullptr)
    {
        NewPlayer->SetControlRotation(PlayerSpawnRotation);
        NewPlayer->GetPawn()->SetActorLocation(PlayerSpawnLocation);
        NewPlayer->GetPawn()->SetActorRotation(PlayerSpawnRotation);
    }
}

void ATheLastRiteGameMode::HandleInspectableProp(AInspectableProp* Prop)
{
    if (Prop == nullptr)
    {
        return;
    }

    if (IsCaseResolved())
    {
        SetStatusText(GetResolvedInteractionText());
        return;
    }

    if (CasePhase == ETheLastRiteCasePhase::RitualInProgress)
    {
        SetStatusText(FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "InspectBlockedDuringRitual",
                "The 4-hand rite is already underway. Keep working the nursery sigil. {0}"),
            GetRitualSequenceStateText()));
        return;
    }

    if (Prop->WasInspected())
    {
        const FText NextGuidance = GetActiveGuidanceText();
        const FText ReviewGuidance = GetInspectableGuidanceText(Prop, true);
        if (Prop->IsTrueClue())
        {
            const FText ReminderText = CasePhase == ETheLastRiteCasePhase::RiteReady
                ? NSLOCTEXT("TheLastRite", "TrueReminderRiteReady", "Already logged. This still confirms the child-facing altar, not the mirror.")
                : NSLOCTEXT("TheLastRite", "TrueReminder", "Already logged. This remains real evidence.");
            SetStatusText(FText::Format(
                NSLOCTEXT("TheLastRite", "AlreadyCheckedTrue", "{0} {1} {2} {3}"),
                Prop->GetClueText(),
                ReminderText,
                ReviewGuidance,
                NextGuidance));
        }
        else
        {
            SetStatusText(FText::Format(
                NSLOCTEXT("TheLastRite", "AlreadyCheckedFalse", "{0} Already ruled out. {1} {2}"),
                Prop->GetClueText(),
                ReviewGuidance,
                NextGuidance));
        }
        return;
    }

    if (DoesEvidenceRequireFilmCamera(Prop->GetEvidenceSummary().ToString()))
    {
        ASessionToolPickup* FilmCamera = FindSessionToolPickupByType(SessionToolPickups, ETheLastRiteSessionTool::FilmCamera);
        if (!HasClaimedToolPickup(FilmCamera))
        {
            SetStatusText(FText::Format(
                NSLOCTEXT(
                    "TheLastRite",
                    "CameraCoverageBlocked",
                    "{0} stays unread without the Film Camera. Claim the camera to expose the hidden evidence coverage before you trust this sign. {1}"),
                Prop->GetDisplayName(),
                GetToolNeedText()));
            UE_LOG(
                LogTemp,
                Display,
                TEXT("GP-C2 CAMERA_GATE evidence_summary=\"%s\" film_camera_claimed=\"false\""),
                *Prop->GetEvidenceSummary().ToString());
            return;
        }
    }

    Prop->MarkInspected();
    FText StatusUpdate = FText::GetEmpty();
    bool bAppendNextMoveToStatus = false;

    if (Prop->IsTrueClue())
    {
        FoundTrueClues = FMath::Min(FoundTrueClues + 1, RequiredTrueClues);
        AddEvidenceLine(FString::Printf(TEXT("TRUE - %s"), *Prop->GetEvidenceSummary().ToString()));
        const FText LessonText = GetInspectableGuidanceText(Prop, false);
        const bool bOpeningSweepComplete = IsOpeningSweepComplete();
        if (!bOpeningSweepComplete)
        {
            StatusUpdate = FText::Format(
                Prop->IsOpeningSweepTarget()
                    ? NSLOCTEXT("TheLastRite", "TrueClueOpeningSweep", "{0} is real. {1} {2}. Next: {3}.")
                    : NSLOCTEXT("TheLastRite", "TrueClueOutOfOrder", "{0} is real, but keep the opening sweep in order. {1} {2}. Next: {3}."),
                Prop->GetClueText(),
                LessonText,
                GetOpeningSweepStateText(),
                GetNextStarterTargetText());
        }
        else if (FoundTrueClues >= RequiredTrueClues)
        {
            ObjectiveText = NSLOCTEXT(
                "TheLastRite",
                "ObjectiveRitualReady",
                "Evidence complete. The child-facing pattern is locked. Start the 4-hand rite at the nursery sigil, where the real chain ends.");
            StatusUpdate = FText::Format(
                NSLOCTEXT("TheLastRite", "EnoughClues", "{0} {1} You have enough evidence. The real pattern converges around the child-facing altar, not the mirror bait."),
                Prop->GetClueText(),
                LessonText);
            bAppendNextMoveToStatus = true;
            TriggerPhasePulse(FLinearColor(0.96f, 0.75f, 0.22f, 1.0f), 1.0f);
        }
        else
        {
            StatusUpdate = FText::Format(
                NSLOCTEXT("TheLastRite", "TrueClue", "{0} That is a true sign of the Hollow Saint. {1}"),
                Prop->GetClueText(),
                LessonText);
            bAppendNextMoveToStatus = true;
        }
    }
    else
    {
        ++FoundFalseLeads;
        AddEvidenceLine(FString::Printf(TEXT("FALSE - %s"), *Prop->GetEvidenceSummary().ToString()));
        const FText LessonText = GetInspectableGuidanceText(Prop, false);
        StatusUpdate = FText::Format(
            IsOpeningSweepComplete()
                ? NSLOCTEXT("TheLastRite", "FalseLead", "{0} False lead. {1}")
                : NSLOCTEXT("TheLastRite", "FalseLeadOpeningSweep", "{0} False lead. {1} Finish the opening sweep first: {2}."),
            Prop->GetClueText(),
            LessonText,
            GetNextStarterTargetText());
        bAppendNextMoveToStatus = IsOpeningSweepComplete();
    }

    UpdateCasePhaseFromEvidence();
    RefreshCasePresentation(false);

    if (bAppendNextMoveToStatus)
    {
        const FText NextGuidance = GetNextMoveText();
        if (!NextGuidance.IsEmpty())
        {
            StatusUpdate = FText::Format(
                NSLOCTEXT("TheLastRite", "StatusWithNextMove", "{0} {1}"),
                StatusUpdate,
                NextGuidance);
        }
    }

    SetStatusText(StatusUpdate);
}

void ATheLastRiteGameMode::HandleToolPickup(ASessionToolPickup* ToolPickup)
{
    if (ToolPickup == nullptr)
    {
        return;
    }

    if (IsCaseResolved())
    {
        SetStatusText(GetResolvedInteractionText());
        return;
    }

    if (ToolPickup->IsClaimed())
    {
        SetStatusText(FText::Format(
            NSLOCTEXT("TheLastRite", "ToolAlreadyClaimed", "{0} is already claimed. {1} role is already covered. {2}"),
            ToolPickup->GetDisplayName(),
            ToolPickup->GetRoleText(),
            GetToolNeedText()));
        return;
    }

    ToolPickup->MarkClaimed();
    AddEvidenceLine(FString::Printf(TEXT("TOOL - %s claimed"), *ToolPickup->GetDisplayName().ToString()));
    UE_LOG(
        LogTemp,
        Display,
        TEXT("GP-C2 TOOL_CLAIM tool=\"%s\" role=\"%s\" claimed_count=%d/4 phase=\"%s\""),
        GetSessionToolMarkerName(ToolPickup->GetToolType()),
        *ToolPickup->GetRoleText().ToString(),
        CountClaimedToolPickups(SessionToolPickups),
        GetCasePhaseMarkerName(CasePhase));

    RefreshCasePresentation(false);
    SetStatusText(FText::Format(
        NSLOCTEXT("TheLastRite", "ToolClaimedStatus", "{0} claimed. {1} role is now covered. {2}"),
        ToolPickup->GetDisplayName(),
        ToolPickup->GetRoleText(),
        GetToolNeedText()));
}

void ATheLastRiteGameMode::HandleRitualAnchor(ARitualAnchor* Anchor)
{
    if (Anchor == nullptr)
    {
        return;
    }

    if (IsCaseResolved())
    {
        SetStatusText(GetResolvedInteractionText());
        return;
    }

    if (FoundTrueClues < RequiredTrueClues)
    {
        const FText NextGuidance = GetActiveGuidanceText();
        SetStatusText(FText::Format(
            NSLOCTEXT("TheLastRite", "NeedMoreClues", "Not yet. You still need {0} real clue(s) before the rite. Finish the evidence chain before you trust any anchor. {1}"),
            FText::AsNumber(RequiredTrueClues - FoundTrueClues),
            NextGuidance));
        return;
    }

    if (!Anchor->IsCorrectAnchor())
    {
        ResolveFailedRitual(Anchor);
        RefreshCasePresentation(true);
        return;
    }

    ETheLastRiteSessionTool RequiredToolType;
    if (TryGetRequiredToolForRitualHand(CompletedRitualHandSteps, RequiredToolType))
    {
        ASessionToolPickup* RequiredToolPickup = FindSessionToolPickupByType(SessionToolPickups, RequiredToolType);
        if (!HasClaimedToolPickup(RequiredToolPickup))
        {
            SetStatusText(FText::Format(
                NSLOCTEXT(
                    "TheLastRite",
                    "RitualToolMissing",
                    "The {0} hand cannot hold yet. Claim {1} before you continue the 4-hand rite. {2}"),
                GetCurrentRitualHandText(),
                RequiredToolPickup != nullptr ? RequiredToolPickup->GetDisplayName() : FText::FromString(TEXT("the missing tool")),
                GetToolNeedText()));
            UE_LOG(
                LogTemp,
                Display,
                TEXT("GP-C2 RITUAL_TOOL_GATE required_tool=\"%s\" active_hand=\"%s\" phase=\"%s\""),
                GetSessionToolMarkerName(RequiredToolType),
                *GetCurrentRitualHandText().ToString(),
                GetCasePhaseMarkerName(CasePhase));
            return;
        }
    }

    AdvanceCorrectRitualHand(Anchor);
    RefreshCasePresentation(IsCaseResolved());
}

void ATheLastRiteGameMode::AdvanceCorrectRitualHand(ARitualAnchor* Anchor)
{
    if (Anchor == nullptr)
    {
        return;
    }

    ActiveRitualAnchor = Anchor;
    const FText CompletedHand = GetCurrentRitualHandText();
    const bool bStartingSoloPlaceholderPass = CompletedRitualHandSteps == 0;
    AddEvidenceLine(FString::Printf(TEXT("RITE - %s hand held at the nursery sigil"), *CompletedHand.ToString()));
    ++CompletedRitualHandSteps;

    UE_LOG(
        LogTemp,
        Display,
        TEXT("GP-C1 RITUAL_MARKER step_complete=%d/%d completed_hand=\"%s\" next_hand=\"%s\" anchor_correct=%s"),
        CompletedRitualHandSteps,
        RitualHandStepCount,
        *CompletedHand.ToString(),
        *GetCurrentRitualHandText().ToString(),
        Anchor->IsCorrectAnchor() ? TEXT("true") : TEXT("false"));

    if (bStartingSoloPlaceholderPass)
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("GP-C1 SOLO_PLACEHOLDER marker=\"enabled\" support=\"%s\""),
            *GetRitualSequenceSupportText().ToString());
    }

    if (CompletedRitualHandSteps >= RitualHandStepCount)
    {
        ResolveSuccessfulRitual(Anchor);
        return;
    }

    CasePhase = ETheLastRiteCasePhase::RitualInProgress;
    ObjectiveText = NSLOCTEXT(
        "TheLastRite",
        "ObjectiveRitualInProgress",
        "The 4-hand rite is underway at the nursery sigil. Keep the sequence moving hand by hand and do not break to the mirror.");
    SetStatusText(FText::Format(
        NSLOCTEXT(
            "TheLastRite",
            "RitualHandAdvanced",
            "{0} hand held at the nursery sigil. {1} hand is now active. {2}"),
        CompletedHand,
        GetCurrentRitualHandText(),
        GetRitualSequenceSupportText()));
    TriggerPhasePulse(FLinearColor(0.92f, 0.74f, 0.24f, 1.0f), 0.8f);
}

void ATheLastRiteGameMode::ResolveSuccessfulRitual(ARitualAnchor* Anchor)
{
    if (Anchor == nullptr)
    {
        return;
    }

    Anchor->MarkActivated();
    CasePhase = ETheLastRiteCasePhase::SealedAwaitingExit;
    ObjectiveText = NSLOCTEXT(
        "TheLastRite",
        "ObjectiveWon",
        "The rite held at the nursery sigil. Leave through the front door, file the case, and do not reopen the room.");
    EndingText = FText::GetEmpty();
    EndingDetailText = FText::GetEmpty();
    SetStatusText(NSLOCTEXT(
        "TheLastRite",
        "WinStatus",
        "All four hands cleared at the nursery sigil. The seal took. Leave through the front door and close the case cleanly."));
    AddEvidenceLine(TEXT("RITE - correct anchor"));
    DeductionText = NSLOCTEXT(
        "TheLastRite",
        "DeductionWon",
        "Conclusion: the saint fed through the child-facing altar, not the mirror imitation. The full 4-hand rite held, and the seal is now stable.");
    UE_LOG(
        LogTemp,
        Display,
        TEXT("GP-C1 RITUAL_RESULT outcome=\"success\" final_phase=\"%s\" completed_steps=%d correct_anchor=\"%s\""),
        GetCasePhaseMarkerName(CasePhase),
        CompletedRitualHandSteps,
        *Anchor->GetDisplayName().ToString());
    TriggerPhasePulse(FLinearColor(0.30f, 0.95f, 0.55f, 1.0f), 1.2f);
}

void ATheLastRiteGameMode::ResolveFailedRitual(ARitualAnchor* Anchor)
{
    if (Anchor == nullptr)
    {
        return;
    }

    Anchor->MarkActivated();
    CasePhase = ETheLastRiteCasePhase::ClosedFail;
    ObjectiveText = NSLOCTEXT(
        "TheLastRite",
        "ObjectiveLost",
        "Case failed. The mirror bait was chosen over the real altar.");
    EndingText = NSLOCTEXT(
        "TheLastRite",
        "LoseEnding",
        "WRONG RITE\nYou fed the imitation.");
    EndingDetailText = NSLOCTEXT(
        "TheLastRite",
        "LoseEndingDetail",
        "The mirror circle borrowed symmetry and spectacle, but the evidence chain never ended there. The real pattern lived around the child-facing altar.");
    SetStatusText(
        CompletedRitualHandSteps > 0
            ? NSLOCTEXT(
                "TheLastRite",
                "LoseStatusInterruptedRite",
                "You broke the 4-hand rite by jumping to the mirror circle. The mirror had staging, but no corroboration. The nursery chain was the real altar.")
            : NSLOCTEXT(
                "TheLastRite",
                "LoseStatus",
                "You chose the wrong anchor. The mirror had staging, but no corroboration. The nursery chain was the real altar."));
    AddEvidenceLine(TEXT("RITE - wrong anchor"));
    DeductionText = NSLOCTEXT(
        "TheLastRite",
        "DeductionLost",
        "Conclusion missed: the mirror copied the saint-mask, but the body, cradle, prayer mess, hymn, and mural all resolved child-side.");
    UE_LOG(
        LogTemp,
        Display,
        TEXT("GP-C1 RITUAL_RESULT outcome=\"failure\" final_phase=\"%s\" completed_steps=%d chosen_anchor=\"%s\""),
        GetCasePhaseMarkerName(CasePhase),
        CompletedRitualHandSteps,
        *Anchor->GetDisplayName().ToString());
    TriggerPhasePulse(FLinearColor(1.0f, 0.05f, 0.03f, 1.0f), 1.4f);
}

FText ATheLastRiteGameMode::GetInspectableGuidanceText(const AInspectableProp* Prop, bool bForRecheck) const
{
    if (Prop == nullptr)
    {
        return FText::GetEmpty();
    }

    const FString EvidenceSummary = Prop->GetEvidenceSummary().ToString();
    const FInspectableGuidanceEntry* Entry = Prop->IsTrueClue()
        ? FindGuidanceEntry(EvidenceSummary, TrueClueGuidanceEntries, UE_ARRAY_COUNT(TrueClueGuidanceEntries))
        : FindGuidanceEntry(EvidenceSummary, FalseLeadGuidanceEntries, UE_ARRAY_COUNT(FalseLeadGuidanceEntries));

    if (Entry == nullptr)
    {
        return FText::GetEmpty();
    }

    return FText::FromString(bForRecheck ? Entry->RecheckLesson : Entry->InspectLesson);
}

FText ATheLastRiteGameMode::GetInspectablePromptText(const AInspectableProp* Prop) const
{
    if (Prop == nullptr || IsCaseResolved())
    {
        return FText::GetEmpty();
    }

    if (CasePhase == ETheLastRiteCasePhase::RitualInProgress)
    {
        return FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "InspectPromptRitualInProgress",
                "Rite underway. Stay on the nursery sigil. {0}"),
            GetRitualSequenceStateText());
    }

    if (IsOpeningSweepComplete())
    {
        return FText::GetEmpty();
    }

    const int32 StepIndex = GetOpeningSweepStepIndexFromSummary(Prop->GetEvidenceSummary().ToString());
    const int32 CurrentStepIndex = GetCurrentOpeningSweepStepIndex(this);

    if (StepIndex == CurrentStepIndex)
    {
        if (StepIndex == 1)
        {
            return FText::Format(
                NSLOCTEXT(
                    "TheLastRite",
                    "InspectPromptOpeningSweepCradleCurrent",
                    "Press E - Inspect {0} (opening sweep step {1}/3; follow CHAIN - NURSERY from the body)"),
                Prop->GetDisplayName(),
                FText::AsNumber(StepIndex + 1));
        }

        return FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "InspectPromptOpeningSweepCurrent",
                "Press E - Inspect {0} (opening sweep step {1}/3)"),
            Prop->GetDisplayName(),
            FText::AsNumber(StepIndex + 1));
    }

    if (StepIndex != INDEX_NONE)
    {
        return FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "InspectPromptOpeningSweepLater",
                "Press E - Inspect {0} (opening sweep step {1}/3; later in route)"),
            Prop->GetDisplayName(),
            FText::AsNumber(StepIndex + 1));
    }

    return FText::Format(
        NSLOCTEXT(
            "TheLastRite",
            "InspectPromptDeferredOpeningSweepRoute",
            "Press E - Inspect {0} (deferred until body -> cradle -> prayer mess is logged)"),
        Prop->GetDisplayName());
}

int32 ATheLastRiteGameMode::GetInspectableFocusPriority(const AInspectableProp* Prop) const
{
    if (Prop == nullptr)
    {
        return 0;
    }

    if (IsCaseResolved())
    {
        return -15;
    }

    if (Prop->WasInspected())
    {
        return (CasePhase == ETheLastRiteCasePhase::RiteReady || CasePhase == ETheLastRiteCasePhase::RitualInProgress) ? -30 : -5;
    }

    if (CasePhase == ETheLastRiteCasePhase::RiteReady)
    {
        return Prop->IsTrueClue() ? 5 : -20;
    }

    if (CasePhase == ETheLastRiteCasePhase::RitualInProgress)
    {
        return -40;
    }

    if (!IsOpeningSweepComplete())
    {
        const int32 StepIndex = GetOpeningSweepStepIndexFromSummary(Prop->GetEvidenceSummary().ToString());
        if (StepIndex == GetCurrentOpeningSweepStepIndex(this))
        {
            return 120;
        }

        if (StepIndex != INDEX_NONE)
        {
            return 45;
        }

        return -20;
    }

    return Prop->IsTrueClue() ? 50 : 15;
}

void ATheLastRiteGameMode::HandleCaseExit(ACaseExit* Exit)
{
    if (Exit == nullptr)
    {
        return;
    }

    if (CasePhase == ETheLastRiteCasePhase::ClosedWin || CasePhase == ETheLastRiteCasePhase::ClosedFail)
    {
        SetStatusText(GetResolvedInteractionText());
        return;
    }

    if (CasePhase != ETheLastRiteCasePhase::SealedAwaitingExit)
    {
        const FText NextGuidance = GetActiveGuidanceText();
        SetStatusText(FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "ExitLockedStatus",
                "You cannot leave yet. The front door only matters after the seal holds. {0}"),
            NextGuidance));
        return;
    }

    Exit->MarkUsed();
    CasePhase = ETheLastRiteCasePhase::ClosedWin;
    ObjectiveText = NSLOCTEXT(
        "TheLastRite",
        "ObjectiveClosed",
        "Case filed. Apartment 302 is sealed, logged, and ready for cleanup.");
    EndingText = NSLOCTEXT(
        "TheLastRite",
        "FinalWinEnding",
        "CASE CLOSED\nApartment 302 is sealed cleanly.");
    EndingDetailText = NSLOCTEXT(
        "TheLastRite",
        "FinalWinDetail",
        "You held the rite, crossed the front door, and filed the report. The Hollow Saint lost the room because the evidence chain was read correctly and the false altar was denied.");
    SetStatusText(NSLOCTEXT(
        "TheLastRite",
        "FinalWinStatus",
        "Front door crossed. Report filed. Apartment 302 is sealed for cleanup."));
    AddEvidenceLine(TEXT("EXIT - front door secured"));
    DeductionText = NSLOCTEXT(
        "TheLastRite",
        "DeductionClosed",
        "Final read: the nursery sigil held because the whole chain ended there, the mirror was imitation, and the case is now sealed.");
    UE_LOG(
        LogTemp,
        Display,
        TEXT("GP-C1 CASE_RESULT outcome=\"case_closed\" final_phase=\"%s\" exit_recorded=\"true\""),
        GetCasePhaseMarkerName(CasePhase));
    TriggerPhasePulse(FLinearColor(0.85f, 0.95f, 0.76f, 1.0f), 1.1f);

    RefreshCasePresentation(true);
}

ETheLastRiteCasePhase ATheLastRiteGameMode::GetCasePhase() const
{
    return CasePhase;
}

FText ATheLastRiteGameMode::GetCaseTitleText() const
{
    return CaseTitleText;
}

FText ATheLastRiteGameMode::GetTargetText() const
{
    return TargetText;
}

FText ATheLastRiteGameMode::GetCurrentObjectiveText() const
{
    return CurrentObjectiveText;
}

FText ATheLastRiteGameMode::GetObjectiveText() const
{
    return ObjectiveText;
}

FText ATheLastRiteGameMode::GetStatusText() const
{
    return StatusText;
}

FText ATheLastRiteGameMode::GetRecentEventText() const
{
    if (RecentEventText.IsEmpty() || GetWorld() == nullptr)
    {
        return FText::GetEmpty();
    }

    const float AgeSeconds = GetWorld()->GetTimeSeconds() - RecentEventTimeSeconds;
    if (AgeSeconds > RecentEventDurationSeconds)
    {
        return FText::GetEmpty();
    }

    return RecentEventText;
}

FText ATheLastRiteGameMode::GetProgressText() const
{
    return ProgressText;
}

FText ATheLastRiteGameMode::GetDeductionText() const
{
    return DeductionText;
}

FText ATheLastRiteGameMode::GetRitualReadText() const
{
    return RitualReadText;
}

FText ATheLastRiteGameMode::GetNextMoveText() const
{
    return NextMoveText;
}

FText ATheLastRiteGameMode::GetFocusZoneText() const
{
    return FocusZoneText;
}

FText ATheLastRiteGameMode::GetTheoryChainText() const
{
    return TheoryChainText;
}

FText ATheLastRiteGameMode::GetRiskText() const
{
    return RiskText;
}

FText ATheLastRiteGameMode::GetCorrectAnchorReadText() const
{
    return CorrectAnchorReadText;
}

FText ATheLastRiteGameMode::GetWrongAnchorReadText() const
{
    return WrongAnchorReadText;
}

FText ATheLastRiteGameMode::GetEndingText() const
{
    return EndingText;
}

FText ATheLastRiteGameMode::GetEndingDetailText() const
{
    return EndingDetailText;
}

FText ATheLastRiteGameMode::GetResolvedInteractionText() const
{
    switch (CasePhase)
    {
    case ETheLastRiteCasePhase::SealedAwaitingExit:
        return NSLOCTEXT("TheLastRite", "ResolvedInteractionExit", "Seal holds. Use the front door to file the case and close Apartment 302.");
    case ETheLastRiteCasePhase::ClosedWin:
        return NSLOCTEXT("TheLastRite", "ResolvedInteractionClosedWin", "Case closed. Press R to restart the case or Esc to quit.");
    case ETheLastRiteCasePhase::ClosedFail:
        return NSLOCTEXT("TheLastRite", "ResolvedInteractionClosedFail", "Case failed. Press R to restart the case or Esc to quit.");
    case ETheLastRiteCasePhase::Investigating:
    case ETheLastRiteCasePhase::RiteReady:
    case ETheLastRiteCasePhase::RitualInProgress:
    default:
        return FText::GetEmpty();
    }
}

FText ATheLastRiteGameMode::GetPostCaseControlText() const
{
    return NSLOCTEXT("TheLastRite", "PostCaseControls", "Press R to restart | Esc to quit");
}

const TArray<FString>& ATheLastRiteGameMode::GetEvidenceLines() const
{
    return EvidenceLines;
}

const TArray<FString>& ATheLastRiteGameMode::GetFinalReportLines() const
{
    return FinalReportLines;
}

bool ATheLastRiteGameMode::HasEvidenceLine(const FString& FullLine) const
{
    return EvidenceLines.Contains(FullLine);
}

bool ATheLastRiteGameMode::IsOpeningSweepComplete() const
{
    return GetOpeningSweepCount() >= 3;
}

FLinearColor ATheLastRiteGameMode::GetPhasePulseColor() const
{
    return PhasePulseColor;
}

float ATheLastRiteGameMode::GetPhasePulseOpacity() const
{
    if (GetWorld() == nullptr || PhasePulseDurationSeconds <= 0.0f)
    {
        return 0.0f;
    }

    const float AgeSeconds = GetWorld()->GetTimeSeconds() - PhasePulseStartedAtSeconds;
    if (AgeSeconds < 0.0f || AgeSeconds > PhasePulseDurationSeconds)
    {
        return 0.0f;
    }

    const float Fade = 1.0f - (AgeSeconds / PhasePulseDurationSeconds);
    return FMath::Clamp(Fade * 0.32f, 0.0f, 0.32f);
}

bool ATheLastRiteGameMode::IsCaseResolved() const
{
    return CasePhase == ETheLastRiteCasePhase::SealedAwaitingExit
        || CasePhase == ETheLastRiteCasePhase::ClosedWin
        || CasePhase == ETheLastRiteCasePhase::ClosedFail;
}

bool ATheLastRiteGameMode::IsCaseClosed() const
{
    return CasePhase == ETheLastRiteCasePhase::ClosedWin
        || CasePhase == ETheLastRiteCasePhase::ClosedFail;
}

bool ATheLastRiteGameMode::DidPlayerWin() const
{
    return CasePhase == ETheLastRiteCasePhase::SealedAwaitingExit
        || CasePhase == ETheLastRiteCasePhase::ClosedWin;
}

bool ATheLastRiteGameMode::IsRitualSequenceActive() const
{
    return CasePhase == ETheLastRiteCasePhase::RitualInProgress;
}

FText ATheLastRiteGameMode::GetRitualSequenceStateText() const
{
    if (FoundTrueClues < RequiredTrueClues || IsCaseResolved())
    {
        return FText::GetEmpty();
    }

    return FText::Format(
        NSLOCTEXT(
            "TheLastRite",
            "RitualSequenceState",
            "4-hand rite: {0}/{1} complete | Active hand: {2}"),
        FText::AsNumber(CompletedRitualHandSteps),
        FText::AsNumber(RitualHandStepCount),
        GetCurrentRitualHandText());
}

FText ATheLastRiteGameMode::GetRitualSequenceSupportText() const
{
    if (FoundTrueClues < RequiredTrueClues || IsCaseResolved())
    {
        return FText::GetEmpty();
    }

    const ASessionToolPickup* HolyBook = FindSessionToolPickupByType(SessionToolPickups, ETheLastRiteSessionTool::HolyBook);
    const ASessionToolPickup* Censer = FindSessionToolPickupByType(SessionToolPickups, ETheLastRiteSessionTool::Censer);
    const ASessionToolPickup* Crucifix = FindSessionToolPickupByType(SessionToolPickups, ETheLastRiteSessionTool::Crucifix);

    if (!HasClaimedToolPickup(HolyBook) || !HasClaimedToolPickup(Censer) || !HasClaimedToolPickup(Crucifix))
    {
        return NSLOCTEXT(
            "TheLastRite",
            "RitualSequenceSupportMissingTools",
            "Solo placeholder: the rite still expects role tools. Claim the Holy Book, Censer, and Crucifix so one operator can cycle Reader, Anchor, Censer, and Guard cleanly.");
    }

    return NSLOCTEXT(
        "TheLastRite",
        "RitualSequenceSupportReady",
        "Solo placeholder: one operator can cycle Reader, Anchor, Censer, and Guard by pressing E at the nursery sigil because the role kit is already claimed.");
}

FText ATheLastRiteGameMode::GetToolRoleText() const
{
    const auto BuildToolState = [this](ETheLastRiteSessionTool ToolType, const TCHAR* ClaimedText, const TCHAR* MissingText)
    {
        const ASessionToolPickup* ToolPickup = FindSessionToolPickupByType(SessionToolPickups, ToolType);
        return HasClaimedToolPickup(ToolPickup) ? ClaimedText : MissingText;
    };

    return FText::FromString(FString::Printf(
        TEXT("Tool roles: Reader/Holy Book %s | Camera %s | Censer %s | Guard/Crucifix %s"),
        BuildToolState(ETheLastRiteSessionTool::HolyBook, TEXT("claimed"), TEXT("missing")),
        BuildToolState(ETheLastRiteSessionTool::FilmCamera, TEXT("claimed"), TEXT("missing")),
        BuildToolState(ETheLastRiteSessionTool::Censer, TEXT("claimed"), TEXT("missing")),
        BuildToolState(ETheLastRiteSessionTool::Crucifix, TEXT("claimed"), TEXT("missing"))));
}

FText ATheLastRiteGameMode::GetToolNeedText() const
{
    const ASessionToolPickup* HolyBook = FindSessionToolPickupByType(SessionToolPickups, ETheLastRiteSessionTool::HolyBook);
    const ASessionToolPickup* FilmCamera = FindSessionToolPickupByType(SessionToolPickups, ETheLastRiteSessionTool::FilmCamera);
    const ASessionToolPickup* Censer = FindSessionToolPickupByType(SessionToolPickups, ETheLastRiteSessionTool::Censer);
    const ASessionToolPickup* Crucifix = FindSessionToolPickupByType(SessionToolPickups, ETheLastRiteSessionTool::Crucifix);

    if (IsCaseResolved())
    {
        return NSLOCTEXT("TheLastRite", "ToolNeedResolved", "Role formation is now locked into the case-close record.");
    }

    if (!HasClaimedToolPickup(FilmCamera))
    {
        return NSLOCTEXT(
            "TheLastRite",
            "ToolNeedCamera",
            "Next tool need: claim the Film Camera so the monitor hymn and nursery mural can enter the evidence chain.");
    }

    if (FoundTrueClues >= RequiredTrueClues)
    {
        if (!HasClaimedToolPickup(HolyBook))
        {
            return NSLOCTEXT(
                "TheLastRite",
                "ToolNeedHolyBook",
                "Next tool need: claim the Holy Book so the Reader hand can carry the final Latin cleanly.");
        }

        if (!HasClaimedToolPickup(Censer))
        {
            return NSLOCTEXT(
                "TheLastRite",
                "ToolNeedCenser",
                "Next tool need: claim the Censer before the purge hand starts at the nursery sigil.");
        }

        if (!HasClaimedToolPickup(Crucifix))
        {
            return NSLOCTEXT(
                "TheLastRite",
                "ToolNeedCrucifix",
                "Next tool need: claim the Crucifix so the Guard hand can close the rite instead of leaving the last pass abstract.");
        }

        return NSLOCTEXT(
            "TheLastRite",
            "ToolNeedReadyForRite",
            "Role kit ready: Reader, Camera, Censer, and Guard coverage are all claimed around the apartment loop.");
    }

    if (!IsOpeningSweepComplete())
    {
        return NSLOCTEXT(
            "TheLastRite",
            "ToolNeedOpeningSweep",
            "Keep the opening sweep primary, then sweep the tool kit so role formation is stable before the rite.");
    }

    if (!HasClaimedToolPickup(HolyBook))
    {
        return NSLOCTEXT(
            "TheLastRite",
            "ToolNeedHolyBookEarly",
            "Next tool need: claim the Holy Book to lock Reader coverage before the rite-ready handoff.");
    }

    if (!HasClaimedToolPickup(Censer))
    {
        return NSLOCTEXT(
            "TheLastRite",
            "ToolNeedCenserEarly",
            "Next tool need: claim the Censer so the apartment loop has a real purge role instead of a missing hand.");
    }

    if (!HasClaimedToolPickup(Crucifix))
    {
        return NSLOCTEXT(
            "TheLastRite",
            "ToolNeedCrucifixEarly",
            "Next tool need: claim the Crucifix so the Guard role is formed before the ritual loop closes.");
    }

    return NSLOCTEXT(
        "TheLastRite",
        "ToolNeedCoverageStable",
        "Role coverage is stable. Finish the evidence chain and keep the real altar call clean.");
}

FText ATheLastRiteGameMode::GetCurrentRitualHandText() const
{
    const int32 StepIndex = FMath::Clamp(CompletedRitualHandSteps, 0, RitualHandStepCount - 1);

    switch (StepIndex)
    {
    case 0:
        return NSLOCTEXT("TheLastRite", "RitualHandReader", "Reader");
    case 1:
        return NSLOCTEXT("TheLastRite", "RitualHandAnchor", "Anchor");
    case 2:
        return NSLOCTEXT("TheLastRite", "RitualHandCenser", "Censer");
    case 3:
    default:
        return NSLOCTEXT("TheLastRite", "RitualHandGuard", "Guard");
    }
}

FText ATheLastRiteGameMode::GetNextRitualHandText() const
{
    const int32 NextStepIndex = CompletedRitualHandSteps + 1;
    if (NextStepIndex >= RitualHandStepCount)
    {
        return NSLOCTEXT("TheLastRite", "RitualHandSeal", "seal and exit");
    }

    switch (NextStepIndex)
    {
    case 1:
        return NSLOCTEXT("TheLastRite", "RitualHandAnchorNext", "Anchor");
    case 2:
        return NSLOCTEXT("TheLastRite", "RitualHandCenserNext", "Censer");
    case 3:
    default:
        return NSLOCTEXT("TheLastRite", "RitualHandGuardNext", "Guard");
    }
}

void ATheLastRiteGameMode::BuildRoom()
{
    const FLinearColor FloorColor(0.18f, 0.18f, 0.20f);
    const FLinearColor WallColor(0.33f, 0.36f, 0.38f);
    const FLinearColor TrimColor(0.24f, 0.25f, 0.28f);

    SpawnRoomPiece(FVector(0.0f, 0.0f, 0.0f), FVector(14.0f, 14.0f, 0.25f), FRotator::ZeroRotator, FloorColor);
    SpawnRoomPiece(FVector(0.0f, 0.0f, 600.0f), FVector(14.0f, 14.0f, 0.25f), FRotator::ZeroRotator, WallColor);
    SpawnRoomPiece(FVector(1400.0f, 0.0f, 300.0f), FVector(0.25f, 14.0f, 6.0f), FRotator::ZeroRotator, WallColor);
    SpawnRoomPiece(FVector(-1400.0f, 0.0f, 300.0f), FVector(0.25f, 14.0f, 6.0f), FRotator::ZeroRotator, WallColor);
    SpawnRoomPiece(FVector(0.0f, 1400.0f, 300.0f), FVector(14.0f, 0.25f, 6.0f), FRotator::ZeroRotator, WallColor);
    SpawnRoomPiece(FVector(0.0f, -1400.0f, 300.0f), FVector(14.0f, 0.25f, 6.0f), FRotator::ZeroRotator, WallColor);
    SpawnRoomPiece(FVector(360.0f, -560.0f, 300.0f), FVector(0.2f, 8.2f, 6.0f), FRotator::ZeroRotator, WallColor);
    SpawnRoomPiece(FVector(360.0f, 1080.0f, 300.0f), FVector(0.2f, 3.2f, 6.0f), FRotator::ZeroRotator, WallColor);
    SpawnRoomPiece(FVector(360.0f, 430.0f, 540.0f), FVector(0.2f, 3.9f, 0.7f), FRotator::ZeroRotator, TrimColor);
    SpawnRoomPiece(FVector(910.0f, 1080.0f, 300.0f), FVector(5.1f, 0.2f, 6.0f), FRotator::ZeroRotator, WallColor);
    SpawnRoomPiece(FVector(1020.0f, 1080.0f, 120.0f), FVector(0.65f, 0.18f, 1.6f), FRotator::ZeroRotator, TrimColor);
    SpawnRoomPiece(FVector(720.0f, 1080.0f, 120.0f), FVector(0.65f, 0.18f, 1.6f), FRotator::ZeroRotator, TrimColor);

    SpawnZoneMarker(FVector(0.0f, -1080.0f, 8.0f), FVector(2.9f, 0.45f, 0.04f), TEXT("START - BODY"), FLinearColor(0.86f, 0.84f, 0.70f));
    SpawnZoneMarker(FVector(230.0f, 260.0f, 8.0f), FVector(3.2f, 0.34f, 0.04f), TEXT("CHAIN - NURSERY"), FLinearColor(0.86f, 0.70f, 0.28f));
    SpawnZoneMarker(FVector(-790.0f, -620.0f, 8.0f), FVector(3.0f, 0.32f, 0.04f), TEXT("BAIT - MIRROR THEATER"), FLinearColor(0.46f, 0.56f, 0.74f));
    SpawnZoneMarker(FVector(910.0f, -1190.0f, 8.0f), FVector(2.4f, 0.20f, 0.04f), TEXT("EXIT"), FLinearColor(0.62f, 0.90f, 0.72f));
}

void ATheLastRiteGameMode::BuildCaseContent()
{
    auto SpawnProp = [this](const FVector& Location, const FVector& Scale3D, const FText& Name, const FText& Clue, const FText& EvidenceSummary, bool bTrueClue, bool bOpeningSweepTarget = false)
    {
        AInspectableProp* Prop = GetWorld()->SpawnActor<AInspectableProp>(Location, FRotator::ZeroRotator);
        if (Prop != nullptr)
        {
            Prop->SetActorScale3D(Scale3D);
            Prop->ConfigureProp(Name, Clue, EvidenceSummary, bTrueClue, bOpeningSweepTarget);
            InspectableProps.Add(Prop);
        }
    };

    auto SpawnAnchor = [this](const FVector& Location, const FText& Name, bool bCorrectAnchor)
    {
        ARitualAnchor* Anchor = GetWorld()->SpawnActor<ARitualAnchor>(Location, FRotator::ZeroRotator);
        if (Anchor != nullptr)
        {
            Anchor->SetActorScale3D(FVector(1.8f, 1.8f, 0.2f));
            Anchor->ConfigureAnchor(Name, bCorrectAnchor);
            RitualAnchors.Add(Anchor);
        }
    };

    auto SpawnExit = [this](const FVector& Location, const FVector& Scale3D, const FText& Name)
    {
        CaseExitActor = GetWorld()->SpawnActor<ACaseExit>(Location, FRotator::ZeroRotator);
        if (CaseExitActor != nullptr)
        {
            CaseExitActor->SetActorScale3D(Scale3D);
            CaseExitActor->ConfigureExit(Name);
        }
    };

    auto SpawnTool = [this](const FVector& Location, const FVector& Scale3D, ETheLastRiteSessionTool ToolType, const FText& Name, const FText& RoleText, const FLinearColor& ToolColor)
    {
        ASessionToolPickup* ToolPickup = GetWorld()->SpawnActor<ASessionToolPickup>(Location, FRotator::ZeroRotator);
        if (ToolPickup != nullptr)
        {
            ToolPickup->SetActorScale3D(Scale3D);
            ToolPickup->ConfigureTool(ToolType, Name, RoleText, ToolColor);
            SessionToolPickups.Add(ToolPickup);
        }
    };

    SpawnProp(
        FVector(0.0f, -780.0f, 95.0f),
        FVector(0.7f, 0.7f, 1.9f),
        NSLOCTEXT("TheLastRite", "NannyName", "Nanny Eliza's body"),
        NSLOCTEXT("TheLastRite", "NannyClue", "Her dry wrist marks mirror each other too perfectly, like a saint pose forced onto skin."),
        NSLOCTEXT("TheLastRite", "NannyEvidence", "Nanny Eliza - mirrored wrist marks"),
        true,
        true);

    SpawnProp(
        FVector(430.0f, 250.0f, 60.0f),
        FVector(1.2f, 1.8f, 0.8f),
        NSLOCTEXT("TheLastRite", "CradleName", "the cradle"),
        NSLOCTEXT("TheLastRite", "CradleClue", "Ash-white handprints ring the cradle in a perfect halo."),
        NSLOCTEXT("TheLastRite", "CradleEvidence", "the cradle - halo of ash-white handprints"),
        true,
        true);

    SpawnProp(
        FVector(-480.0f, 320.0f, 40.0f),
        FVector(0.8f, 0.8f, 0.8f),
        NSLOCTEXT("TheLastRite", "PrayerCardsName", "the prayer mess"),
        NSLOCTEXT("TheLastRite", "PrayerCardsClue", "The prayer cards in the mess are fused into a crown-like circle."),
        NSLOCTEXT("TheLastRite", "PrayerCardsEvidence", "the prayer cards - fused into a crown"),
        true,
        true);

    SpawnProp(
        FVector(160.0f, -520.0f, 55.0f),
        FVector(0.5f, 0.5f, 1.1f),
        NSLOCTEXT("TheLastRite", "MonitorName", "the baby monitor"),
        NSLOCTEXT("TheLastRite", "MonitorClue", "The monitor hums the same hymn on every channel."),
        NSLOCTEXT("TheLastRite", "MonitorEvidence", "the baby monitor - hymn repeating on every channel"),
        true);

    SpawnProp(
        FVector(980.0f, 980.0f, 110.0f),
        FVector(1.2f, 0.12f, 1.7f),
        NSLOCTEXT("TheLastRite", "WallpaperName", "the nursery wallpaper"),
        NSLOCTEXT("TheLastRite", "WallpaperClue", "A child's sun mural has been repainted into a pale halo with tiny nailed eyes."),
        NSLOCTEXT("TheLastRite", "WallpaperEvidence", "the nursery wallpaper - child's sun turned into a halo"),
        true);

    SpawnProp(
        FVector(-520.0f, -450.0f, 70.0f),
        FVector(1.4f, 0.2f, 1.4f),
        NSLOCTEXT("TheLastRite", "WindowName", "the broken window latch"),
        NSLOCTEXT("TheLastRite", "WindowClue", "The latch is busted from outside. It is violence, not ritual."),
        NSLOCTEXT("TheLastRite", "WindowEvidence", "the broken window latch - forced from outside"),
        false);

    SpawnProp(
        FVector(620.0f, -180.0f, 35.0f),
        FVector(0.8f, 0.8f, 0.3f),
        NSLOCTEXT("TheLastRite", "TicketName", "the pawn ticket pouch"),
        NSLOCTEXT("TheLastRite", "TicketClue", "A pawn ticket pouch sits near the sink. It is ordinary greed."),
        NSLOCTEXT("TheLastRite", "TicketEvidence", "the pawn ticket pouch - ordinary greed"),
        false);

    SpawnProp(
        FVector(-980.0f, -120.0f, 55.0f),
        FVector(0.15f, 0.9f, 0.15f),
        NSLOCTEXT("TheLastRite", "KnifeName", "the kitchen knife"),
        NSLOCTEXT("TheLastRite", "KnifeClue", "The kitchen knife is filthy, but the stains are cooking grease, not offering blood."),
        NSLOCTEXT("TheLastRite", "KnifeEvidence", "the kitchen knife - grease, not offering blood"),
        false);

    SpawnTool(
        FVector(140.0f, -920.0f, 56.0f),
        FVector(0.42f, 0.26f, 0.10f),
        ETheLastRiteSessionTool::HolyBook,
        NSLOCTEXT("TheLastRite", "HolyBookName", "the Holy Book"),
        NSLOCTEXT("TheLastRite", "HolyBookRole", "Reader"),
        FLinearColor(0.78f, 0.70f, 0.28f));

    SpawnTool(
        FVector(320.0f, -400.0f, 62.0f),
        FVector(0.34f, 0.22f, 0.16f),
        ETheLastRiteSessionTool::FilmCamera,
        NSLOCTEXT("TheLastRite", "FilmCameraName", "the Film Camera"),
        NSLOCTEXT("TheLastRite", "FilmCameraRole", "Camera"),
        FLinearColor(0.42f, 0.66f, 0.92f));

    SpawnTool(
        FVector(540.0f, 540.0f, 62.0f),
        FVector(0.22f, 0.22f, 0.52f),
        ETheLastRiteSessionTool::Censer,
        NSLOCTEXT("TheLastRite", "CenserName", "the Censer"),
        NSLOCTEXT("TheLastRite", "CenserRole", "Censer"),
        FLinearColor(0.90f, 0.84f, 0.52f));

    SpawnTool(
        FVector(-1090.0f, -520.0f, 70.0f),
        FVector(0.16f, 0.12f, 0.64f),
        ETheLastRiteSessionTool::Crucifix,
        NSLOCTEXT("TheLastRite", "CrucifixName", "the Crucifix"),
        NSLOCTEXT("TheLastRite", "CrucifixRole", "Guard"),
        FLinearColor(0.72f, 0.52f, 0.34f));

    SpawnAnchor(
        FVector(730.0f, 620.0f, 20.0f),
        NSLOCTEXT("TheLastRite", "CorrectAnchor", "the nursery sigil"),
        true);

    SpawnAnchor(
        FVector(-730.0f, -620.0f, 20.0f),
        NSLOCTEXT("TheLastRite", "WrongAnchor", "the mirror circle"),
        false);

    SpawnExit(
        FVector(910.0f, -1390.0f, 118.0f),
        FVector(1.2f, 0.16f, 2.2f),
        NSLOCTEXT("TheLastRite", "FrontDoorName", "the front door"));
}

void ATheLastRiteGameMode::BuildSetDressing()
{
    const FLinearColor FurnitureColor(0.42f, 0.31f, 0.23f);
    const FLinearColor PaleColor(0.65f, 0.68f, 0.72f);
    const FLinearColor ClothColor(0.12f, 0.11f, 0.13f);
    const FLinearColor OldGoldColor(0.76f, 0.61f, 0.22f);
    const FLinearColor BloodlessSkinColor(0.68f, 0.66f, 0.62f);
    const FLinearColor MirrorColor(0.22f, 0.26f, 0.31f);
    const FLinearColor RunnerColor(0.40f, 0.08f, 0.08f);
    const FLinearColor NurseryGlowColor(0.54f, 0.48f, 0.18f);
    const FLinearColor TileColor(0.26f, 0.28f, 0.31f);
    const FLinearColor ExitGuideColor(0.18f, 0.42f, 0.26f);
    const FLinearColor BodyGuideColor(0.50f, 0.46f, 0.30f);
    const FLinearColor MirrorBaitColor(0.22f, 0.34f, 0.46f);

    SpawnRoomPiece(FVector(0.0f, -780.0f, 18.0f), FVector(2.7f, 1.2f, 0.08f), FRotator::ZeroRotator, ClothColor);
    SpawnRoomPiece(FVector(-52.0f, -780.0f, 65.0f), FVector(0.35f, 0.45f, 0.62f), FRotator::ZeroRotator, BloodlessSkinColor);
    SpawnRoomPiece(FVector(28.0f, -780.0f, 50.0f), FVector(0.75f, 0.36f, 0.16f), FRotator::ZeroRotator, BloodlessSkinColor);
    SpawnRoomPiece(FVector(0.0f, -780.0f, 35.0f), FVector(2.0f, 0.04f, 0.04f), FRotator::ZeroRotator, OldGoldColor);
    SpawnRoomPiece(FVector(0.0f, -780.0f, 38.0f), FVector(2.0f, 0.04f, 0.04f), FRotator(0.0f, 45.0f, 0.0f), OldGoldColor);
    SpawnRoomPiece(FVector(0.0f, -780.0f, 41.0f), FVector(2.0f, 0.04f, 0.04f), FRotator(0.0f, 90.0f, 0.0f), OldGoldColor);
    SpawnRoomPiece(FVector(0.0f, -780.0f, 44.0f), FVector(2.0f, 0.04f, 0.04f), FRotator(0.0f, 135.0f, 0.0f), OldGoldColor);

    SpawnRoomPiece(FVector(860.0f, 620.0f, 110.0f), FVector(1.8f, 0.9f, 1.1f), FRotator::ZeroRotator, FurnitureColor);
    SpawnRoomPiece(FVector(860.0f, 860.0f, 80.0f), FVector(0.8f, 0.8f, 0.8f), FRotator::ZeroRotator, PaleColor);
    SpawnRoomPiece(FVector(-920.0f, 780.0f, 145.0f), FVector(0.6f, 3.6f, 2.9f), FRotator::ZeroRotator, FurnitureColor);
    SpawnRoomPiece(FVector(-780.0f, -830.0f, 110.0f), FVector(2.4f, 0.8f, 1.1f), FRotator::ZeroRotator, FurnitureColor);
    SpawnRoomPiece(FVector(-860.0f, -520.0f, 55.0f), FVector(1.0f, 1.0f, 0.5f), FRotator::ZeroRotator, PaleColor);
    SpawnRoomPiece(FVector(0.0f, 980.0f, 145.0f), FVector(4.4f, 0.15f, 2.9f), FRotator::ZeroRotator, PaleColor);
    SpawnRoomPiece(FVector(420.0f, 980.0f, 145.0f), FVector(0.15f, 1.2f, 2.9f), FRotator::ZeroRotator, PaleColor);
    SpawnRoomPiece(FVector(-250.0f, -920.0f, 60.0f), FVector(1.1f, 1.1f, 0.5f), FRotator::ZeroRotator, FurnitureColor);
    SpawnRoomPiece(FVector(-1330.0f, 0.0f, 150.0f), FVector(0.08f, 2.1f, 3.0f), FRotator::ZeroRotator, MirrorColor);
    SpawnRoomPiece(FVector(-680.0f, -180.0f, 8.0f), FVector(5.6f, 2.8f, 0.04f), FRotator::ZeroRotator, TileColor);
    SpawnRoomPiece(FVector(130.0f, 260.0f, 8.0f), FVector(1.2f, 7.4f, 0.04f), FRotator::ZeroRotator, RunnerColor);
    SpawnRoomPiece(FVector(610.0f, -960.0f, 8.0f), FVector(2.6f, 4.0f, 0.04f), FRotator::ZeroRotator, RunnerColor);
    SpawnRoomPiece(FVector(910.0f, -1230.0f, 8.0f), FVector(2.4f, 0.24f, 0.05f), FRotator::ZeroRotator, OldGoldColor);
    SpawnRoomPiece(FVector(0.0f, -1080.0f, 8.0f), FVector(2.8f, 0.26f, 0.04f), FRotator::ZeroRotator, BodyGuideColor);
    SpawnRoomPiece(FVector(180.0f, -240.0f, 8.0f), FVector(0.28f, 5.5f, 0.04f), FRotator::ZeroRotator, BodyGuideColor);
    SpawnRoomPiece(FVector(500.0f, 620.0f, 8.0f), FVector(2.9f, 0.22f, 0.04f), FRotator::ZeroRotator, NurseryGlowColor);
    SpawnRoomPiece(FVector(760.0f, 980.0f, 8.0f), FVector(1.6f, 0.16f, 0.04f), FRotator::ZeroRotator, NurseryGlowColor);
    SpawnRoomPiece(FVector(-980.0f, -620.0f, 8.0f), FVector(2.0f, 0.18f, 0.04f), FRotator::ZeroRotator, MirrorBaitColor);
    SpawnRoomPiece(FVector(910.0f, -1080.0f, 8.0f), FVector(0.18f, 2.2f, 0.04f), FRotator::ZeroRotator, ExitGuideColor);
    SpawnRoomPiece(FVector(780.0f, 760.0f, 8.0f), FVector(3.0f, 2.8f, 0.04f), FRotator::ZeroRotator, NurseryGlowColor);
    SpawnRoomPiece(FVector(940.0f, 730.0f, 215.0f), FVector(0.12f, 0.12f, 2.0f), FRotator::ZeroRotator, OldGoldColor);
    SpawnRoomPiece(FVector(780.0f, 730.0f, 215.0f), FVector(0.12f, 0.12f, 2.0f), FRotator::ZeroRotator, OldGoldColor);
    SpawnRoomPiece(FVector(860.0f, 730.0f, 215.0f), FVector(0.95f, 0.08f, 0.08f), FRotator::ZeroRotator, OldGoldColor);
    SpawnRoomPiece(FVector(860.0f, 1040.0f, 185.0f), FVector(0.28f, 0.10f, 2.0f), FRotator::ZeroRotator, NurseryGlowColor);
    SpawnRoomPiece(FVector(-900.0f, -320.0f, 165.0f), FVector(0.20f, 0.10f, 1.6f), FRotator::ZeroRotator, MirrorBaitColor);
    SpawnRoomPiece(FVector(910.0f, -1360.0f, 120.0f), FVector(1.45f, 0.1f, 2.35f), FRotator::ZeroRotator, FurnitureColor);
    SpawnRoomPiece(FVector(1090.0f, -1360.0f, 120.0f), FVector(0.12f, 0.08f, 0.12f), FRotator::ZeroRotator, OldGoldColor);
    SpawnRoomPiece(FVector(910.0f, -1160.0f, 150.0f), FVector(0.16f, 0.10f, 1.5f), FRotator::ZeroRotator, ExitGuideColor);
    SpawnRoomPiece(FVector(730.0f, 620.0f, 30.0f), FVector(2.9f, 0.05f, 0.05f), FRotator::ZeroRotator, OldGoldColor);
    SpawnRoomPiece(FVector(730.0f, 620.0f, 34.0f), FVector(2.9f, 0.05f, 0.05f), FRotator(0.0f, 90.0f, 0.0f), OldGoldColor);
    SpawnRoomPiece(FVector(-730.0f, -620.0f, 30.0f), FVector(2.9f, 0.05f, 0.05f), FRotator(0.0f, 45.0f, 0.0f), MirrorColor);
    SpawnRoomPiece(FVector(-730.0f, -620.0f, 34.0f), FVector(2.9f, 0.05f, 0.05f), FRotator(0.0f, 135.0f, 0.0f), MirrorColor);
}

void ATheLastRiteGameMode::SpawnRoomPiece(const FVector& Location, const FVector& Scale3D, const FRotator& Rotation, const FLinearColor& Color)
{
    AStaticMeshActor* Piece = GetWorld()->SpawnActor<AStaticMeshActor>(Location, Rotation);
    if (Piece == nullptr)
    {
        return;
    }

    UStaticMeshComponent* MeshComponent = Piece->GetStaticMeshComponent();
    if (MeshComponent == nullptr)
    {
        return;
    }

    if (UStaticMesh* CubeMesh = LoadBasicMesh(TEXT("/Engine/BasicShapes/Cube.Cube")))
    {
        MeshComponent->SetStaticMesh(CubeMesh);
    }

    if (UMaterialInterface* BaseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")))
    {
        UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, Piece);
        if (DynamicMaterial != nullptr)
        {
            DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
            MeshComponent->SetMaterial(0, DynamicMaterial);
        }
    }

    Piece->SetActorScale3D(Scale3D);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
}

void ATheLastRiteGameMode::SpawnZoneMarker(const FVector& Location, const FVector& PlateScale3D, const FString& Label, const FLinearColor& Color, float TextYaw)
{
    AActor* Marker = GetWorld()->SpawnActor<AActor>(Location, FRotator::ZeroRotator);
    if (Marker == nullptr)
    {
        return;
    }

    USceneComponent* Root = NewObject<USceneComponent>(Marker, TEXT("ZoneMarkerRoot"));
    if (Root == nullptr)
    {
        return;
    }

    Marker->SetRootComponent(Root);
    Root->RegisterComponent();

    UStaticMeshComponent* Plate = NewObject<UStaticMeshComponent>(Marker, TEXT("ZoneMarkerPlate"));
    if (Plate != nullptr)
    {
        Plate->SetupAttachment(Root);
        Plate->RegisterComponent();
        if (UStaticMesh* CubeMesh = LoadBasicMesh(TEXT("/Engine/BasicShapes/Cube.Cube")))
        {
            Plate->SetStaticMesh(CubeMesh);
        }

        if (UMaterialInterface* BaseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")))
        {
            UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, Marker);
            if (DynamicMaterial != nullptr)
            {
                DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
                Plate->SetMaterial(0, DynamicMaterial);
            }
        }

        Plate->SetWorldScale3D(PlateScale3D);
        Plate->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    UTextRenderComponent* Text = NewObject<UTextRenderComponent>(Marker, TEXT("ZoneMarkerText"));
    if (Text != nullptr)
    {
        Text->SetupAttachment(Root);
        Text->RegisterComponent();
        Text->SetHorizontalAlignment(EHTA_Center);
        Text->SetVerticalAlignment(EVRTA_TextCenter);
        Text->SetWorldSize(82.0f);
        Text->SetText(FText::FromString(Label));
        Text->SetTextRenderColor(Color.ToFColor(true));
        Text->SetRelativeLocation(FVector(0.0f, 0.0f, 28.0f));
        Text->SetRelativeRotation(FRotator(90.0f, TextYaw, 0.0f));
    }
}

void ATheLastRiteGameMode::SpawnLights()
{
    APointLight* CenterLight = GetWorld()->SpawnActor<APointLight>(FVector(0.0f, 0.0f, 480.0f), FRotator::ZeroRotator);
    if (CenterLight != nullptr)
    {
        CaseLights.Add(CenterLight);
        CenterLight->SetActorScale3D(FVector(6.0f, 6.0f, 6.0f));
        if (UPointLightComponent* LightComponent = Cast<UPointLightComponent>(CenterLight->GetLightComponent()))
        {
            LightComponent->Intensity = 18000.0f;
            LightComponent->AttenuationRadius = 2500.0f;
        }
    }

    APointLight* NurseryLight = GetWorld()->SpawnActor<APointLight>(FVector(700.0f, 650.0f, 220.0f), FRotator::ZeroRotator);
    if (NurseryLight != nullptr)
    {
        CaseLights.Add(NurseryLight);
        NurseryLight->SetActorScale3D(FVector(3.0f, 3.0f, 3.0f));
        if (UPointLightComponent* LightComponent = Cast<UPointLightComponent>(NurseryLight->GetLightComponent()))
        {
            LightComponent->Intensity = 8000.0f;
            LightComponent->AttenuationRadius = 1200.0f;
        }
    }

    APointLight* MirrorLight = GetWorld()->SpawnActor<APointLight>(FVector(-780.0f, -640.0f, 220.0f), FRotator::ZeroRotator);
    if (MirrorLight != nullptr)
    {
        CaseLights.Add(MirrorLight);
        MirrorLight->SetActorScale3D(FVector(3.0f, 3.0f, 3.0f));
        if (UPointLightComponent* LightComponent = Cast<UPointLightComponent>(MirrorLight->GetLightComponent()))
        {
            LightComponent->Intensity = 5200.0f;
            LightComponent->AttenuationRadius = 1100.0f;
        }
    }
}

void ATheLastRiteGameMode::UpdateRitualAnchors()
{
    const bool bRitualReady = CasePhase == ETheLastRiteCasePhase::RiteReady
        || CasePhase == ETheLastRiteCasePhase::RitualInProgress;
    for (ARitualAnchor* Anchor : RitualAnchors)
    {
        if (Anchor != nullptr)
        {
            Anchor->SetRitualState(bRitualReady, IsRitualSequenceActive(), Anchor == ActiveRitualAnchor);
        }
    }
}

void ATheLastRiteGameMode::UpdateCaseExit()
{
    if (CaseExitActor != nullptr)
    {
        const bool bExitReady = CasePhase == ETheLastRiteCasePhase::SealedAwaitingExit;
        CaseExitActor->SetExitReady(bExitReady);
    }
}

void ATheLastRiteGameMode::UpdateCasePhaseFromEvidence()
{
    if (IsCaseResolved())
    {
        return;
    }

    if (CasePhase == ETheLastRiteCasePhase::RitualInProgress)
    {
        return;
    }

    const ETheLastRiteCasePhase PreviousPhase = CasePhase;
    CasePhase = FoundTrueClues >= RequiredTrueClues
        ? ETheLastRiteCasePhase::RiteReady
        : ETheLastRiteCasePhase::Investigating;

    if (CasePhase != ETheLastRiteCasePhase::RiteReady)
    {
        CompletedRitualHandSteps = 0;
        ActiveRitualAnchor = nullptr;
    }

    if (CasePhase != PreviousPhase)
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("GP-C1 PHASE_MARKER previous=\"%s\" current=\"%s\" true_clues=%d/%d false_leads=%d/%d"),
            GetCasePhaseMarkerName(PreviousPhase),
            GetCasePhaseMarkerName(CasePhase),
            FoundTrueClues,
            RequiredTrueClues,
            FoundFalseLeads,
            TotalFalseLeads);

        if (CasePhase == ETheLastRiteCasePhase::RiteReady)
        {
            UE_LOG(
                LogTemp,
                Display,
                TEXT("GP-C1 SOLO_PLACEHOLDER marker=\"ready\" support=\"%s\""),
                *GetRitualSequenceSupportText().ToString());
        }
    }
}

void ATheLastRiteGameMode::RunAutomatedRitualVerification()
{
    UE_LOG(
        LogTemp,
        Display,
        TEXT("GP-C1 AUTO_VERIFY marker=\"start\" initial_phase=\"%s\""),
        GetCasePhaseMarkerName(CasePhase));

    const ETheLastRiteSessionTool RequiredTools[] =
    {
        ETheLastRiteSessionTool::HolyBook,
        ETheLastRiteSessionTool::FilmCamera,
        ETheLastRiteSessionTool::Censer,
        ETheLastRiteSessionTool::Crucifix
    };

    for (ETheLastRiteSessionTool ToolType : RequiredTools)
    {
        ASessionToolPickup* ToolPickup = FindSessionToolPickupByType(SessionToolPickups, ToolType);
        if (ToolPickup == nullptr)
        {
            UE_LOG(
                LogTemp,
                Error,
                TEXT("GP-C2 AUTO_VERIFY marker=\"missing_tool\" tool=\"%s\""),
                GetSessionToolMarkerName(ToolType));
            return;
        }

        if (!ToolPickup->IsClaimed())
        {
            HandleToolPickup(ToolPickup);
        }
    }

    const TCHAR* RequiredEvidenceSummaries[] =
    {
        TEXT("Nanny Eliza - mirrored wrist marks"),
        TEXT("the cradle - halo of ash-white handprints"),
        TEXT("the prayer cards - fused into a crown"),
        TEXT("the baby monitor - hymn repeating on every channel"),
        TEXT("the nursery wallpaper - child's sun turned into a halo")
    };

    for (const TCHAR* RequiredEvidenceSummary : RequiredEvidenceSummaries)
    {
        AInspectableProp* Prop = FindInspectablePropByEvidenceSummary(RequiredEvidenceSummary);
        if (Prop == nullptr)
        {
            UE_LOG(
                LogTemp,
                Error,
                TEXT("GP-C1 AUTO_VERIFY marker=\"missing_prop\" evidence_summary=\"%s\""),
                RequiredEvidenceSummary);
            return;
        }

        if (!Prop->WasInspected())
        {
            HandleInspectableProp(Prop);
        }

        UE_LOG(
            LogTemp,
            Display,
            TEXT("GP-C1 AUTO_VERIFY marker=\"true_clue_confirmed\" evidence_summary=\"%s\" phase=\"%s\""),
            RequiredEvidenceSummary,
            GetCasePhaseMarkerName(CasePhase));
    }

    ARitualAnchor* CorrectAnchor = FindCorrectRitualAnchor();
    if (CorrectAnchor == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("GP-C1 AUTO_VERIFY marker=\"missing_correct_anchor\""));
        return;
    }

    for (int32 StepIndex = 0; StepIndex < RitualHandStepCount && !IsCaseResolved(); ++StepIndex)
    {
        HandleRitualAnchor(CorrectAnchor);
    }

    if (CasePhase == ETheLastRiteCasePhase::SealedAwaitingExit && CaseExitActor != nullptr)
    {
        HandleCaseExit(CaseExitActor);
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("GP-C1 AUTO_VERIFY marker=\"complete\" final_phase=\"%s\" true_clues=%d/%d completed_steps=%d exit_recorded=%s"),
        GetCasePhaseMarkerName(CasePhase),
        FoundTrueClues,
        RequiredTrueClues,
        CompletedRitualHandSteps,
        HasEvidenceLine(TEXT("EXIT - front door secured")) ? TEXT("true") : TEXT("false"));

    FPlatformMisc::RequestExit(false);
}

AInspectableProp* ATheLastRiteGameMode::FindInspectablePropByEvidenceSummary(const FString& EvidenceSummary) const
{
    for (AInspectableProp* Prop : InspectableProps)
    {
        if (Prop != nullptr && Prop->GetEvidenceSummary().ToString().Equals(EvidenceSummary, ESearchCase::CaseSensitive))
        {
            return Prop;
        }
    }

    return nullptr;
}

ARitualAnchor* ATheLastRiteGameMode::FindCorrectRitualAnchor() const
{
    for (ARitualAnchor* Anchor : RitualAnchors)
    {
        if (Anchor != nullptr && Anchor->IsCorrectAnchor())
        {
            return Anchor;
        }
    }

    return nullptr;
}

void ATheLastRiteGameMode::UpdateWorldMood()
{
    FLinearColor LightColor(0.96f, 0.92f, 0.82f);
    float IntensityScale = 1.0f;

    switch (CasePhase)
    {
    case ETheLastRiteCasePhase::ClosedFail:
        LightColor = FLinearColor(1.0f, 0.20f, 0.14f);
        IntensityScale = 0.48f;
        break;
    case ETheLastRiteCasePhase::SealedAwaitingExit:
        LightColor = FLinearColor(0.65f, 1.0f, 0.72f);
        IntensityScale = 1.15f;
        break;
    case ETheLastRiteCasePhase::ClosedWin:
        LightColor = FLinearColor(0.86f, 0.96f, 0.78f);
        IntensityScale = 1.05f;
        break;
    case ETheLastRiteCasePhase::RiteReady:
        LightColor = FLinearColor(1.0f, 0.82f, 0.30f);
        IntensityScale = 1.05f;
        break;
    case ETheLastRiteCasePhase::RitualInProgress:
        LightColor = FLinearColor(0.94f, 0.78f, 0.36f);
        IntensityScale = 1.12f;
        break;
    case ETheLastRiteCasePhase::Investigating:
    default:
    {
        const float ClueProgress = RequiredTrueClues > 0 ? static_cast<float>(FoundTrueClues) / static_cast<float>(RequiredTrueClues) : 0.0f;
        LightColor = FMath::Lerp(FLinearColor(0.96f, 0.92f, 0.82f), FLinearColor(1.0f, 0.78f, 0.30f), ClueProgress);
        IntensityScale = FMath::Clamp(1.0f - (static_cast<float>(FoundFalseLeads) * 0.16f), 0.62f, 1.15f);
        break;
    }
    }

    for (int32 Index = 0; Index < CaseLights.Num(); ++Index)
    {
        APointLight* Light = CaseLights[Index];
        if (Light == nullptr)
        {
            continue;
        }

        if (UPointLightComponent* LightComponent = Cast<UPointLightComponent>(Light->GetLightComponent()))
        {
            const float BaseIntensity = Index == 0 ? 18000.0f : (Index == 1 ? 8000.0f : 5200.0f);
            LightComponent->SetIntensity(BaseIntensity * IntensityScale);
            LightComponent->SetLightColor(LightColor);
        }
    }
}

void ATheLastRiteGameMode::UpdateProgressText()
{
    const FText ToolRoleState = FText::Format(
        NSLOCTEXT("TheLastRite", "ToolProgressState", "Roles: {0}/4 claimed"),
        FText::AsNumber(CountClaimedToolPickups(SessionToolPickups)));

    switch (CasePhase)
    {
    case ETheLastRiteCasePhase::SealedAwaitingExit:
        ProgressText = NSLOCTEXT("TheLastRite", "ProgressExitReady", "Case status: SEALED | Leave through the front door and file the case");
        return;
    case ETheLastRiteCasePhase::ClosedWin:
        ProgressText = NSLOCTEXT("TheLastRite", "ProgressWon", "Case status: CLOSED | Press R to restart the case | Esc to quit");
        return;
    case ETheLastRiteCasePhase::ClosedFail:
        ProgressText = NSLOCTEXT("TheLastRite", "ProgressLost", "Case status: FAILED | Press R to restart the case | Esc to quit");
        return;
    case ETheLastRiteCasePhase::RiteReady:
        ProgressText = FText::Format(
            NSLOCTEXT("TheLastRite", "ProgressRiteReady", "True clues: {0}/{1} | False leads: {2}/{3} | {4} | {5}"),
            FText::AsNumber(FoundTrueClues),
            FText::AsNumber(RequiredTrueClues),
            FText::AsNumber(FoundFalseLeads),
            FText::AsNumber(TotalFalseLeads),
            GetRitualSequenceStateText(),
            ToolRoleState);
        return;
    case ETheLastRiteCasePhase::RitualInProgress:
        ProgressText = FText::Format(
            NSLOCTEXT("TheLastRite", "ProgressRitualInProgress", "True clues: {0}/{1} | False leads: {2}/{3} | {4} | {5}"),
            FText::AsNumber(FoundTrueClues),
            FText::AsNumber(RequiredTrueClues),
            FText::AsNumber(FoundFalseLeads),
            FText::AsNumber(TotalFalseLeads),
            GetRitualSequenceStateText(),
            ToolRoleState);
        return;
    case ETheLastRiteCasePhase::Investigating:
    default:
        break;
    }

    const FText RiteState = FText::Format(
        NSLOCTEXT("TheLastRite", "RiteLocked", "Rite locked: {0} clue(s) left"),
        FText::AsNumber(RequiredTrueClues - FoundTrueClues));

    ProgressText = FText::Format(
        NSLOCTEXT("TheLastRite", "Progress", "True clues: {0}/{1} | False leads: {2}/{3} | {4} | {5} | {6}"),
        FText::AsNumber(FoundTrueClues),
        FText::AsNumber(RequiredTrueClues),
        FText::AsNumber(FoundFalseLeads),
        FText::AsNumber(TotalFalseLeads),
        GetOpeningSweepStateText(),
        RiteState,
        ToolRoleState);
}

void ATheLastRiteGameMode::UpdateDeductionText()
{
    if (CasePhase == ETheLastRiteCasePhase::SealedAwaitingExit
        || CasePhase == ETheLastRiteCasePhase::ClosedWin
        || CasePhase == ETheLastRiteCasePhase::ClosedFail)
    {
        return;
    }

    if (CasePhase == ETheLastRiteCasePhase::RitualInProgress)
    {
        DeductionText = FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "DeductionRitualInProgress",
                "Read of the room: the theory is already closed. Hold the nursery sigil through the {0} hand, then pass to {1}."),
            GetCurrentRitualHandText(),
            GetNextRitualHandText());
        return;
    }

    if (FoundTrueClues <= 0)
    {
        DeductionText = NSLOCTEXT(
            "TheLastRite",
            "DeductionZero",
            "Read of the room: pattern unclear. Build the first read in order: body -> cradle -> prayer mess.");
        return;
    }

    if (FoundTrueClues == 1)
    {
        DeductionText = NSLOCTEXT(
            "TheLastRite",
            "DeductionOne",
            "Read of the room: the body looks staged. Push into the CHAIN - NURSERY side and see whether the cradle repeats that same false sanctity.");
        return;
    }

    if (FoundTrueClues == 2)
    {
        DeductionText = NSLOCTEXT(
            "TheLastRite",
            "DeductionTwo",
            "Read of the room: body and cradle already agree. Finish the prayer mess before you let louder bait compete for the read.");
        return;
    }

    if (FoundTrueClues == 3)
    {
        DeductionText = FoundFalseLeads > 0
            ? NSLOCTEXT(
                "TheLastRite",
                "DeductionThreeFalse",
                "Read of the room: body, cradle, and prayer mess all repeat the same false sanctity around the nursery. The ugly damage is noise, not a rival center.")
            : NSLOCTEXT(
                "TheLastRite",
                "DeductionThree",
                "Read of the room: body, cradle, and prayer mess are now agreeing around the nursery side. The mirror still has spectacle, but not a matching chain.");
        return;
    }

    if (FoundTrueClues == 4)
    {
        DeductionText = NSLOCTEXT(
            "TheLastRite",
            "DeductionFour",
            "Read of the room: the nursery chain has reached the monitor. The room's voice is siding child-side too, leaving the mirror with theater instead of proof.");
        return;
    }

    DeductionText = NSLOCTEXT(
        "TheLastRite",
        "DeductionComplete",
        "Read of the room: conclusion locked. The true altar is child-facing. The mirror is bait.");
}

void ATheLastRiteGameMode::UpdateRitualReadText()
{
    const bool bFoundMonitor = HasEvidenceLine(TEXT("TRUE - the baby monitor - hymn repeating on every channel"));
    const bool bFoundWallpaper = HasEvidenceLine(TEXT("TRUE - the nursery wallpaper - child's sun turned into a halo"));

    switch (CasePhase)
    {
    case ETheLastRiteCasePhase::SealedAwaitingExit:
        RitualReadText = NSLOCTEXT(
            "TheLastRite",
            "RitualReadResolvedExit",
            "Ritual read: nursery sigil was correct. The seal holds. Leave through the front door now.");
        return;
    case ETheLastRiteCasePhase::ClosedWin:
        RitualReadText = NSLOCTEXT(
            "TheLastRite",
            "RitualReadClosedWin",
            "Ritual read: nursery sigil was correct. Apartment 302 is sealed.");
        return;
    case ETheLastRiteCasePhase::ClosedFail:
        RitualReadText = NSLOCTEXT(
            "TheLastRite",
            "RitualReadClosedFail",
            "Ritual read: the mirror circle was bait. The nursery sigil was the right anchor.");
        return;
    case ETheLastRiteCasePhase::RiteReady:
        RitualReadText = NSLOCTEXT(
            "TheLastRite",
            "RitualReadReady",
            "Ritual read: the nursery sigil is confirmed. Start the 4-hand rite there; the mirror only copied the shape.");
        return;
    case ETheLastRiteCasePhase::RitualInProgress:
        RitualReadText = FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "RitualReadInProgress",
                "Ritual read: stay on the nursery sigil. {0} is active now, and the prototype solo pass can cycle every hand on the same anchor."),
            GetCurrentRitualHandText());
        return;
    case ETheLastRiteCasePhase::Investigating:
    default:
        break;
    }

    if (GetOpeningSweepCount() < 3)
    {
        RitualReadText = NSLOCTEXT(
            "TheLastRite",
            "RitualReadOpeningSweep",
            "Ritual read: unresolved. Finish body -> cradle -> prayer mess before trusting any altar.");
        return;
    }

    if (!bFoundMonitor)
    {
        RitualReadText = NSLOCTEXT(
            "TheLastRite",
            "RitualReadMonitor",
            "Ritual read: body, cradle, and prayer mess already point child-side. Check whether the monitor hymn joins that same chain.");
        return;
    }

    if (!bFoundWallpaper)
    {
        RitualReadText = NSLOCTEXT(
            "TheLastRite",
            "RitualReadWallpaper",
            "Ritual read: the hymn already backs the nursery read. Confirm the mural halo and the mirror should lose the logic war cleanly.");
        return;
    }

    RitualReadText = NSLOCTEXT(
        "TheLastRite",
        "RitualReadLate",
        "Ritual read: the nursery sigil is leading because every real clue keeps resolving there. The mirror still has staging, not corroboration.");
}

void ATheLastRiteGameMode::UpdateNextMoveText()
{
    const bool bFoundNanny = HasEvidenceLine(TEXT("TRUE - Nanny Eliza - mirrored wrist marks"));
    const bool bFoundCradle = HasEvidenceLine(TEXT("TRUE - the cradle - halo of ash-white handprints"));
    const bool bFoundPrayerCards = HasEvidenceLine(TEXT("TRUE - the prayer cards - fused into a crown"));
    const bool bFoundMonitor = HasEvidenceLine(TEXT("TRUE - the baby monitor - hymn repeating on every channel"));
    const bool bFoundWallpaper = HasEvidenceLine(TEXT("TRUE - the nursery wallpaper - child's sun turned into a halo"));
    const bool bFoundWindow = HasEvidenceLine(TEXT("FALSE - the broken window latch - forced from outside"));
    const bool bFoundTicket = HasEvidenceLine(TEXT("FALSE - the pawn ticket pouch - ordinary greed"));
    const bool bFoundKnife = HasEvidenceLine(TEXT("FALSE - the kitchen knife - grease, not offering blood"));

    switch (CasePhase)
    {
    case ETheLastRiteCasePhase::SealedAwaitingExit:
        NextMoveText = NSLOCTEXT(
            "TheLastRite",
            "NextMoveExit",
            "The seal is holding. Leave through the front door and file the case.");
        return;
    case ETheLastRiteCasePhase::ClosedWin:
        NextMoveText = NSLOCTEXT(
            "TheLastRite",
            "NextMoveClosedWin",
            "Case closed. Press R to restart the case, or Esc to quit.");
        return;
    case ETheLastRiteCasePhase::ClosedFail:
        NextMoveText = NSLOCTEXT(
            "TheLastRite",
            "NextMoveClosedFail",
            "Press R to restart the case, or Esc to quit. On the next run, stay with the child-facing pattern instead of the mirror bait.");
        return;
    case ETheLastRiteCasePhase::RiteReady:
        NextMoveText = NSLOCTEXT(
            "TheLastRite",
            "NextMoveRiteReady",
            "The evidence is enough. Start the 4-hand rite at the nursery sigil, where the chain actually ends. Reject the mirror circle's imitation.");
        return;
    case ETheLastRiteCasePhase::RitualInProgress:
        NextMoveText = FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "NextMoveRitualInProgress",
                "Continue the 4-hand rite at the nursery sigil. {0} is active now; {1} comes after it. Do not break to the mirror."),
            GetCurrentRitualHandText(),
            GetNextRitualHandText());
        return;
    case ETheLastRiteCasePhase::Investigating:
    default:
        break;
    }

    if (!bFoundNanny)
    {
        NextMoveText = NSLOCTEXT(
            "TheLastRite",
            "NextMoveZero",
            "Opening route: body -> cradle -> prayer mess. Inspect Nanny Eliza's body first.");
        return;
    }

    if (!bFoundCradle)
    {
        NextMoveText = NSLOCTEXT(
            "TheLastRite",
            "NextMoveCradle",
            "Body logged. Turn into the CHAIN - NURSERY side and inspect the cradle next. Prayer mess comes after it. Ignore louder bait for now.");
        return;
    }

    if (!bFoundPrayerCards)
    {
        NextMoveText = NSLOCTEXT(
            "TheLastRite",
            "NextMovePrayerCards",
            "Cradle logged. Finish the opening route at the prayer mess. Then the rest of the room can compete for the read.");
        return;
    }

    if (!bFoundMonitor)
    {
        NextMoveText = (bFoundWindow || bFoundTicket || bFoundKnife)
            ? NSLOCTEXT(
                "TheLastRite",
                "NextMoveMonitorAfterFalseLead",
                "You have seen enough bait. Return to the nursery chain and inspect the baby monitor. If the hymn loops there too, the mirror loses ground.")
            : NSLOCTEXT(
                "TheLastRite",
                "NextMoveMonitor",
                "Opening sweep complete. Inspect the baby monitor next. If the room's voice also points child-side, the mirror keeps only spectacle.");
        return;
    }

    if (!bFoundWallpaper)
    {
        NextMoveText = NSLOCTEXT(
            "TheLastRite",
            "NextMoveWallpaper",
            "The monitor backed the nursery read. Inspect the mural next and see whether the room's visual center lands there too.");
        return;
    }

    NextMoveText = FoundFalseLeads > 0
        ? NSLOCTEXT(
            "TheLastRite",
            "NextMoveLateWithFalseLead",
            "The read is complete. The mirror had noise, but the nursery had the chain. Use the nursery sigil when you are ready to seal the room.")
        : NSLOCTEXT(
            "TheLastRite",
            "NextMoveLate",
            "All five true signs agree. The nursery chain won the room, so commit to the nursery sigil, not the mirror circle.");
}

void ATheLastRiteGameMode::UpdateFocusZoneText()
{
    const bool bFoundNanny = HasEvidenceLine(TEXT("TRUE - Nanny Eliza - mirrored wrist marks"));
    const bool bFoundCradle = HasEvidenceLine(TEXT("TRUE - the cradle - halo of ash-white handprints"));
    const bool bFoundPrayerCards = HasEvidenceLine(TEXT("TRUE - the prayer cards - fused into a crown"));
    const bool bFoundMonitor = HasEvidenceLine(TEXT("TRUE - the baby monitor - hymn repeating on every channel"));
    const bool bFoundWallpaper = HasEvidenceLine(TEXT("TRUE - the nursery wallpaper - child's sun turned into a halo"));

    switch (CasePhase)
    {
    case ETheLastRiteCasePhase::SealedAwaitingExit:
        FocusZoneText = NSLOCTEXT(
            "TheLastRite",
            "FocusZoneExit",
            "Focus zone: front door. The room is sealed. Leave cleanly and file the case.");
        return;
    case ETheLastRiteCasePhase::ClosedWin:
        FocusZoneText = NSLOCTEXT(
            "TheLastRite",
            "FocusZoneClosedWin",
            "Focus zone: case report. The investigation is over; the surviving lesson is that the nursery carried the room's real weight.");
        return;
    case ETheLastRiteCasePhase::ClosedFail:
        FocusZoneText = NSLOCTEXT(
            "TheLastRite",
            "FocusZoneClosedFail",
            "Focus zone: failure review. Rebuild the run from the body side, then stay on the nursery pattern instead of the mirror bait.");
        return;
    case ETheLastRiteCasePhase::RiteReady:
        FocusZoneText = NSLOCTEXT(
            "TheLastRite",
            "FocusZoneRiteReady",
            "Focus zone: ritual start. The nursery sigil matches the full chain; begin the 4-hand rite there and leave the mirror alone.");
        return;
    case ETheLastRiteCasePhase::RitualInProgress:
        FocusZoneText = FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "FocusZoneRitualInProgress",
                "Focus zone: nursery sigil. Keep the {0} hand on the child-facing altar until the whole 4-hand sequence clears."),
            GetCurrentRitualHandText());
        return;
    case ETheLastRiteCasePhase::Investigating:
    default:
        break;
    }

    if (!bFoundNanny)
    {
        FocusZoneText = NSLOCTEXT(
            "TheLastRite",
            "FocusZoneBody",
            "Focus zone: the body. Log Nanny Eliza first before the room has time to scatter your attention.");
        return;
    }

    if (!bFoundCradle)
    {
        FocusZoneText = NSLOCTEXT(
            "TheLastRite",
            "FocusZoneCradle",
            "Focus zone: cradle lane. From the body, angle into the CHAIN - NURSERY side and keep your read on the cradle label until it takes over.");
        return;
    }

    if (!bFoundPrayerCards)
    {
        FocusZoneText = NSLOCTEXT(
            "TheLastRite",
            "FocusZonePrayerMess",
            "Focus zone: prayer mess. Finish the opening sweep there before you test the rest of the room.");
        return;
    }

    if (!bFoundMonitor)
    {
        FocusZoneText = NSLOCTEXT(
            "TheLastRite",
            "FocusZoneMonitor",
            "Focus zone: nursery chain, step 4. The monitor should tell you whether the room's voice belongs to the same child-facing pattern.");
        return;
    }

    if (!bFoundWallpaper)
    {
        FocusZoneText = NSLOCTEXT(
            "TheLastRite",
            "FocusZoneWallpaper",
            "Focus zone: nursery chain, step 5. The mural should confirm that the visual center lands where the body, cradle, prayer mess, and hymn were already pointing.");
        return;
    }

    FocusZoneText = NSLOCTEXT(
        "TheLastRite",
        "FocusZoneLate",
        "Focus zone: ritual threshold. The room has given you enough to commit; do not drift back toward the mirror theater.");
}

void ATheLastRiteGameMode::UpdateTheoryChainText()
{
    const bool bFoundNanny = HasEvidenceLine(TEXT("TRUE - Nanny Eliza - mirrored wrist marks"));
    const bool bFoundCradle = HasEvidenceLine(TEXT("TRUE - the cradle - halo of ash-white handprints"));
    const bool bFoundPrayerCards = HasEvidenceLine(TEXT("TRUE - the prayer cards - fused into a crown"));
    const bool bFoundMonitor = HasEvidenceLine(TEXT("TRUE - the baby monitor - hymn repeating on every channel"));
    const bool bFoundWallpaper = HasEvidenceLine(TEXT("TRUE - the nursery wallpaper - child's sun turned into a halo"));

    switch (CasePhase)
    {
    case ETheLastRiteCasePhase::SealedAwaitingExit:
    case ETheLastRiteCasePhase::ClosedWin:
        TheoryChainText = NSLOCTEXT(
            "TheLastRite",
            "TheoryChainWon",
            "Theory chain: body pose, cradle halo, prayer crown, monitor hymn, and mural halo all converged on the nursery sigil.");
        return;
    case ETheLastRiteCasePhase::ClosedFail:
        TheoryChainText = NSLOCTEXT(
            "TheLastRite",
            "TheoryChainLost",
            "Theory chain: the real signs still converged on the nursery side. The mirror copied the pattern's shape without carrying its weight.");
        return;
    case ETheLastRiteCasePhase::RiteReady:
        TheoryChainText = NSLOCTEXT(
            "TheLastRite",
            "TheoryChainReady",
            "Theory chain: body pose, cradle halo, prayer crown, monitor hymn, and mural halo all ended at the nursery sigil. The mirror only copied the look.");
        return;
    case ETheLastRiteCasePhase::RitualInProgress:
        TheoryChainText = FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "TheoryChainRitualInProgress",
                "Theory chain: the read is settled and the rite is underway. The nursery sigil carries the chain while the {0} hand keeps the seal moving."),
            GetCurrentRitualHandText());
        return;
    case ETheLastRiteCasePhase::Investigating:
    default:
        break;
    }

    if (!bFoundNanny)
    {
        TheoryChainText = NSLOCTEXT(
            "TheLastRite",
            "TheoryChainZero",
            "Theory chain: start with the body. If the pose is ritualized, the nursery should echo it in cleaner forms than the room's random violence.");
        return;
    }

    if (!bFoundCradle)
    {
        TheoryChainText = NSLOCTEXT(
            "TheLastRite",
            "TheoryChainAfterBody",
            "Theory chain: the body looks staged. Check whether the cradle repeats that forced sanctity around the child-facing space.");
        return;
    }

    if (!bFoundPrayerCards)
    {
        TheoryChainText = NSLOCTEXT(
            "TheLastRite",
            "TheoryChainAfterCradle",
            "Theory chain: the body pose and cradle halo already agree. If the prayer mess also crowns the nursery side, the room stops reading as random.");
        return;
    }

    if (!bFoundMonitor)
    {
        TheoryChainText = NSLOCTEXT(
            "TheLastRite",
            "TheoryChainAfterSweep",
            "Theory chain: body pose, cradle halo, and prayer crown already agree. Now test whether the monitor hymn keeps the chain on the nursery side instead of the mirror.");
        return;
    }

    if (!bFoundWallpaper)
    {
        TheoryChainText = NSLOCTEXT(
            "TheLastRite",
            "TheoryChainAfterMonitor",
            "Theory chain: body, cradle, prayer mess, and monitor now agree. Check whether the mural locks the room's visual center to the nursery too.");
        return;
    }

    TheoryChainText = NSLOCTEXT(
        "TheLastRite",
        "TheoryChainLate",
        "Theory chain: the investigation is complete. Every real sign points child-side, so the mirror can only be theater.");
}

void ATheLastRiteGameMode::UpdateRiskText()
{
    const bool bFoundWindow = HasEvidenceLine(TEXT("FALSE - the broken window latch - forced from outside"));
    const bool bFoundTicket = HasEvidenceLine(TEXT("FALSE - the pawn ticket pouch - ordinary greed"));
    const bool bFoundKnife = HasEvidenceLine(TEXT("FALSE - the kitchen knife - grease, not offering blood"));
    const bool bFoundAnyFalseLead = bFoundWindow || bFoundTicket || bFoundKnife;

    switch (CasePhase)
    {
    case ETheLastRiteCasePhase::SealedAwaitingExit:
    case ETheLastRiteCasePhase::ClosedWin:
        RiskText = NSLOCTEXT(
            "TheLastRite",
            "RiskWon",
            "Risk read: cleared. The mirror bait never carried the room once the real chain was finished.");
        return;
    case ETheLastRiteCasePhase::ClosedFail:
        RiskText = NSLOCTEXT(
            "TheLastRite",
            "RiskLost",
            "Risk read: the failure came from trusting spectacle over structure. The mirror stole the call that belonged to the nursery.");
        return;
    case ETheLastRiteCasePhase::RiteReady:
        RiskText = NSLOCTEXT(
            "TheLastRite",
            "RiskReady",
            "Risk read: the last trap is choosing spectacle over corroboration. The mirror circle imitates sanctity, but the room's real weight stayed child-facing from start to finish.");
        return;
    case ETheLastRiteCasePhase::RitualInProgress:
        RiskText = NSLOCTEXT(
            "TheLastRite",
            "RiskRitualInProgress",
            "Risk read: the trap has narrowed. Breaking from the nursery sigil now hands the room back to spectacle, and a wrong anchor still fails the case immediately.");
        return;
    case ETheLastRiteCasePhase::Investigating:
    default:
        break;
    }

    if (!IsOpeningSweepComplete())
    {
        RiskText = NSLOCTEXT(
            "TheLastRite",
            "RiskOpeningSweep",
            "Risk read: do not let broken glass, kitchen grime, or panic staging pull you off the opening sweep.");
        return;
    }

    if (bFoundAnyFalseLead)
    {
        RiskText = NSLOCTEXT(
            "TheLastRite",
            "RiskFalseLeadSeen",
            "Risk read: the room has shown you what fake weight looks like. Broken glass and ugliness spike attention, but the repeated chain still lives on the nursery side.");
        return;
    }

    RiskText = NSLOCTEXT(
        "TheLastRite",
        "RiskLateInvestigating",
        "Risk read: the mirror is still louder to the eye, but it is losing the logic war. It copies symmetry without inheriting the chain.");
}

void ATheLastRiteGameMode::UpdateCorrectAnchorReadText()
{
    const bool bFoundMonitor = HasEvidenceLine(TEXT("TRUE - the baby monitor - hymn repeating on every channel"));
    const bool bFoundWallpaper = HasEvidenceLine(TEXT("TRUE - the nursery wallpaper - child's sun turned into a halo"));

    switch (CasePhase)
    {
    case ETheLastRiteCasePhase::SealedAwaitingExit:
    case ETheLastRiteCasePhase::ClosedWin:
        CorrectAnchorReadText = NSLOCTEXT(
            "TheLastRite",
            "CorrectAnchorReadWon",
            "Use the nursery sigil because the confirmed chain ended there: pose, cradle halo, prayer crown, hymn loop, and mural halo.");
        return;
    case ETheLastRiteCasePhase::ClosedFail:
        CorrectAnchorReadText = NSLOCTEXT(
            "TheLastRite",
            "CorrectAnchorReadLost",
            "The nursery sigil was still correct. The child-facing signs were the only clues that kept reinforcing one another.");
        return;
    case ETheLastRiteCasePhase::RiteReady:
        CorrectAnchorReadText = NSLOCTEXT(
            "TheLastRite",
            "CorrectAnchorReadReady",
            "Use the nursery sigil. Every confirmed sign, from body pose through mural halo, converges on the child-facing altar instead of the mirror theatrics.");
        return;
    case ETheLastRiteCasePhase::RitualInProgress:
        CorrectAnchorReadText = FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "CorrectAnchorReadInProgress",
                "Stay on the nursery sigil. The chain is already confirmed, and the {0} hand belongs here until the full rite is complete."),
            GetCurrentRitualHandText());
        return;
    case ETheLastRiteCasePhase::Investigating:
    default:
        break;
    }

    if (!IsOpeningSweepComplete())
    {
        CorrectAnchorReadText = NSLOCTEXT(
            "TheLastRite",
            "CorrectAnchorReadOpeningSweep",
            "Nursery sigil read: hold the call. Finish the body, cradle, and prayer mess before naming the altar.");
        return;
    }

    if (!bFoundMonitor)
    {
        CorrectAnchorReadText = NSLOCTEXT(
            "TheLastRite",
            "CorrectAnchorReadMonitor",
            "Nursery sigil read: leading. The body, cradle, and prayer mess already agree; check whether the monitor hymn joins them.");
        return;
    }

    if (!bFoundWallpaper)
    {
        CorrectAnchorReadText = NSLOCTEXT(
            "TheLastRite",
            "CorrectAnchorReadWallpaper",
            "Nursery sigil read: nearly locked. The hymn already reinforced the nursery chain; confirm the mural halo to finish the case.");
        return;
    }

    CorrectAnchorReadText = NSLOCTEXT(
        "TheLastRite",
        "CorrectAnchorReadLate",
        "Nursery sigil read: it already has the strongest case. The room's real pattern keeps resolving child-side.");
}

void ATheLastRiteGameMode::UpdateWrongAnchorReadText()
{
    const bool bFoundAnyFalseLead =
        HasEvidenceLine(TEXT("FALSE - the broken window latch - forced from outside"))
        || HasEvidenceLine(TEXT("FALSE - the pawn ticket pouch - ordinary greed"))
        || HasEvidenceLine(TEXT("FALSE - the kitchen knife - grease, not offering blood"));

    switch (CasePhase)
    {
    case ETheLastRiteCasePhase::SealedAwaitingExit:
    case ETheLastRiteCasePhase::ClosedWin:
        WrongAnchorReadText = NSLOCTEXT(
            "TheLastRite",
            "WrongAnchorReadWon",
            "Reject the mirror circle because it only copied the saint-mask. None of the room's confirmed evidence terminated there.");
        return;
    case ETheLastRiteCasePhase::ClosedFail:
        WrongAnchorReadText = NSLOCTEXT(
            "TheLastRite",
            "WrongAnchorReadLost",
            "The mirror circle was wrong because it offered spectacle without corroboration. It looked ritualized, but the evidence chain never landed there.");
        return;
    case ETheLastRiteCasePhase::RiteReady:
        WrongAnchorReadText = NSLOCTEXT(
            "TheLastRite",
            "WrongAnchorReadReady",
            "Reject the mirror circle. It copies false sanctity with broken glass and symmetry, but the evidence chain never terminates there.");
        return;
    case ETheLastRiteCasePhase::RitualInProgress:
        WrongAnchorReadText = NSLOCTEXT(
            "TheLastRite",
            "WrongAnchorReadInProgress",
            "Reject the mirror circle. It is still bait, and breaking the 4-hand rite there would fail the case immediately.");
        return;
    case ETheLastRiteCasePhase::Investigating:
    default:
        break;
    }

    if (!IsOpeningSweepComplete())
    {
        WrongAnchorReadText = NSLOCTEXT(
            "TheLastRite",
            "WrongAnchorReadOpeningSweep",
            "Mirror circle read: too early to trust. Loud staging is not the same thing as ritual weight.");
        return;
    }

    if (bFoundAnyFalseLead)
    {
        WrongAnchorReadText = NSLOCTEXT(
            "TheLastRite",
            "WrongAnchorReadFalseLeadSeen",
            "Mirror circle read: it belongs with the other bait. The room keeps using spectacle to mimic meaning without joining the confirmed chain.");
        return;
    }

    WrongAnchorReadText = NSLOCTEXT(
        "TheLastRite",
        "WrongAnchorReadLate",
        "Mirror circle read: still tempting, but unsupported. It is visually louder than the nursery because it wants to be chosen before the evidence is complete.");
}

void ATheLastRiteGameMode::RefreshCurrentObjectiveText()
{
    const bool bFoundMonitor = HasEvidenceLine(TEXT("TRUE - the baby monitor - hymn repeating on every channel"));
    const bool bFoundWallpaper = HasEvidenceLine(TEXT("TRUE - the nursery wallpaper - child's sun turned into a halo"));

    switch (CasePhase)
    {
    case ETheLastRiteCasePhase::Investigating:
        if (GetOpeningSweepCount() < 3)
        {
            CurrentObjectiveText = FText::Format(
                NSLOCTEXT(
                    "TheLastRite",
                    "CurrentObjectiveInvestigatingOpeningSweep",
                    "Current objective: {0}."),
                GetNextStarterTargetText());
        }
        else if (!bFoundMonitor)
        {
            CurrentObjectiveText = NSLOCTEXT(
                "TheLastRite",
                "CurrentObjectiveInvestigatingMonitor",
                "Current objective: inspect the baby monitor and test whether the nursery chain extends beyond the opening sweep.");
        }
        else if (!bFoundWallpaper)
        {
            CurrentObjectiveText = NSLOCTEXT(
                "TheLastRite",
                "CurrentObjectiveInvestigatingWallpaper",
                "Current objective: inspect the nursery mural and confirm the room's visual center matches the chain.");
        }
        else
        {
            CurrentObjectiveText = NSLOCTEXT(
                "TheLastRite",
                "CurrentObjectiveInvestigatingLate",
                "Current objective: settle the real ritual center. The nursery chain should now beat the mirror bait.");
        }
        break;
    case ETheLastRiteCasePhase::RiteReady:
        CurrentObjectiveText = NSLOCTEXT(
            "TheLastRite",
            "CurrentObjectiveRiteReady",
            "Current objective: start the 4-hand rite at the nursery sigil, where the confirmed chain converges.");
        break;
    case ETheLastRiteCasePhase::RitualInProgress:
        CurrentObjectiveText = FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "CurrentObjectiveRitualInProgress",
                "Current objective: continue the 4-hand rite at the nursery sigil. Active hand: {0}."),
            GetCurrentRitualHandText());
        break;
    case ETheLastRiteCasePhase::SealedAwaitingExit:
        CurrentObjectiveText = NSLOCTEXT(
            "TheLastRite",
            "CurrentObjectiveExit",
            "Current objective: leave through the front door and file the case.");
        break;
    case ETheLastRiteCasePhase::ClosedWin:
    case ETheLastRiteCasePhase::ClosedFail:
    default:
        CurrentObjectiveText = NSLOCTEXT(
            "TheLastRite",
            "CurrentObjectiveRestart",
            "Current objective: press R to restart the case, or Esc to quit.");
        break;
    }
}

void ATheLastRiteGameMode::RefreshCasePresentation(bool bRebuildFinalReport)
{
    UpdateDeductionText();
    UpdateRitualReadText();
    UpdateNextMoveText();
    UpdateFocusZoneText();
    UpdateTheoryChainText();
    UpdateRiskText();
    UpdateCorrectAnchorReadText();
    UpdateWrongAnchorReadText();
    UpdateProgressText();
    RefreshCurrentObjectiveText();
    UpdateRitualAnchors();
    UpdateCaseExit();
    UpdateWorldMood();

    if (bRebuildFinalReport)
    {
        RebuildFinalReport();
    }
}

int32 ATheLastRiteGameMode::GetOpeningSweepCount() const
{
    const bool bFoundNanny = HasEvidenceLine(TEXT("TRUE - Nanny Eliza - mirrored wrist marks"));
    const bool bFoundCradle = HasEvidenceLine(TEXT("TRUE - the cradle - halo of ash-white handprints"));
    const bool bFoundPrayerCards = HasEvidenceLine(TEXT("TRUE - the prayer cards - fused into a crown"));
    return static_cast<int32>(bFoundNanny) + static_cast<int32>(bFoundCradle) + static_cast<int32>(bFoundPrayerCards);
}

FText ATheLastRiteGameMode::GetOpeningSweepStateText() const
{
    const int32 OpeningSweepCount = GetOpeningSweepCount();
    return OpeningSweepCount < 3
        ? FText::Format(
            NSLOCTEXT("TheLastRite", "OpeningSweepInProgress", "Opening sweep: {0}/3"),
            FText::AsNumber(OpeningSweepCount))
        : NSLOCTEXT("TheLastRite", "OpeningSweepComplete", "Opening sweep: complete");
}

FText ATheLastRiteGameMode::GetNextStarterTargetText() const
{
    if (!HasEvidenceLine(TEXT("TRUE - Nanny Eliza - mirrored wrist marks")))
    {
        return NSLOCTEXT("TheLastRite", "NextStarterTargetNanny", "inspect Nanny Eliza's body");
    }

    if (!HasEvidenceLine(TEXT("TRUE - the cradle - halo of ash-white handprints")))
    {
        return NSLOCTEXT("TheLastRite", "NextStarterTargetCradle", "move into the CHAIN - NURSERY side and inspect the cradle");
    }

    if (!HasEvidenceLine(TEXT("TRUE - the prayer cards - fused into a crown")))
    {
        return NSLOCTEXT("TheLastRite", "NextStarterTargetPrayerCards", "inspect the prayer mess");
    }

    return NSLOCTEXT("TheLastRite", "NextStarterTargetComplete", "the opening sweep is complete");
}

FText ATheLastRiteGameMode::GetActiveGuidanceText() const
{
    return GetNextMoveText().IsEmpty() ? GetNextStarterTargetText() : GetNextMoveText();
}

FString ATheLastRiteGameMode::GetToolPayloadLine() const
{
    const auto GetClaimedText = [this](ETheLastRiteSessionTool ToolType)
    {
        const ASessionToolPickup* ToolPickup = FindSessionToolPickupByType(SessionToolPickups, ToolType);
        return HasClaimedToolPickup(ToolPickup) ? TEXT("claimed") : TEXT("missing");
    };

    return FString::Printf(
        TEXT("Payload: roles reader=%s camera=%s censer=%s guard=%s"),
        GetClaimedText(ETheLastRiteSessionTool::HolyBook),
        GetClaimedText(ETheLastRiteSessionTool::FilmCamera),
        GetClaimedText(ETheLastRiteSessionTool::Censer),
        GetClaimedText(ETheLastRiteSessionTool::Crucifix));
}

FString ATheLastRiteGameMode::GetCoveragePayloadLine() const
{
    const ASessionToolPickup* FilmCamera = FindSessionToolPickupByType(SessionToolPickups, ETheLastRiteSessionTool::FilmCamera);
    const ASessionToolPickup* HolyBook = FindSessionToolPickupByType(SessionToolPickups, ETheLastRiteSessionTool::HolyBook);
    const ASessionToolPickup* Censer = FindSessionToolPickupByType(SessionToolPickups, ETheLastRiteSessionTool::Censer);
    const ASessionToolPickup* Crucifix = FindSessionToolPickupByType(SessionToolPickups, ETheLastRiteSessionTool::Crucifix);

    const bool bRitualToolsReady = HasClaimedToolPickup(HolyBook)
        && HasClaimedToolPickup(Censer)
        && HasClaimedToolPickup(Crucifix);

    return FString::Printf(
        TEXT("Payload: coverage opening_sweep=%s camera_clues=%s ritual_tools=%s"),
        IsOpeningSweepComplete() ? TEXT("complete") : TEXT("incomplete"),
        HasClaimedToolPickup(FilmCamera) ? TEXT("2_of_2_available") : TEXT("camera_missing"),
        bRitualToolsReady ? TEXT("ready") : TEXT("incomplete"));
}

bool ATheLastRiteGameMode::HasClaimedToolPickup(const ASessionToolPickup* ToolPickup) const
{
    return ToolPickup != nullptr && ToolPickup->IsClaimed();
}

void ATheLastRiteGameMode::RebuildFinalReport()
{
    FinalReportLines.Reset();

    if (CasePhase == ETheLastRiteCasePhase::ClosedWin)
    {
        FinalReportLines.Add(TEXT("Apartment 302"));
        FinalReportLines.Add(TEXT("Hollow Saint"));
        FinalReportLines.Add(TEXT("Verdict: the nursery sigil was the correct anchor."));
        FinalReportLines.Add(TEXT("Read: the full evidence chain terminated child-side, and the mirror never carried corroboration."));
        FinalReportLines.Add(FString::Printf(TEXT("Why the nursery held: %s"), *CorrectAnchorReadText.ToString()));
        FinalReportLines.Add(FString::Printf(TEXT("Why the mirror failed: %s"), *WrongAnchorReadText.ToString()));
        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(TEXT("Role formation"));
        FinalReportLines.Add(GetToolRoleText().ToString());
        FinalReportLines.Add(GetCoveragePayloadLine());
        FinalReportLines.Add(GetToolPayloadLine());
        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(FString::Printf(TEXT("Confirmed chain (%d/%d)"), FoundTrueClues, RequiredTrueClues));

        for (const FReportEvidenceEntry& Entry : TrueClueReportEntries)
        {
            if (HasEvidenceLine(Entry.EvidenceLine))
            {
                FinalReportLines.Add(FString::Printf(TEXT("Confirmed sign: %s"), Entry.ReportSummary));
            }
        }

        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(FString::Printf(TEXT("Discarded bait (%d/%d)"), FoundFalseLeads, TotalFalseLeads));

        for (const FReportEvidenceEntry& Entry : FalseLeadReportEntries)
        {
            if (HasEvidenceLine(Entry.EvidenceLine))
            {
                FinalReportLines.Add(FString::Printf(TEXT("Discarded bait: %s"), Entry.ReportSummary));
            }
        }

        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(TEXT("Result: all four hands cleared at the nursery sigil, the front door was crossed, and the case was filed cleanly."));
        FinalReportLines.Add(TEXT("Press R to restart the case or Esc to quit."));
    }
    else if (CasePhase == ETheLastRiteCasePhase::ClosedFail)
    {
        FinalReportLines.Add(TEXT("Apartment 302"));
        FinalReportLines.Add(TEXT("Hollow Saint"));
        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(TEXT("What went wrong"));
        FinalReportLines.Add(TEXT("Wrong rite: mirror circle."));
        FinalReportLines.Add(TEXT("Right anchor: nursery sigil."));
        FinalReportLines.Add(TEXT("Right read: the body, cradle, prayer mess, monitor, and mural all weighted the nursery side."));
        FinalReportLines.Add(TEXT("Failure read: the mirror copied sanctity and symmetry, but the evidence chain never ended there."));
        FinalReportLines.Add(FString::Printf(TEXT("Why the nursery held: %s"), *CorrectAnchorReadText.ToString()));
        FinalReportLines.Add(FString::Printf(TEXT("Why the mirror failed: %s"), *WrongAnchorReadText.ToString()));
        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(TEXT("Role formation"));
        FinalReportLines.Add(GetToolRoleText().ToString());
        FinalReportLines.Add(GetCoveragePayloadLine());
        FinalReportLines.Add(GetToolPayloadLine());
        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(FString::Printf(TEXT("Confirmed chain (%d/%d)"), FoundTrueClues, RequiredTrueClues));
        for (const FReportEvidenceEntry& Entry : TrueClueReportEntries)
        {
            if (HasEvidenceLine(Entry.EvidenceLine))
            {
                FinalReportLines.Add(FString::Printf(TEXT("Confirmed sign: %s"), Entry.ReportSummary));
            }
            else
            {
                FinalReportLines.Add(FString::Printf(TEXT("Missed sign: %s"), Entry.ReportSummary));
            }
        }
        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(FString::Printf(TEXT("Discarded bait (%d/%d)"), FoundFalseLeads, TotalFalseLeads));
        for (const FReportEvidenceEntry& Entry : FalseLeadReportEntries)
        {
            if (HasEvidenceLine(Entry.EvidenceLine))
            {
                FinalReportLines.Add(FString::Printf(TEXT("Discarded bait: %s"), Entry.ReportSummary));
            }
            else
            {
                FinalReportLines.Add(FString::Printf(TEXT("Unchecked bait: %s"), Entry.ReportSummary));
            }
        }
        FinalReportLines.Add(TEXT("Result: the bait circle was trusted over the corroborated child-facing chain."));
        FinalReportLines.Add(TEXT("Press R to restart the case or Esc to quit."));
    }
}

void ATheLastRiteGameMode::TriggerPhasePulse(const FLinearColor& Color, float DurationSeconds)
{
    PhasePulseColor = Color;
    PhasePulseDurationSeconds = DurationSeconds;
    if (GetWorld() != nullptr)
    {
        PhasePulseStartedAtSeconds = GetWorld()->GetTimeSeconds();
    }
}

void ATheLastRiteGameMode::SetStatusText(const FText& NewStatusText)
{
    StatusText = NewStatusText;
    RecentEventText = NewStatusText;
    if (GetWorld() != nullptr)
    {
        RecentEventTimeSeconds = GetWorld()->GetTimeSeconds();
    }
}

void ATheLastRiteGameMode::AddEvidenceLine(const FString& NewLine)
{
    EvidenceLines.Add(NewLine);
}
