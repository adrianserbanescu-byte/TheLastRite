#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TheLastRiteGameMode.generated.h"

class AInspectableProp;
class APointLight;
class ARitualAnchor;
class ACaseExit;

UENUM()
enum class ETheLastRiteCasePhase : uint8
{
    Investigating,
    RiteReady,
    SealedAwaitingExit,
    ClosedWin,
    ClosedFail
};

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
    void HandleCaseExit(ACaseExit* Exit);

    ETheLastRiteCasePhase GetCasePhase() const;
    FText GetCaseTitleText() const;
    FText GetTargetText() const;
    FText GetCurrentObjectiveText() const;
    FText GetObjectiveText() const;
    FText GetStatusText() const;
    FText GetRecentEventText() const;
    FText GetProgressText() const;
    FText GetDeductionText() const;
    FText GetRitualReadText() const;
    FText GetNextMoveText() const;
    FText GetEndingText() const;
    FText GetEndingDetailText() const;
    FText GetResolvedInteractionText() const;
    FText GetPostCaseControlText() const;
    const TArray<FString>& GetEvidenceLines() const;
    const TArray<FString>& GetFinalReportLines() const;
    bool HasEvidenceLine(const FString& FullLine) const;
    bool IsOpeningSweepComplete() const;
    FLinearColor GetPhasePulseColor() const;
    float GetPhasePulseOpacity() const;

    bool IsCaseResolved() const;
    bool IsCaseClosed() const;
    bool DidPlayerWin() const;

private:
    void BuildRoom();
    void BuildCaseContent();
    void BuildSetDressing();
    void SpawnRoomPiece(const FVector& Location, const FVector& Scale3D, const FRotator& Rotation, const FLinearColor& Color);
    void SpawnLights();
    void UpdateRitualAnchors();
    void UpdateCaseExit();
    void UpdateCasePhaseFromEvidence();
    void UpdateWorldMood();
    void UpdateProgressText();
    void UpdateDeductionText();
    void UpdateRitualReadText();
    void UpdateNextMoveText();
    void RefreshCurrentObjectiveText();
    void RebuildFinalReport();
    int32 GetOpeningSweepCount() const;
    FText GetOpeningSweepStateText() const;
    FText GetNextStarterTargetText() const;
    void TriggerPhasePulse(const FLinearColor& Color, float DurationSeconds);
    void SetStatusText(const FText& NewStatusText);
    void AddEvidenceLine(const FString& NewLine);

    UPROPERTY()
    FText CaseTitleText;

    UPROPERTY()
    FText TargetText;

    UPROPERTY()
    FText CurrentObjectiveText;

    UPROPERTY()
    FText ObjectiveText;

    UPROPERTY()
    FText StatusText;

    UPROPERTY()
    FText RecentEventText;

    UPROPERTY()
    FText ProgressText;

    UPROPERTY()
    FText DeductionText;

    UPROPERTY()
    FText RitualReadText;

    UPROPERTY()
    FText NextMoveText;

    UPROPERTY()
    FText EndingText;

    UPROPERTY()
    FText EndingDetailText;

    UPROPERTY()
    TArray<FString> EvidenceLines;

    UPROPERTY()
    TArray<FString> FinalReportLines;

    UPROPERTY()
    int32 RequiredTrueClues;

    UPROPERTY()
    int32 FoundTrueClues;

    UPROPERTY()
    int32 FoundFalseLeads;

    UPROPERTY()
    int32 TotalFalseLeads;

    UPROPERTY()
    ETheLastRiteCasePhase CasePhase;

    UPROPERTY()
    float RecentEventTimeSeconds;

    UPROPERTY()
    float RecentEventDurationSeconds;

    UPROPERTY()
    float PhasePulseStartedAtSeconds;

    UPROPERTY()
    float PhasePulseDurationSeconds;

    UPROPERTY()
    FLinearColor PhasePulseColor;

    UPROPERTY()
    FVector PlayerSpawnLocation;

    UPROPERTY()
    FRotator PlayerSpawnRotation;

    UPROPERTY()
    TArray<TObjectPtr<APointLight>> CaseLights;

    UPROPERTY()
    TArray<TObjectPtr<ARitualAnchor>> RitualAnchors;

    UPROPERTY()
    TObjectPtr<ACaseExit> CaseExitActor;
};
