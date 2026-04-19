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
    virtual int32 GetInteractionFocusPriority() const override;
    virtual void Interact(ATheLastRiteCharacter* InteractingCharacter) override;

    void ConfigureProp(const FText& InDisplayName, const FText& InClueText, const FText& InEvidenceSummary, bool bInIsTrueClue, bool bInIsOpeningSweepTarget = false);
    bool WasInspected() const;
    bool IsTrueClue() const;
    bool IsOpeningSweepTarget() const;
    const FText& GetClueText() const;
    const FText& GetEvidenceSummary() const;
    void MarkInspected();

private:
    UPROPERTY()
    FText ClueText;

    UPROPERTY()
    FText EvidenceSummary;

    UPROPERTY()
    bool bTrueClue;

    UPROPERTY()
    bool bOpeningSweepTarget;

    UPROPERTY()
    bool bInspected;
};
