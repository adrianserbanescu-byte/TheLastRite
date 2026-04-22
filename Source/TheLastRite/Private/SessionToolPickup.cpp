#include "SessionToolPickup.h"

#include "Components/StaticMeshComponent.h"
#include "TheLastRiteCharacter.h"
#include "TheLastRiteGameMode.h"
#include "UObject/ConstructorHelpers.h"

ASessionToolPickup::ASessionToolPickup()
{
    ToolType = ETheLastRiteSessionTool::HolyBook;
    ToolColor = FLinearColor(0.82f, 0.82f, 0.82f);
    bClaimed = false;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(
        TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (MeshFinder.Succeeded())
    {
        GetMeshComponent()->SetStaticMesh(MeshFinder.Object);
    }
}

FText ASessionToolPickup::GetPromptText() const
{
    const ATheLastRiteGameMode* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ATheLastRiteGameMode>() : nullptr;
    if (GameMode != nullptr && GameMode->IsCaseResolved())
    {
        const FText ResolvedPrompt = GameMode->GetResolvedInteractionText();
        if (!ResolvedPrompt.IsEmpty())
        {
            return ResolvedPrompt;
        }
    }

    if (bClaimed)
    {
        return FText::Format(
            NSLOCTEXT("TheLastRite", "ClaimedToolPrompt", "{0} is already claimed. {1} role is covered."),
            DisplayName,
            RoleText);
    }

    return FText::Format(
        NSLOCTEXT("TheLastRite", "ToolPickupPrompt", "Press E - Take {0} ({1})"),
        DisplayName,
        RoleText);
}

int32 ASessionToolPickup::GetInteractionFocusPriority() const
{
    const ATheLastRiteGameMode* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ATheLastRiteGameMode>() : nullptr;
    if (GameMode == nullptr)
    {
        return 10;
    }

    if (GameMode->IsCaseResolved())
    {
        return -15;
    }

    if (bClaimed)
    {
        return -10;
    }

    return GameMode->IsOpeningSweepComplete() ? 70 : 20;
}

void ASessionToolPickup::Interact(ATheLastRiteCharacter* InteractingCharacter)
{
    if (ATheLastRiteGameMode* GameMode = GetWorld()->GetAuthGameMode<ATheLastRiteGameMode>())
    {
        GameMode->HandleToolPickup(this);
    }
}

void ASessionToolPickup::ConfigureTool(ETheLastRiteSessionTool InToolType, const FText& InDisplayName, const FText& InRoleText, const FLinearColor& InColor)
{
    ToolType = InToolType;
    RoleText = InRoleText;
    ToolColor = InColor;
    SetDisplayName(InDisplayName);
    ApplyMeshColor(ToolColor);
    UpdateWorldLabel(ToolColor);
}

ETheLastRiteSessionTool ASessionToolPickup::GetToolType() const
{
    return ToolType;
}

const FText& ASessionToolPickup::GetRoleText() const
{
    return RoleText;
}

bool ASessionToolPickup::IsClaimed() const
{
    return bClaimed;
}

void ASessionToolPickup::MarkClaimed()
{
    bClaimed = true;
    const FLinearColor ClaimedColor = FLinearColor(
        FMath::Clamp(ToolColor.R + 0.18f, 0.0f, 1.0f),
        FMath::Clamp(ToolColor.G + 0.18f, 0.0f, 1.0f),
        FMath::Clamp(ToolColor.B + 0.18f, 0.0f, 1.0f),
        1.0f);
    ApplyMeshColor(ClaimedColor);
    UpdateWorldLabel(ClaimedColor);
}
