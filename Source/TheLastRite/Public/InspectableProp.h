#pragma once

#include "CoreMinimal.h"
#include "TheLastRiteInteractable.h"
#include "InspectableProp.generated.h"

class ATheLastRiteCharacter;

UCLASS()
class THELASTRITE_API AInspectableProp : public ATheLastRiteInteractable
{
    GENERATED_BODY()

public:
    AInspectableProp();

    virtual FText GetPromptText() const override;
    virtual void Interact(ATheLastRiteCharacter* InteractingCharacter) override;

    void ConfigureProp(const FText& InDisplayName, const FText& InClueText, bool bInIsTrueClue);
    bool WasInspected() const;
    bool IsTrueClue() const;
    const FText& GetClueText() const;
    void MarkInspected();

private:
    UPROPERTY()
    FText ClueText;

    UPROPERTY()
    bool bTrueClue;

    UPROPERTY()
    bool bInspected;
};
