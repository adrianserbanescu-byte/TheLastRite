#pragma once

#include "CoreMinimal.h"
#include "TheLastRiteInteractable.h"
#include "RitualAnchor.generated.h"

class ATheLastRiteCharacter;

UCLASS()
class THELASTRITE_API ARitualAnchor : public ATheLastRiteInteractable
{
    GENERATED_BODY()

public:
    ARitualAnchor();

    virtual FText GetPromptText() const override;
    virtual void Interact(ATheLastRiteCharacter* InteractingCharacter) override;

    void ConfigureAnchor(const FText& InDisplayName, bool bInCorrectAnchor);
    void SetRitualReady(bool bInRitualReady);
    bool IsCorrectAnchor() const;
    bool WasActivated() const;
    void MarkActivated();

private:
    UPROPERTY()
    bool bCorrectAnchor;

    UPROPERTY()
    bool bActivated;

    UPROPERTY()
    bool bRitualReady;
};
