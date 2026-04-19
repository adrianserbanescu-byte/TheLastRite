#include "TheLastRiteGameMode.h"

#include "CaseExit.h"
#include "InspectableProp.h"
#include "RitualAnchor.h"
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

namespace
{
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
        "Apartment 302. Work the room in order: body, cradle, prayer mess. Find all 5 real clues tied to the Hollow Saint, perform the rite, then leave through the front door.");
    StatusText = NSLOCTEXT(
        "TheLastRite",
        "StatusInitial",
        "White labels mark usable objects. The notes panel tracks what is real and what is bait.");
    CurrentObjectiveText = NSLOCTEXT(
        "TheLastRite",
        "CurrentObjectiveInitial",
        "Current objective: step 1 of 3 - inspect the body, the cradle, and the prayer mess.");
    RecentEventText = FText::GetEmpty();
    ProgressText = FText::GetEmpty();
    DeductionText = NSLOCTEXT(
        "TheLastRite",
        "DeductionInitial",
        "Read of the room: no clean pattern yet. Start with the body, the cradle, and the prayer mess.");
    RitualReadText = NSLOCTEXT(
        "TheLastRite",
        "RitualReadInitial",
        "Ritual read: unresolved. Finish the opening sweep before trusting any altar.");
    NextMoveText = NSLOCTEXT(
        "TheLastRite",
        "NextMoveInitial",
        "Start at the body. Then move to the cradle and the prayer mess before chasing anything else in the room.");
    FocusZoneText = NSLOCTEXT(
        "TheLastRite",
        "FocusZoneInitial",
        "Focus zone: entry and body side. Establish the opening sweep before you test the nursery or the mirror.");
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
                "Evidence complete. The child-facing pattern is locked. Choose the ritual anchor that fits the room's true center.");
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
            NSLOCTEXT("TheLastRite", "NeedMoreClues", "Not yet. You still need {0} real clue(s) before the rite. {1}"),
            FText::AsNumber(RequiredTrueClues - FoundTrueClues),
            NextGuidance));
        return;
    }

    Anchor->MarkActivated();

    if (Anchor->IsCorrectAnchor())
    {
        CasePhase = ETheLastRiteCasePhase::SealedAwaitingExit;
        ObjectiveText = NSLOCTEXT(
            "TheLastRite",
            "ObjectiveWon",
            "The rite held. Leave through the front door and file the case.");
        EndingText = FText::GetEmpty();
        EndingDetailText = FText::GetEmpty();
        SetStatusText(NSLOCTEXT(
            "TheLastRite",
            "WinStatus",
            "You chose the nursery sigil. The room goes still. Do not linger. Leave through the front door and file the case."));
        AddEvidenceLine(TEXT("RITE - correct anchor"));
        DeductionText = NSLOCTEXT(
            "TheLastRite",
            "DeductionWon",
            "Conclusion: the saint fed through the child-facing altar. The seal is holding. Leave through the front door and file the case cleanly.");
        TriggerPhasePulse(FLinearColor(0.30f, 0.95f, 0.55f, 1.0f), 1.2f);
    }
    else
    {
        CasePhase = ETheLastRiteCasePhase::ClosedFail;
        ObjectiveText = NSLOCTEXT(
            "TheLastRite",
            "ObjectiveLost",
            "Case failed. The wrong rite fed the Hollow Saint.");
        EndingText = NSLOCTEXT(
            "TheLastRite",
            "LoseEnding",
            "WRONG RITE\nThe saint answers first.");
        EndingDetailText = NSLOCTEXT(
            "TheLastRite",
            "LoseEndingDetail",
            "The mirror circle was bait. The real pattern lived around the child, not the broken glass.");
        SetStatusText(NSLOCTEXT(
            "TheLastRite",
            "LoseStatus",
            "You chose the wrong anchor. The false circle feeds the thing you were hunting."));
        AddEvidenceLine(TEXT("RITE - wrong anchor"));
        DeductionText = NSLOCTEXT(
            "TheLastRite",
            "DeductionLost",
            "Conclusion missed: the mirror was only theater. The real ritual weight lived around the cradle.");
        TriggerPhasePulse(FLinearColor(1.0f, 0.05f, 0.03f, 1.0f), 1.4f);
    }

    RefreshCasePresentation(true);
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
                "You cannot leave yet. The rite still has to be settled. {0}"),
            NextGuidance));
        return;
    }

    Exit->MarkUsed();
    CasePhase = ETheLastRiteCasePhase::ClosedWin;
    ObjectiveText = NSLOCTEXT(
        "TheLastRite",
        "ObjectiveClosed",
        "Case filed. Apartment 302 is sealed for cleanup.");
    EndingText = NSLOCTEXT(
        "TheLastRite",
        "FinalWinEnding",
        "CASE CLOSED\nApartment 302 is sealed.");
    EndingDetailText = NSLOCTEXT(
        "TheLastRite",
        "FinalWinDetail",
        "You held the rite, left cleanly, and filed the report. The Hollow Saint loses its room when the living stop feeding it a story.");
    SetStatusText(NSLOCTEXT(
        "TheLastRite",
        "FinalWinStatus",
        "You left through the front door and filed the case."));
    AddEvidenceLine(TEXT("EXIT - front door secured"));
    DeductionText = NSLOCTEXT(
        "TheLastRite",
        "DeductionClosed",
        "Final read: the nursery was the altar, the mirror was bait, and the case is now sealed.");
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
        return NSLOCTEXT("TheLastRite", "ResolvedInteractionExit", "Case settled. Leave through the front door and file the case.");
    case ETheLastRiteCasePhase::ClosedWin:
        return NSLOCTEXT("TheLastRite", "ResolvedInteractionClosedWin", "Case closed. Press R to restart the case or Esc to quit.");
    case ETheLastRiteCasePhase::ClosedFail:
        return NSLOCTEXT("TheLastRite", "ResolvedInteractionClosedFail", "Case failed. Press R to restart the case or Esc to quit.");
    case ETheLastRiteCasePhase::Investigating:
    case ETheLastRiteCasePhase::RiteReady:
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
    SpawnZoneMarker(FVector(230.0f, 260.0f, 8.0f), FVector(3.2f, 0.34f, 0.04f), TEXT("SWEEP - NURSERY"), FLinearColor(0.86f, 0.70f, 0.28f));
    SpawnZoneMarker(FVector(-790.0f, -620.0f, 8.0f), FVector(2.5f, 0.32f, 0.04f), TEXT("BAIT - MIRROR"), FLinearColor(0.46f, 0.56f, 0.74f));
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

    SpawnProp(
        FVector(0.0f, -780.0f, 95.0f),
        FVector(0.7f, 0.7f, 1.9f),
        NSLOCTEXT("TheLastRite", "NannyName", "Nanny Eliza"),
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
        NSLOCTEXT("TheLastRite", "PrayerCardsName", "the prayer cards"),
        NSLOCTEXT("TheLastRite", "PrayerCardsClue", "The prayer cards are fused into a crown-like circle."),
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
    const bool bRitualReady = CasePhase == ETheLastRiteCasePhase::RiteReady;
    for (ARitualAnchor* Anchor : RitualAnchors)
    {
        if (Anchor != nullptr)
        {
            Anchor->SetRitualReady(bRitualReady);
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

    CasePhase = FoundTrueClues >= RequiredTrueClues
        ? ETheLastRiteCasePhase::RiteReady
        : ETheLastRiteCasePhase::Investigating;
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
            NSLOCTEXT("TheLastRite", "ProgressRiteReady", "True clues: {0}/{1} | False leads: {2}/{3} | Rite ready: act on your read"),
            FText::AsNumber(FoundTrueClues),
            FText::AsNumber(RequiredTrueClues),
            FText::AsNumber(FoundFalseLeads),
            FText::AsNumber(TotalFalseLeads));
        return;
    case ETheLastRiteCasePhase::Investigating:
    default:
        break;
    }

    const FText RiteState = FText::Format(
        NSLOCTEXT("TheLastRite", "RiteLocked", "Rite locked: {0} clue(s) left"),
        FText::AsNumber(RequiredTrueClues - FoundTrueClues));

    ProgressText = FText::Format(
        NSLOCTEXT("TheLastRite", "Progress", "True clues: {0}/{1} | False leads: {2}/{3} | {4} | {5}"),
        FText::AsNumber(FoundTrueClues),
        FText::AsNumber(RequiredTrueClues),
        FText::AsNumber(FoundFalseLeads),
        FText::AsNumber(TotalFalseLeads),
        GetOpeningSweepStateText(),
        RiteState);
}

