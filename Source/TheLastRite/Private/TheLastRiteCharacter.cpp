#include "TheLastRiteCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TheLastRiteInteractable.h"

namespace
{
    constexpr float InteractionTraceDistance = 520.0f;
    constexpr float InteractionSweepRadius = 52.0f;
    constexpr float FocusStickDistance = 560.0f;
    constexpr float FocusStickCenterlineDistanceSq = 72.0f * 72.0f;
    constexpr float FocusStickCenterlineSlackSq = 20.0f * 20.0f;
    constexpr float FocusStickForwardSlack = 40.0f;
    constexpr int32 DirectHitPriorityBonus = 60;

    struct FInteractionCandidate
    {
        ATheLastRiteInteractable* Interactable = nullptr;
        int32 Priority = TNumericLimits<int32>::Lowest();
        float CenterlineDistanceSq = TNumericLimits<float>::Max();
        float ForwardDistance = TNumericLimits<float>::Max();
    };

    FVector GetInteractableFocusPoint(const ATheLastRiteInteractable* Interactable)
    {
        FVector Origin = FVector::ZeroVector;
        FVector BoxExtent = FVector::ZeroVector;
        if (Interactable != nullptr)
        {
            Interactable->GetActorBounds(true, Origin, BoxExtent);
        }

        return Origin;
    }

    bool BuildInteractionCandidate(
        const FVector& ViewLocation,
        const FVector& ViewDirection,
        const ATheLastRiteInteractable* Interactable,
        const FVector& FocusPoint,
        bool bDirectHit,
        FInteractionCandidate& OutCandidate)
    {
        if (Interactable == nullptr)
        {
            return false;
        }

        const FVector ToFocus = FocusPoint - ViewLocation;
        const float ForwardDistance = FVector::DotProduct(ToFocus, ViewDirection);
        if (ForwardDistance <= 0.0f)
        {
            return false;
        }

        const FVector CenterlineOffset = ToFocus - (ViewDirection * ForwardDistance);
        OutCandidate.Interactable = const_cast<ATheLastRiteInteractable*>(Interactable);
        OutCandidate.Priority = Interactable->GetInteractionFocusPriority() + (bDirectHit ? DirectHitPriorityBonus : 0);
        OutCandidate.CenterlineDistanceSq = CenterlineOffset.SizeSquared();
        OutCandidate.ForwardDistance = ForwardDistance;
        return true;
    }

    bool IsBetterInteractionCandidate(const FInteractionCandidate& Candidate, const FInteractionCandidate& CurrentBest)
    {
        if (CurrentBest.Interactable == nullptr)
        {
            return Candidate.Interactable != nullptr;
        }

        if (Candidate.Priority != CurrentBest.Priority)
        {
            return Candidate.Priority > CurrentBest.Priority;
        }

        if (!FMath::IsNearlyEqual(Candidate.CenterlineDistanceSq, CurrentBest.CenterlineDistanceSq, 1.0f))
        {
            return Candidate.CenterlineDistanceSq < CurrentBest.CenterlineDistanceSq;
        }

        return Candidate.ForwardDistance < CurrentBest.ForwardDistance;
    }
}

ATheLastRiteCharacter::ATheLastRiteCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = true;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->MaxWalkSpeed = 360.0f;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(GetCapsuleComponent());
    CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
    CameraComponent->bUsePawnControlRotation = true;
}

void ATheLastRiteCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        PlayerController->SetInputMode(FInputModeGameOnly());
        PlayerController->bShowMouseCursor = false;
    }
}

void ATheLastRiteCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateFocusedInteractable();
}

void ATheLastRiteCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATheLastRiteCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ATheLastRiteCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATheLastRiteCharacter::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATheLastRiteCharacter::LookUp);

    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ATheLastRiteCharacter::Interact);
    PlayerInputComponent->BindAction(TEXT("Restart"), IE_Pressed, this, &ATheLastRiteCharacter::RestartRun);
    PlayerInputComponent->BindAction(TEXT("Quit"), IE_Pressed, this, &ATheLastRiteCharacter::QuitGame);
}

FText ATheLastRiteCharacter::GetInteractionPrompt() const
{
    if (FocusedInteractable.IsValid())
    {
        return FocusedInteractable->GetPromptText();
    }

    return FText::GetEmpty();
}

