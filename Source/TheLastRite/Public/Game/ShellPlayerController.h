#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShellPlayerController.generated.h"

UCLASS()
class THELASTRITE_API AShellPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    void AdvanceShellFlow();
    void GoBack();
    void QuitShell();
};