void ATheLastRiteGameMode::UpdateDeductionText()
{
    if (CasePhase == ETheLastRiteCasePhase::SealedAwaitingExit
        || CasePhase == ETheLastRiteCasePhase::ClosedWin
        || CasePhase == ETheLastRiteCasePhase::ClosedFail)
    {
        return;
    }

    if (FoundTrueClues <= 0)
    {
        DeductionText = NSLOCTEXT(
            "TheLastRite",
            "DeductionZero",
            "Read of the room: pattern unclear. Start with the body, the cradle, and the prayer mess.");
        return;
    }

    if (FoundTrueClues == 1)
    {
        DeductionText = NSLOCTEXT(
            "TheLastRite",
            "DeductionOne",
            "Read of the room: pattern unclear, but the room already feels staged instead of random.");
        return;
    }

    if (FoundTrueClues == 2)
    {
        DeductionText = NSLOCTEXT(
            "TheLastRite",
            "DeductionTwo",
            "Read of the room: the nursery side is gaining weight. The room wants your eyes on the child-facing signs.");
        return;
    }

    if (FoundTrueClues == 3)
    {
        DeductionText = FoundFalseLeads > 0
            ? NSLOCTEXT(
                "TheLastRite",
                "DeductionThreeFalse",
                "Read of the room: the nursery side is gaining weight. The ugly damage is bait, not the ritual center.")
            : NSLOCTEXT(
                "TheLastRite",
                "DeductionThree",
                "Read of the room: the nursery side is gaining weight. The signs are starting to agree against the mirror.");
        return;
    }

    if (FoundTrueClues == 4)
    {
        DeductionText = NSLOCTEXT(
            "TheLastRite",
            "DeductionFour",
            "Read of the room: nearly locked. The mirror is bait. One last confirmation should settle the rite.");
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
            "Ritual read: commit to the nursery sigil. Ignore the mirror circle.");
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
            "Ritual read: unresolved. Finish the body, cradle, and prayer mess before trusting any altar.");
        return;
    }

    if (!bFoundMonitor)
    {
        RitualReadText = NSLOCTEXT(
            "TheLastRite",
            "RitualReadMonitor",
            "Ritual read: the weight is leaning child-side, but the room is not settled yet. Check the monitor.");
        return;
    }

    if (!bFoundWallpaper)
    {
        RitualReadText = NSLOCTEXT(
            "TheLastRite",
            "RitualReadWallpaper",
            "Ritual read: the child-side pattern is nearly locked. Check the nursery mural.");
        return;
    }

    RitualReadText = NSLOCTEXT(
        "TheLastRite",
        "RitualReadLate",
        "Ritual read: the nursery sigil is likely correct, and the mirror still reads as bait.");
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
            "The evidence is enough. Perform the rite at the nursery sigil, not the mirror circle.");
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
            "Start at the body. Inspect Nanny Eliza first, then move to the cradle and the prayer mess before chasing anything else in the room.");
        return;
    }

    if (!bFoundCradle)
    {
        NextMoveText = NSLOCTEXT(
            "TheLastRite",
            "NextMoveCradle",
            "Body logged. Move straight to the cradle next. The room should start making sense there before you drift toward the mirror side.");
        return;
    }

    if (!bFoundPrayerCards)
    {
        NextMoveText = NSLOCTEXT(
            "TheLastRite",
            "NextMovePrayerCards",
            "Cradle logged. Check the prayer mess next. That completes the opening sweep before you start testing the rest of the room.");
        return;
    }

    if (!bFoundMonitor)
    {
        NextMoveText = (bFoundWindow || bFoundTicket || bFoundKnife)
            ? NSLOCTEXT(
                "TheLastRite",
                "NextMoveMonitorAfterFalseLead",
                "You have seen enough bait. Go back to the nursery side and inspect the baby monitor. It is one of the room's real tells.")
            : NSLOCTEXT(
                "TheLastRite",
                "NextMoveMonitor",
                "Opening sweep complete. Stay with the child-facing pattern and inspect the baby monitor before trusting any uglier distractions.");
        return;
    }

    if (!bFoundWallpaper)
    {
        NextMoveText = NSLOCTEXT(
            "TheLastRite",
            "NextMoveWallpaper",
            "One confirmation remains. Inspect the nursery mural and settle the room's true center before attempting the rite.");
        return;
    }

    NextMoveText = FoundFalseLeads > 0
        ? NSLOCTEXT(
            "TheLastRite",
            "NextMoveLateWithFalseLead",
            "The read is complete. Ignore the mirror bait and use the nursery sigil when you are ready to seal the room.")
        : NSLOCTEXT(
            "TheLastRite",
            "NextMoveLate",
            "All five true signs agree. Commit to the nursery sigil, not the mirror circle.");
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
            "Focus zone: ritual area. The evidence is already enough; commit at the nursery sigil and ignore the mirror circle.");
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
            "Focus zone: entry and body side. Start at Nanny Eliza before the room has time to scatter your attention.");
        return;
    }

    if (!bFoundCradle || !bFoundPrayerCards)
    {
        FocusZoneText = NSLOCTEXT(
            "TheLastRite",
            "FocusZoneNurserySweep",
            "Focus zone: nursery sweep. Stay with the cradle and prayer mess until the opening read is complete.");
        return;
    }

    if (!bFoundMonitor)
    {
        FocusZoneText = NSLOCTEXT(
            "TheLastRite",
            "FocusZoneMonitor",
            "Focus zone: child-facing support evidence. The monitor should tell you whether the nursery side keeps repeating the same false sanctity.");
        return;
    }

    if (!bFoundWallpaper)
    {
        FocusZoneText = NSLOCTEXT(
            "TheLastRite",
            "FocusZoneWallpaper",
            "Focus zone: nursery mural. One last child-facing confirmation should settle where the rite really belongs.");
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
            "Theory chain: every real sign now agrees. The saint-mask was built around the child-facing space, not the broken glass.");
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
            "Theory chain: the opening sweep is complete. Now test whether the room's voice also loops back toward the child-facing pattern.");
        return;
    }

    if (!bFoundWallpaper)
    {
        TheoryChainText = NSLOCTEXT(
            "TheLastRite",
            "TheoryChainAfterMonitor",
            "Theory chain: the room's voice already sides with the nursery. Check the mural to see whether the visual center agrees too.");
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
            "Risk read: the remaining trap is simple. The mirror circle imitates the saint-mask, but the room's true weight is still child-facing.");
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
            "Risk read: the room is proving that ugliness alone means nothing. Keep weighting repeated child-facing signs over noisy damage.");
        return;
    }

    RiskText = NSLOCTEXT(
        "TheLastRite",
        "RiskLateInvestigating",
        "Risk read: the next trap is the mirror side. It is visually louder than the nursery because it wants to be trusted too early.");
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
            "Use the nursery sigil. Every confirmed sign converges on the child-facing altar instead of the glass theatrics.");
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
            "Nursery sigil read: leading, but not locked. Check whether the monitor hymn also loops back toward the child-facing space.");
        return;
    }

    if (!bFoundWallpaper)
    {
        CorrectAnchorReadText = NSLOCTEXT(
            "TheLastRite",
            "CorrectAnchorReadWallpaper",
            "Nursery sigil read: nearly locked. Confirm the mural halo and the room should finally give you the altar cleanly.");
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
            "Reject the mirror circle. It copies false sanctity with broken glass and symmetry, but none of the confirmed clues resolve at the mirror.");
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
            "Mirror circle read: it belongs with the other bait. The room keeps using spectacle to mimic meaning without supporting it.");
        return;
    }

    WrongAnchorReadText = NSLOCTEXT(
        "TheLastRite",
        "WrongAnchorReadLate",
        "Mirror circle read: still suspect. It is visually louder than the nursery because it is trying to be chosen before the evidence is complete.");
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
                    "Current objective: step 1 of 3 - {0}"),
                GetNextStarterTargetText());
        }
        else if (!bFoundMonitor)
        {
            CurrentObjectiveText = NSLOCTEXT(
                "TheLastRite",
                "CurrentObjectiveInvestigatingMonitor",
                "Current objective: step 2 of 3 - inspect the baby monitor and keep the nursery side in focus.");
        }
        else if (!bFoundWallpaper)
        {
            CurrentObjectiveText = NSLOCTEXT(
                "TheLastRite",
                "CurrentObjectiveInvestigatingWallpaper",
                "Current objective: step 2 of 3 - inspect the nursery mural and settle the room's true center.");
        }
        else
        {
            CurrentObjectiveText = NSLOCTEXT(
                "TheLastRite",
                "CurrentObjectiveInvestigatingLate",
                "Current objective: step 2 of 3 - settle which ritual point is real and prepare for the rite.");
        }
        break;
    case ETheLastRiteCasePhase::RiteReady:
        CurrentObjectiveText = NSLOCTEXT(
            "TheLastRite",
            "CurrentObjectiveRiteReady",
            "Current objective: step 3 of 3 - perform the rite where the child-facing pattern converges.");
        break;
    case ETheLastRiteCasePhase::SealedAwaitingExit:
        CurrentObjectiveText = NSLOCTEXT(
            "TheLastRite",
            "CurrentObjectiveExit",
            "Current objective: step 3 of 3 - leave through the front door and file the case.");
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
        return NSLOCTEXT("TheLastRite", "NextStarterTargetNanny", "inspect the body first");
    }

    if (!HasEvidenceLine(TEXT("TRUE - the cradle - halo of ash-white handprints")))
    {
        return NSLOCTEXT("TheLastRite", "NextStarterTargetCradle", "inspect the cradle next");
    }

    if (!HasEvidenceLine(TEXT("TRUE - the prayer cards - fused into a crown")))
    {
        return NSLOCTEXT("TheLastRite", "NextStarterTargetPrayerCards", "inspect the prayer mess to finish the opening sweep");
    }

    return NSLOCTEXT("TheLastRite", "NextStarterTargetComplete", "the opening sweep is complete");
}

