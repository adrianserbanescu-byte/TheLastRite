#include "InspectableProp.h"

#include "TheLastRiteCharacter.h"
#include "TheLastRiteGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AInspectableProp::AInspectableProp()
{
    bTrueClue = false;
    bOpeningSweepTarget = false;
    bInspected = false;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(
        TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (MeshFinder.Succeeded())
    {
        GetMeshComponent()->SetStaticMesh(MeshFinder.Object);
    }
}

FText AInspectableProp::GetPromptText() const
{
    const ATheLastRiteGameMode* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ATheLastRiteGameMode>() : nullptr;
    if (GameMode != nullptr)
    {
        switch (GameMode->GetCasePhase())
        {
        case ETheLastRiteCasePhase::SealedAwaitingExit:
            return NSLOCTEXT("TheLastRite", "InspectPromptResolvedExit", "Case settled. Leave through the front door.");
        case ETheLastRiteCasePhase::ClosedWin:
            return NSLOCTEXT("TheLastRite", "InspectPromptClosedWin", "Case closed. Press R to restart.");
        case ETheLastRiteCasePhase::ClosedFail:
            return NSLOCTEXT("TheLastRite", "InspectPromptClosedFail", "Case failed. Press R to restart.");
        case ETheLastRiteCasePhase::Investigating:
        case ETheLastRiteCasePhase::RiteReady:
        default:
            break;
        }
    }

    if (bInspected)
    {
        return FText::Format(
            NSLOCTEXT("TheLastRite", "RecheckPrompt", "Already checked: {0}"),
            DisplayName);
    }

    if (bOpeningSweepTarget)
    {
        return FText::Format(
            NSLOCTEXT("TheLastRite", "InspectPromptOpeningSweep", "Press E - Inspect {0} (opening sweep target)"),
            DisplayName);
    }

    if (!bOpeningSweepTarget && GameMode != nullptr && !GameMode->IsOpeningSweepComplete())
    {
        return FText::Format(
            NSLOCTEXT("TheLastRite", "InspectPromptDeferred", "Press E - Inspect {0} (later; finish the opening sweep first)"),
            DisplayName);
    }

    return FText::Format(
        NSLOCTEXT("TheLastRite", "InspectPrompt", "Press E - Inspect {0}"),
        DisplayName);
}

void AInspectableProp::Interact(ATheLastRiteCharacter* InteractingCharacter)
{
    if (ATheLastRiteGameMode* GameMode = GetWorld()->GetAuthGameMode<ATheLastRiteGameMode>())
    {
        GameMode->HandleInspectableProp(this);
    }
}

void AInspectableProp::ConfigureProp(const FText& InDisplayName, const FText& InClueText, const FText& InEvidenceSummary, bool bInIsTrueClue, bool bInIsOpeningSweepTarget)
{
    SetDisplayName(InDisplayName);
    ClueText = InClueText;
    EvidenceSummary = InEvidenceSummary;
    bTrueClue = bInIsTrueClue;
    bOpeningSweepTarget = bInIsOpeningSweepTarget;
    ApplyMeshColor(FLinearColor(0.24f, 0.42f, 0.72f));
    UpdateWorldLabel(bOpeningSweepTarget ? FLinearColor(0.82f, 0.94f, 1.0f) : FLinearColor(0.70f, 0.84f, 1.0f));
}

bool AInspectableProp::WasInspected() const
{
    return bInspected;
}

bool AInspectableProp::IsTrueClue() const
{
    return bTrueClue;
}

const FText& AInspectableProp::GetClueText() const
{
    return ClueText;
}

const FText& AInspectableProp::GetEvidenceSummary() const
{
    return EvidenceSummary;
}

void AInspectableProp::MarkInspected()
{
    bInspected = true;
    const FLinearColor ResultColor = bTrueClue ? FLinearColor(0.78f, 0.94f, 0.82f) : FLinearColor(0.92f, 0.52f, 0.28f);
    ApplyMeshColor(ResultColor);
    UpdateWorldLabel(ResultColor);
}
