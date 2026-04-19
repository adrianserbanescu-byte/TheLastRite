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

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(
        TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (MeshFinder.Succeeded())
    {
        GetMeshComponent()->SetStaticMesh(MeshFinder.Object);
    }
}

FText ARitualAnchor::GetPromptText() const
{
    if (bActivated)
    {
        return FText::Format(
            NSLOCTEXT("TheLastRite", "SpentAnchorPrompt", "{0} is spent"),
            DisplayName);
    }

    if (!bRitualReady)
    {
        const ATheLastRiteGameMode* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ATheLastRiteGameMode>() : nullptr;
        const FText NextMove = GameMode ? GameMode->GetNextMoveText() : FText::GetEmpty();
        if (!NextMove.IsEmpty())
        {
            return FText::Format(
                NSLOCTEXT("TheLastRite", "LockedAnchorPromptDetailed", "Rite locked at {0}. {1}"),
                DisplayName,
                NextMove);
        }

        return FText::Format(
            NSLOCTEXT("TheLastRite", "LockedAnchorPrompt", "Find all real clues before using {0}"),
            DisplayName);
    }

    return FText::Format(
        NSLOCTEXT("TheLastRite", "RitualPrompt", "Press E - Perform the rite at {0}"),
        DisplayName);
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

    if (UStaticMesh* RitualMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder")))
    {
        GetMeshComponent()->SetStaticMesh(RitualMesh);
    }

    const FLinearColor BaseColor(0.24f, 0.24f, 0.22f);
    const FLinearColor LabelColor(0.82f, 0.82f, 0.78f);

    ApplyMeshColor(BaseColor);
    UpdateWorldLabel(LabelColor);
}

void ARitualAnchor::SetRitualReady(bool bInRitualReady)
{
    if (bActivated)
    {
        return;
    }

    bRitualReady = bInRitualReady;
    const FLinearColor ReadyColor = bCorrectAnchor
        ? FLinearColor(0.56f, 0.52f, 0.34f)
        : FLinearColor(0.50f, 0.46f, 0.40f);
    const FLinearColor LockedColor(0.24f, 0.24f, 0.22f);
    const FLinearColor LabelReadyColor = bCorrectAnchor
        ? FLinearColor(0.96f, 0.88f, 0.62f)
        : FLinearColor(0.86f, 0.82f, 0.76f);
    const FLinearColor LabelLockedColor(0.82f, 0.82f, 0.78f);

    ApplyMeshColor(bRitualReady ? ReadyColor : LockedColor);
    UpdateWorldLabel(bRitualReady ? LabelReadyColor : LabelLockedColor);
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
