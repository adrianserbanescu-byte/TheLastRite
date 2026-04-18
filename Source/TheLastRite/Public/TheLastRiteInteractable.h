#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TheLastRiteInteractable.generated.h"

class ATheLastRiteCharacter;
class UStaticMeshComponent;

UCLASS(Abstract)
class THELASTRITE_API ATheLastRiteInteractable : public AActor
{
    GENERATED_BODY()

public:
    ATheLastRiteInteractable();

    virtual FText GetPromptText() const;
    virtual void Interact(ATheLastRiteCharacter* InteractingCharacter);

    void SetDisplayName(const FText& NewDisplayName);
    UStaticMeshComponent* GetMeshComponent() const;

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    UPROPERTY()
    FText DisplayName;
};
