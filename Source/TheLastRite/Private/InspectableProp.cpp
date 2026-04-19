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
        case ETheLastRiteCasePhase::ClosedWin:
        case ETheLastRiteCasePhase::ClosedFail:
        {
            const FText ResolvedPrompt = GameMode->GetResolvedInteractionText();
            if (!ResolvedPrompt.IsEmpty())
            {
                return ResolvedPrompt;
            }

            break;
        }
        case ETheLastRiteCasePhase::Investigating:
        case ETheLastRiteCasePhase::RiteReady:
        default:
            break;
        }
    }

    if (bInspected)
    {
        const FText NextMove = GameMode ? GameMode->GetNextMoveText() : FText::GetEmpty();
        if (!NextMove.IsEmpty())
        {
            return FText::Format(
                bTrueClue
                    ? NSLOCTEXT("TheLastRite", "RecheckPromptTrueDetailed", "{0} is already logged as a true sign. {1}")
                    : NSLOCTEXT("TheLastRite", "RecheckPromptFalseDetailed", "{0} is already ruled out as bait. {1}"),
                DisplayName,
                NextMove);
        }

        return FText::Format(
            bTrueClue
                ? NSLOCTEXT("TheLastRite", "RecheckPromptTrue", "{0} is already logged as a true sign.")
                : NSLOCTEXT("TheLastRite", "RecheckPromptFalse", "{0} is already ruled out as bait."),
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

int32 AInspectableProp::GetInteractionFocusPriority() const
{
    if (bInspected)
    {
        return 10;
    }

    const ATheLastRiteGameMode* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ATheLastRiteGameMode>() : nullptr;
    if (GameMode != nullptr && !GameMode->IsOpeningSweepComplete())
    {
        return bOpeningSweepTarget ? 80 : 10;
    }

    return bTrueClue ? 50 : 25;
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

bool AInspectableProp::IsOpeningSweepTarget() const
{
    return bOpeningSweepTarget;
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
