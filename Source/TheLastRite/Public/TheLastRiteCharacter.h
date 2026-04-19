#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TheLastRiteCharacter.generated.h"

class ATheLastRiteInteractable;
class UCameraComponent;

UCLASS()
class THELASTRITE_API ATheLastRiteCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATheLastRiteCharacter();

    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    FText GetInteractionPrompt() const;

protected:
    virtual void BeginPlay() override;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void Interact();
    void RestartRun();
    void QuitGame();
    void UpdateFocusedInteractable();
    ATheLastRiteInteractable* FindBestInteractable(const FVector& ViewLocation, const FVector& ViewDirection, const FCollisionQueryParams& QueryParams) const;
    bool CanKeepFocusedInteractable(const FVector& ViewLocation, const FVector& ViewDirection, const ATheLastRiteInteractable* CandidateInteractable) const;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> CameraComponent;

    TWeakObjectPtr<ATheLastRiteInteractable> FocusedInteractable;
};
