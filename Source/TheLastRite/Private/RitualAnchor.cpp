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
        return FText::Format(
            NSLOCTEXT("TheLastRite", "LockedAnchorPrompt", "Find all 4 true clues before using {0}"),
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

    const TCHAR* MeshPath = bCorrectAnchor
        ? TEXT("/Engine/BasicShapes/Cylinder.Cylinder")
        : TEXT("/Engine/BasicShapes/Cone.Cone");

    if (UStaticMesh* RitualMesh = LoadObject<UStaticMesh>(nullptr, MeshPath))
    {
        GetMeshComponent()->SetStaticMesh(RitualMesh);
    }

    const FLinearColor BaseColor = bCorrectAnchor
        ? FLinearColor(0.42f, 0.32f, 0.16f)
        : FLinearColor(0.16f, 0.18f, 0.24f);
    const FLinearColor LabelColor = bCorrectAnchor
        ? FLinearColor(0.92f, 0.82f, 0.56f)
        : FLinearColor(0.68f, 0.70f, 0.82f);

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
        ? FLinearColor(0.82f, 0.64f, 0.24f)
        : FLinearColor(0.42f, 0.20f, 0.28f);
    const FLinearColor LockedColor = bCorrectAnchor
        ? FLinearColor(0.42f, 0.32f, 0.16f)
        : FLinearColor(0.16f, 0.18f, 0.24f);
    const FLinearColor LabelReadyColor = bCorrectAnchor
        ? FLinearColor(1.0f, 0.90f, 0.45f)
        : FLinearColor(0.92f, 0.62f, 0.70f);
    const FLinearColor LabelLockedColor = bCorrectAnchor
        ? FLinearColor(0.92f, 0.82f, 0.56f)
        : FLinearColor(0.68f, 0.70f, 0.82f);

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
