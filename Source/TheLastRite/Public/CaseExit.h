#pragma once

#include "CoreMinimal.h"
#include "TheLastRiteInteractable.h"
#include "CaseExit.generated.h"

class ATheLastRiteCharacter;

UCLASS()
class THELASTRITE_API ACaseExit : public ATheLastRiteInteractable
{
    GENERATED_BODY()

public:
    ACaseExit();

    virtual FText GetPromptText() const override;
    virtual void Interact(ATheLastRiteCharacter* InteractingCharacter) override;

    void ConfigureExit(const FText& InDisplayName);
    void SetExitReady(bool bInExitReady);
    void MarkUsed();

private:
    UPROPERTY()
    bool bExitReady;

    UPROPERTY()
    bool bUsed;
};