void ATheLastRiteCharacter::MoveForward(float Value)
{
    if (FMath::IsNearlyZero(Value) || Controller == nullptr)
    {
        return;
    }

    const FRotator ControlRotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

void ATheLastRiteCharacter::MoveRight(float Value)
{
    if (FMath::IsNearlyZero(Value) || Controller == nullptr)
    {
        return;
    }

    const FRotator ControlRotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void ATheLastRiteCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void ATheLastRiteCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void ATheLastRiteCharacter::Interact()
{
    UpdateFocusedInteractable();

    if (FocusedInteractable.IsValid())
    {
        FocusedInteractable->Interact(this);
    }
}

void ATheLastRiteCharacter::RestartRun()
{
    UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true)));
}

void ATheLastRiteCharacter::QuitGame()
{
    APlayerController* PlayerController = Cast<APlayerController>(Controller);
    UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}

void ATheLastRiteCharacter::UpdateFocusedInteractable()
{
    if (Controller == nullptr)
    {
        FocusedInteractable = nullptr;
        return;
    }

    FVector ViewLocation;
    FRotator ViewRotation;
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

    const FVector ViewDirection = ViewRotation.Vector();
    const FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(TheLastRiteInteractTrace), false, this);
    ATheLastRiteInteractable* CandidateInteractable = FindBestInteractable(ViewLocation, ViewDirection, QueryParams);

    if (CanKeepFocusedInteractable(ViewLocation, ViewDirection, CandidateInteractable))
    {
        return;
    }

    FocusedInteractable = CandidateInteractable;
}

ATheLastRiteInteractable* ATheLastRiteCharacter::FindBestInteractable(const FVector& ViewLocation, const FVector& ViewDirection, const FCollisionQueryParams& QueryParams) const
{
    const FVector TraceEnd = ViewLocation + (ViewDirection * InteractionTraceDistance);
    FInteractionCandidate BestCandidate;

    FHitResult DirectHit;
    if (GetWorld()->LineTraceSingleByChannel(
        DirectHit,
        ViewLocation,
        TraceEnd,
        ECC_Visibility,
        QueryParams))
    {
        if (ATheLastRiteInteractable* DirectInteractable = Cast<ATheLastRiteInteractable>(DirectHit.GetActor()))
        {
            FInteractionCandidate DirectCandidate;
            if (BuildInteractionCandidate(ViewLocation, ViewDirection, DirectInteractable, DirectHit.ImpactPoint, true, DirectCandidate))
            {
                BestCandidate = DirectCandidate;
            }
        }
    }

    TArray<FHitResult> HitResults;
    GetWorld()->SweepMultiByChannel(
        HitResults,
        ViewLocation,
        TraceEnd,
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeSphere(InteractionSweepRadius),
        QueryParams);

    for (const FHitResult& HitResult : HitResults)
    {
        if (ATheLastRiteInteractable* Interactable = Cast<ATheLastRiteInteractable>(HitResult.GetActor()))
        {
            FInteractionCandidate Candidate;
            if (!BuildInteractionCandidate(ViewLocation, ViewDirection, Interactable, HitResult.ImpactPoint, false, Candidate))
            {
                continue;
            }

            if (IsBetterInteractionCandidate(Candidate, BestCandidate))
            {
                BestCandidate = Candidate;
            }
        }
    }

    return BestCandidate.Interactable;
}

bool ATheLastRiteCharacter::CanKeepFocusedInteractable(const FVector& ViewLocation, const FVector& ViewDirection, const ATheLastRiteInteractable* CandidateInteractable) const
{
    ATheLastRiteInteractable* CurrentInteractable = FocusedInteractable.Get();
    if (CurrentInteractable == nullptr || CurrentInteractable == CandidateInteractable)
    {
        return false;
    }

    FInteractionCandidate CurrentCandidate;
    if (!BuildInteractionCandidate(
        ViewLocation,
        ViewDirection,
        CurrentInteractable,
        GetInteractableFocusPoint(CurrentInteractable),
        false,
        CurrentCandidate))
    {
        return false;
    }

    if (CurrentCandidate.ForwardDistance > FocusStickDistance
        || CurrentCandidate.CenterlineDistanceSq > FocusStickCenterlineDistanceSq)
    {
        return false;
    }

    if (CandidateInteractable == nullptr)
    {
        return true;
    }

    FInteractionCandidate Candidate;
    if (!BuildInteractionCandidate(
        ViewLocation,
        ViewDirection,
        CandidateInteractable,
        GetInteractableFocusPoint(CandidateInteractable),
        false,
        Candidate))
    {
        return true;
    }

    if (CurrentCandidate.Priority != Candidate.Priority)
    {
        return CurrentCandidate.Priority > Candidate.Priority;
    }

    return CurrentCandidate.CenterlineDistanceSq <= Candidate.CenterlineDistanceSq + FocusStickCenterlineSlackSq
        && CurrentCandidate.ForwardDistance <= Candidate.ForwardDistance + FocusStickForwardSlack;
}
