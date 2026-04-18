#include "TheLastRiteCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TheLastRiteInteractable.h"

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

    const FVector TraceEnd = ViewLocation + (ViewRotation.Vector() * 400.0f);

    TArray<FHitResult> HitResults;
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(TheLastRiteInteractTrace), false, this);
    GetWorld()->SweepMultiByChannel(
        HitResults,
        ViewLocation,
        TraceEnd,
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeSphere(42.0f),
        QueryParams);

    FocusedInteractable = nullptr;
    for (const FHitResult& HitResult : HitResults)
    {
        if (ATheLastRiteInteractable* Interactable = Cast<ATheLastRiteInteractable>(HitResult.GetActor()))
        {
            FocusedInteractable = Interactable;
            return;
        }
    }
}
