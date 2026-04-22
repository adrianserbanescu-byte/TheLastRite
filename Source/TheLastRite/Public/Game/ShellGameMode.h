#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShellGameMode.generated.h"

UENUM()
enum class EShellFlowState : uint8
{
    Title,
    Lobby,
    Briefing,
    ToolGrab,
    CaseLaunch
};

UCLASS()
class THELASTRITE_API AShellGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AShellGameMode();
    virtual void BeginPlay() override;

    EShellFlowState GetFlowState() const;
    FText GetTitleText() const;
    FText GetSubtitleText() const;
    FText GetPrimaryPromptText() const;
    FText GetSecondaryPromptText() const;
    FText GetStatusText() const;

    void AdvanceFlow();
    void ReturnToPrevious();
    void StartSession();

private:
    void RunAutomatedShellVerification();
    void RefreshShellText();

    UPROPERTY()
    EShellFlowState FlowState;

    UPROPERTY()
    FText TitleText;

    UPROPERTY()
    FText SubtitleText;

    UPROPERTY()
    FText PrimaryPromptText;

    UPROPERTY()
    FText SecondaryPromptText;

    UPROPERTY()
    FText StatusText;
};
