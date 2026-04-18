#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TheLastRiteGameMode.generated.h"

class AInspectableProp;
class ARitualAnchor;

UCLASS()
class THELASTRITE_API ATheLastRiteGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATheLastRiteGameMode();

    virtual void BeginPlay() override;
    virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

    void HandleInspectableProp(AInspectableProp* Prop);
    void HandleRitualAnchor(ARitualAnchor* Anchor);

    FText GetObjectiveText() const;
    FText GetStatusText() const;
    FText GetProgressText() const;
    FText GetEndingText() const;

    bool IsCaseResolved() const;
    bool DidPlayerWin() const;

private:
    void BuildRoom();
    void BuildCaseContent();
    void BuildSetDressing();
    void SpawnRoomPiece(const FVector& Location, const FVector& Scale3D, const FRotator& Rotation);
    void SpawnLights();
    void UpdateProgressText();
    void SetStatusText(const FText& NewStatusText);

    UPROPERTY()
    FText ObjectiveText;

    UPROPERTY()
    FText StatusText;

    UPROPERTY()
    FText ProgressText;

    UPROPERTY()
    FText EndingText;

    UPROPERTY()
    int32 RequiredTrueClues;

    UPROPERTY()
    int32 FoundTrueClues;

    UPROPERTY()
    int32 FoundFalseLeads;

    UPROPERTY()
    bool bCaseResolved;

    UPROPERTY()
    bool bPlayerWon;

    UPROPERTY()
    FVector PlayerSpawnLocation;

    UPROPERTY()
    FRotator PlayerSpawnRotation;
};