FText ATheLastRiteGameMode::GetActiveGuidanceText() const
{
    return GetNextMoveText().IsEmpty() ? GetNextStarterTargetText() : GetNextMoveText();
}

void ATheLastRiteGameMode::RebuildFinalReport()
{
    FinalReportLines.Reset();

    if (CasePhase == ETheLastRiteCasePhase::ClosedWin)
    {
        FinalReportLines.Add(TEXT("Case title: Apartment 302"));
        FinalReportLines.Add(TEXT("Demon: Hollow Saint"));
        FinalReportLines.Add(TEXT("Conclusion: the nursery sigil was the correct anchor."));
        FinalReportLines.Add(FString::Printf(TEXT("Why the nursery was correct: %s"), *CorrectAnchorReadText.ToString()));
        FinalReportLines.Add(FString::Printf(TEXT("Why the mirror was wrong: %s"), *WrongAnchorReadText.ToString()));
        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(FString::Printf(TEXT("Confirmed true clues (%d/%d)"), FoundTrueClues, RequiredTrueClues));

        for (const FReportEvidenceEntry& Entry : TrueClueReportEntries)
        {
            if (HasEvidenceLine(Entry.EvidenceLine))
            {
                FinalReportLines.Add(FString::Printf(TEXT("True clue: %s"), Entry.ReportSummary));
            }
        }

        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(FString::Printf(TEXT("Discarded false leads (%d/%d)"), FoundFalseLeads, TotalFalseLeads));

        for (const FReportEvidenceEntry& Entry : FalseLeadReportEntries)
        {
            if (HasEvidenceLine(Entry.EvidenceLine))
            {
                FinalReportLines.Add(FString::Printf(TEXT("False lead: %s"), Entry.ReportSummary));
            }
        }

        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(TEXT("Seal result: front door used and case filed cleanly."));
        FinalReportLines.Add(TEXT("Controls: press R to restart the case or Esc to quit."));
    }
    else if (CasePhase == ETheLastRiteCasePhase::ClosedFail)
    {
        FinalReportLines.Add(TEXT("Case title: Apartment 302"));
        FinalReportLines.Add(TEXT("Demon: Hollow Saint"));
        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(TEXT("What went wrong"));
        FinalReportLines.Add(TEXT("Wrong rite used: mirror circle."));
        FinalReportLines.Add(TEXT("Correct anchor: nursery sigil."));
        FinalReportLines.Add(TEXT("Correct read: the child-facing signs all weighted the nursery side."));
        FinalReportLines.Add(FString::Printf(TEXT("Why the nursery was correct: %s"), *CorrectAnchorReadText.ToString()));
        FinalReportLines.Add(FString::Printf(TEXT("Why the mirror was wrong: %s"), *WrongAnchorReadText.ToString()));
        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(FString::Printf(TEXT("True clue audit (%d/%d)"), FoundTrueClues, RequiredTrueClues));
        for (const FReportEvidenceEntry& Entry : TrueClueReportEntries)
        {
            if (HasEvidenceLine(Entry.EvidenceLine))
            {
                FinalReportLines.Add(FString::Printf(TEXT("True clue confirmed: %s"), Entry.ReportSummary));
            }
            else
            {
                FinalReportLines.Add(FString::Printf(TEXT("True clue missed: %s"), Entry.ReportSummary));
            }
        }
        FinalReportLines.Add(TEXT(""));
        FinalReportLines.Add(FString::Printf(TEXT("False lead audit (%d/%d)"), FoundFalseLeads, TotalFalseLeads));
        for (const FReportEvidenceEntry& Entry : FalseLeadReportEntries)
        {
            if (HasEvidenceLine(Entry.EvidenceLine))
            {
                FinalReportLines.Add(FString::Printf(TEXT("False lead cleared: %s"), Entry.ReportSummary));
            }
            else
            {
                FinalReportLines.Add(FString::Printf(TEXT("False lead unchecked: %s"), Entry.ReportSummary));
            }
        }
        FinalReportLines.Add(TEXT("Outcome: the bait circle was trusted over the child-facing signs."));
        FinalReportLines.Add(TEXT("Recovery: press R, rebuild the opening sweep, then follow the nursery read instead of the mirror bait."));
        FinalReportLines.Add(TEXT("Controls: press R to restart the case or Esc to quit."));
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
