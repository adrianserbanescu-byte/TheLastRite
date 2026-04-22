#pragma once

#include "CoreMinimal.h"
#include "TheLastRiteInteractable.h"
#include "SessionToolPickup.generated.h"

class ATheLastRiteCharacter;

enum class ETheLastRiteSessionTool : uint8
{
    HolyBook,
    FilmCamera,
    Censer,
    Crucifix
};

UCLASS()
class THELASTRITE_API ASessionToolPickup : public ATheLastRiteInteractable
{
    GENERATED_BODY()

public:
    ASessionToolPickup();

    virtual FText GetPromptText() const override;
    virtual int32 GetInteractionFocusPriority() const override;
    virtual void Interact(ATheLastRiteCharacter* InteractingCharacter) override;

    void ConfigureTool(ETheLastRiteSessionTool InToolType, const FText& InDisplayName, const FText& InRoleText, const FLinearColor& InColor);
    ETheLastRiteSessionTool GetToolType() const;
    const FText& GetRoleText() const;
    bool IsClaimed() const;
    void MarkClaimed();

private:
    ETheLastRiteSessionTool ToolType;
    FText RoleText;
    FLinearColor ToolColor;
    bool bClaimed;
};
