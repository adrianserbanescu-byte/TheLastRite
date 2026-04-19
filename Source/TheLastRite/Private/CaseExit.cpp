#include "CaseExit.h"

#include "TheLastRiteCharacter.h"
#include "TheLastRiteGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ACaseExit::ACaseExit()
{
    bExitReady = false;
    bUsed = false;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(
        TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (MeshFinder.Succeeded())
    {
        GetMeshComponent()->SetStaticMesh(MeshFinder.Object);
    }
}

FText ACaseExit::GetPromptText() const
{
    if (bUsed)
    {
        return FText::Format(
            NSLOCTEXT("TheLastRite", "ExitSpentPrompt", "{0} is already behind you"),
            DisplayName);
    }

    if (!bExitReady)
    {
        const ATheLastRiteGameMode* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ATheLastRiteGameMode>() : nullptr;
        const FText NextMove = GameMode ? GameMode->GetNextMoveText() : FText::GetEmpty();
        if (!NextMove.IsEmpty())
        {
            return FText::Format(
                NSLOCTEXT("TheLastRite", "ExitLockedPromptDetailed", "{0} stays shut. {1}"),
                DisplayName,
                NextMove);
        }

        return FText::Format(
            NSLOCTEXT("TheLastRite", "ExitLockedPrompt", "Finish the rite before leaving through {0}"),
            DisplayName);
    }

    return FText::Format(
        NSLOCTEXT("TheLastRite", "ExitPrompt", "Press E - Leave through {0} and file the case"),
        DisplayName);
}

int32 ACaseExit::GetInteractionFocusPriority() const
{
    if (bUsed)
    {
        return -10;
    }

    return bExitReady ? 70 : 0;
}

void ACaseExit::Interact(ATheLastRiteCharacter* InteractingCharacter)
{
    if (bUsed)
    {
        return;
    }

    if (ATheLastRiteGameMode* GameMode = GetWorld()->GetAuthGameMode<ATheLastRiteGameMode>())
    {
        GameMode->HandleCaseExit(this);
    }
}

void ACaseExit::ConfigureExit(const FText& InDisplayName)
{
    SetDisplayName(InDisplayName);
    ApplyMeshColor(FLinearColor(0.20f, 0.15f, 0.09f));
    UpdateWorldLabel(FLinearColor(0.85f, 0.78f, 0.60f));
}

void ACaseExit::SetExitReady(bool bInExitReady)
{
    if (bUsed)
    {
        return;
    }

    bExitReady = bInExitReady;
    ApplyMeshColor(bExitReady ? FLinearColor(0.36f, 0.78f, 0.58f) : FLinearColor(0.20f, 0.15f, 0.09f));
    UpdateWorldLabel(bExitReady ? FLinearColor(0.72f, 1.0f, 0.80f) : FLinearColor(0.85f, 0.78f, 0.60f));
}

void ACaseExit::MarkUsed()
{
    bUsed = true;
    ApplyMeshColor(FLinearColor(0.92f, 0.90f, 0.82f));
    UpdateWorldLabel(FLinearColor(0.96f, 0.96f, 0.90f));
}
