#include "Game/ShellPlayerController.h"

#include "Game/ShellGameMode.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void AShellPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = false;
    SetInputMode(FInputModeGameOnly());
}

void AShellPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent == nullptr)
    {
        return;
    }

    InputComponent->BindKey(EKeys::Enter, IE_Pressed, this, &AShellPlayerController::AdvanceShellFlow);
    InputComponent->BindKey(EKeys::SpaceBar, IE_Pressed, this, &AShellPlayerController::AdvanceShellFlow);
    InputComponent->BindKey(EKeys::BackSpace, IE_Pressed, this, &AShellPlayerController::GoBack);
    InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &AShellPlayerController::QuitShell);
}

void AShellPlayerController::AdvanceShellFlow()
{
    if (AShellGameMode* ShellGameMode = GetWorld() ? GetWorld()->GetAuthGameMode<AShellGameMode>() : nullptr)
    {
        ShellGameMode->AdvanceFlow();
    }
}

void AShellPlayerController::GoBack()
{
    if (AShellGameMode* ShellGameMode = GetWorld() ? GetWorld()->GetAuthGameMode<AShellGameMode>() : nullptr)
    {
        if (ShellGameMode->GetFlowState() != EShellFlowState::Title)
        {
            ShellGameMode->ReturnToPrevious();
        }
    }
}

void AShellPlayerController::QuitShell()
{
    UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}
