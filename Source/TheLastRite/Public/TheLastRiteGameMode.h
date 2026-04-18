#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TheLastRiteGameMode.generated.h"

class AInspectableProp;
class APointLight;
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
    FText GetRecentEventText() const;
    FText GetProgressText() const;
    FText GetEndingText() const;
    const TArray<FString>& GetEvidenceLines() const;

    bool IsCaseResolved() const;
    bool DidPlayerWin() const;

private:
    void BuildRoom();
    void BuildCaseContent();
    void BuildSetDressing();
    void SpawnRoomPiece(const FVector& Location, const FVector& Scale3D, const FRotator& Rotation, const FLinearColor& Color);
    void SpawnLights();
    void UpdateWorldMood();
    void UpdateProgressText();
    void SetStatusText(const FText& NewStatusText);
    void AddEvidenceLine(const FString& NewLine);

    UPROPERTY()
    FText ObjectiveText;

    UPROPERTY()
    FText StatusText;

    UPROPERTY()
    FText RecentEventText;

    UPROPERTY()
    FText ProgressText;

    UPROPERTY()
    FText EndingText;

    UPROPERTY()
    TArray<FString> EvidenceLines;

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
    float RecentEventTimeSeconds;

    UPROPERTY()
    float RecentEventDurationSeconds;

    UPROPERTY()
    FVector PlayerSpawnLocation;

    UPROPERTY()
    FRotator PlayerSpawnRotation;

    UPROPERTY()
    TArray<TObjectPtr<APointLight>> CaseLights;
};
