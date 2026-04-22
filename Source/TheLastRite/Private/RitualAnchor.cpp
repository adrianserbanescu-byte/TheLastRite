#include "RitualAnchor.h"

#include "TheLastRiteCharacter.h"
#include "TheLastRiteGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ARitualAnchor::ARitualAnchor()
{
    bCorrectAnchor = false;
    bActivated = false;
    bRitualReady = false;
    bRitualInProgress = false;
    bCurrentRitualTarget = false;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(
        TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (MeshFinder.Succeeded())
    {
        GetMeshComponent()->SetStaticMesh(MeshFinder.Object);
    }
}

FText ARitualAnchor::GetPromptText() const
{
    const ATheLastRiteGameMode* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ATheLastRiteGameMode>() : nullptr;
    if (GameMode != nullptr && !bActivated)
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

    if (bActivated)
    {
        if (GameMode != nullptr)
        {
            const FText ResolvedPrompt = GameMode->GetResolvedInteractionText();
            if (!ResolvedPrompt.IsEmpty())
            {
                return ResolvedPrompt;
            }
        }

        return FText::Format(
            NSLOCTEXT("TheLastRite", "SpentAnchorPrompt", "{0} is spent"),
            DisplayName);
    }

    if (!bRitualReady)
    {
        const FText NextMove = GameMode ? GameMode->GetNextMoveText() : FText::GetEmpty();
        if (!NextMove.IsEmpty())
        {
            return FText::Format(
                NSLOCTEXT("TheLastRite", "LockedAnchorPromptDetailed", "Rite locked at {0}. Finish the evidence chain first. {1}"),
                DisplayName,
                NextMove);
        }

        return FText::Format(
            NSLOCTEXT("TheLastRite", "LockedAnchorPrompt", "Find all real clues before using {0}"),
            DisplayName);
    }

    if (bRitualInProgress && bCorrectAnchor && bCurrentRitualTarget && GameMode != nullptr)
    {
        return FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "RitualPromptContinue",
                "Press E - Continue the 4-hand rite at {0}. {1} {2}"),
            DisplayName,
            GameMode->GetRitualSequenceStateText(),
            GameMode->GetRitualSequenceSupportText());
    }

    if (bRitualInProgress && !bCorrectAnchor && GameMode != nullptr)
    {
        return FText::Format(
            NSLOCTEXT(
                "TheLastRite",
                "RitualPromptBreak",
                "Press E - Break the rite at {0}. Warning: {1}"),
            DisplayName,
            GameMode->GetWrongAnchorReadText());
    }

    if (bCorrectAnchor && GameMode != nullptr)
    {
        return FText::Format(
            NSLOCTEXT("TheLastRite", "RitualPromptCorrect", "Press E - Start the 4-hand rite at {0}. {1} {2}"),
            DisplayName,
            GameMode->GetRitualSequenceStateText(),
            GameMode->GetRitualSequenceSupportText());
    }

    return FText::Format(
        NSLOCTEXT("TheLastRite", "RitualPromptWrong", "Press E - Perform the rite at {0}. Warning: {1}"),
        DisplayName,
        GameMode != nullptr ? GameMode->GetWrongAnchorReadText() : FText::GetEmpty());
}

int32 ARitualAnchor::GetInteractionFocusPriority() const
{
    const ATheLastRiteGameMode* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ATheLastRiteGameMode>() : nullptr;
    if (GameMode != nullptr && GameMode->IsCaseResolved())
    {
        return -20;
    }

    if (bActivated)
    {
        return -10;
    }

    if (bRitualInProgress)
    {
        return (bCorrectAnchor && bCurrentRitualTarget) ? 90 : 12;
    }

    if (bRitualReady)
    {
        return bCorrectAnchor ? 70 : 25;
    }

    return 5;
}

void ARitualAnchor::Interact(ATheLastRiteCharacter* InteractingCharacter)
{
    if (ATheLastRiteGameMode* GameMode = GetWorld()->GetAuthGameMode<ATheLastRiteGameMode>())
    {
        GameMode->HandleRitualAnchor(this);
    }
}

void ARitualAnchor::ConfigureAnchor(const FText& InDisplayName, bool bInCorrectAnchor)
{
    SetDisplayName(InDisplayName);
    bCorrectAnchor = bInCorrectAnchor;
    bRitualReady = false;
    bRitualInProgress = false;
    bCurrentRitualTarget = false;

    if (UStaticMesh* RitualMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder")))
    {
        GetMeshComponent()->SetStaticMesh(RitualMesh);
    }

    const FLinearColor BaseColor(0.24f, 0.24f, 0.22f);
    const FLinearColor LabelColor(0.82f, 0.82f, 0.78f);

    ApplyMeshColor(BaseColor);
    UpdateWorldLabel(LabelColor);
}

void ARitualAnchor::SetRitualState(bool bInRitualReady, bool bInRitualInProgress, bool bInCurrentRitualTarget)
{
    if (bActivated)
    {
        return;
    }

    bRitualReady = bInRitualReady;
    bRitualInProgress = bInRitualInProgress;
    bCurrentRitualTarget = bInCurrentRitualTarget;
    const FLinearColor ReadyColor = bCorrectAnchor
        ? FLinearColor(0.64f, 0.72f, 0.30f)
        : FLinearColor(0.42f, 0.18f, 0.18f);
    const FLinearColor ActiveColor = bCorrectAnchor
        ? FLinearColor(0.92f, 0.78f, 0.28f)
        : FLinearColor(0.58f, 0.14f, 0.14f);
    const FLinearColor LockedColor(0.24f, 0.24f, 0.22f);
    const FLinearColor LabelReadyColor = bCorrectAnchor
        ? FLinearColor(0.96f, 0.92f, 0.64f)
        : FLinearColor(1.0f, 0.56f, 0.42f);
    const FLinearColor LabelActiveColor = bCorrectAnchor
        ? FLinearColor(1.0f, 0.96f, 0.78f)
        : FLinearColor(1.0f, 0.46f, 0.40f);
    const FLinearColor LabelLockedColor(0.82f, 0.82f, 0.78f);

    const bool bUseActiveColor = bRitualInProgress && (bCurrentRitualTarget || !bCorrectAnchor);
    ApplyMeshColor(bRitualReady ? (bUseActiveColor ? ActiveColor : ReadyColor) : LockedColor);
    UpdateWorldLabel(bRitualReady ? (bUseActiveColor ? LabelActiveColor : LabelReadyColor) : LabelLockedColor);
}

bool ARitualAnchor::IsCorrectAnchor() const
{
    return bCorrectAnchor;
}

bool ARitualAnchor::WasActivated() const
{
    return bActivated;
}

void ARitualAnchor::MarkActivated()
{
    bActivated = true;
    const FLinearColor ResultColor = bCorrectAnchor ? FLinearColor(0.35f, 1.0f, 0.45f) : FLinearColor(1.0f, 0.18f, 0.18f);
    ApplyMeshColor(ResultColor);
    UpdateWorldLabel(ResultColor);
}
