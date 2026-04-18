#include "InspectableProp.h"

#include "TheLastRiteCharacter.h"
#include "TheLastRiteGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AInspectableProp::AInspectableProp()
{
    bTrueClue = false;
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
    if (bInspected)
    {
        return FText::Format(
            NSLOCTEXT("TheLastRite", "RecheckPrompt", "Already checked: {0}"),
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

void AInspectableProp::ConfigureProp(const FText& InDisplayName, const FText& InClueText, bool bInIsTrueClue)
{
    SetDisplayName(InDisplayName);
    ClueText = InClueText;
    bTrueClue = bInIsTrueClue;
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

void AInspectableProp::MarkInspected()
{
    bInspected = true;
}
