#include "TheLastRiteGameMode.h"

#include "InspectableProp.h"
#include "RitualAnchor.h"
#include "TheLastRiteCharacter.h"
#include "TheLastRiteHUD.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/PointLight.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"

namespace
{
    static UStaticMesh* LoadBasicMesh(const TCHAR* AssetPath)
    {
        return LoadObject<UStaticMesh>(nullptr, AssetPath);
    }
}

ATheLastRiteGameMode::ATheLastRiteGameMode()
{
    DefaultPawnClass = ATheLastRiteCharacter::StaticClass();
    PlayerControllerClass = APlayerController::StaticClass();
    HUDClass = ATheLastRiteHUD::StaticClass();
    bStartPlayersAsSpectators = false;

    RequiredTrueClues = 3;
    FoundTrueClues = 0;
    FoundFalseLeads = 0;
    bCaseResolved = false;
    bPlayerWon = false;

    PlayerSpawnLocation = FVector(0.0f, 0.0f, 140.0f);
    PlayerSpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

    ObjectiveText = NSLOCTEXT(
        "TheLastRite",
        "Objective",
        "Apartment 302. The babysitter vanished. Find 3 real clues tied to the Hollow Saint, then perform the rite.");
    StatusText = NSLOCTEXT(
        "TheLastRite",
        "StatusInitial",
        "Search the room. Real clues carry the saint's pale halo pattern.");
    ProgressText = FText::GetEmpty();
    EndingText = FText::GetEmpty();
}

void ATheLastRiteGameMode::BeginPlay()
{
    Super::BeginPlay();

    BuildRoom();
    BuildSetDressing();
    BuildCaseContent();
    SpawnLights();
    UpdateWorldMood();
    UpdateProgressText();
}

void ATheLastRiteGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    Super::HandleStartingNewPlayer_Implementation(NewPlayer);

    if (NewPlayer != nullptr && NewPlayer->GetPawn() != nullptr)
    {
        NewPlayer->SetControlRotation(PlayerSpawnRotation);
        NewPlayer->GetPawn()->SetActorLocation(PlayerSpawnLocation);
        NewPlayer->GetPawn()->SetActorRotation(PlayerSpawnRotation);
    }
}

void ATheLastRiteGameMode::HandleInspectableProp(AInspectableProp* Prop)
{
    if (Prop == nullptr || bCaseResolved)
    {
        return;
    }

    if (Prop->WasInspected())
    {
        SetStatusText(FText::Format(
            NSLOCTEXT("TheLastRite", "AlreadyChecked", "You already checked this. {0}"),
            Prop->GetClueText()));
        return;
    }

    Prop->MarkInspected();

    if (Prop->IsTrueClue())
    {
        FoundTrueClues = FMath::Min(FoundTrueClues + 1, RequiredTrueClues);
        AddEvidenceLine(FString::Printf(TEXT("TRUE - %s"), *Prop->GetDisplayName().ToString()));
        if (FoundTrueClues >= RequiredTrueClues)
        {
            ObjectiveText = NSLOCTEXT(
                "TheLastRite",
                "ObjectiveRitualReady",
                "Evidence complete. Choose the ritual anchor that matches the Hollow Saint.");
            SetStatusText(FText::Format(
                NSLOCTEXT("TheLastRite", "EnoughClues", "{0} You have enough evidence. Choose the correct ritual anchor."),
                Prop->GetClueText()));
        }
        else
        {
            SetStatusText(FText::Format(
                NSLOCTEXT("TheLastRite", "TrueClue", "{0} That is a true sign of the Hollow Saint."),
                Prop->GetClueText()));
        }
    }
    else
    {
        ++FoundFalseLeads;
        AddEvidenceLine(FString::Printf(TEXT("FALSE - %s"), *Prop->GetDisplayName().ToString()));
        SetStatusText(FText::Format(
            NSLOCTEXT("TheLastRite", "FalseLead", "{0} False lead. It looks ugly, but it is not the saint."),
            Prop->GetClueText()));
    }

    UpdateProgressText();
    UpdateWorldMood();
}

void ATheLastRiteGameMode::HandleRitualAnchor(ARitualAnchor* Anchor)
{
    if (Anchor == nullptr)
    {
        return;
    }

    if (bCaseResolved)
    {
        return;
    }

    if (FoundTrueClues < RequiredTrueClues)
    {
        SetStatusText(FText::Format(
            NSLOCTEXT("TheLastRite", "NeedMoreClues", "Not yet. You still need {0} real clue(s) before the rite."),
            FText::AsNumber(RequiredTrueClues - FoundTrueClues)));
        return;
    }

    Anchor->MarkActivated();
    bCaseResolved = true;
    bPlayerWon = Anchor->IsCorrectAnchor();

    if (bPlayerWon)
    {
        ObjectiveText = NSLOCTEXT(
            "TheLastRite",
            "ObjectiveWon",
            "Case closed. The Hollow Saint was forced out of Apartment 302.");
        EndingText = NSLOCTEXT(
            "TheLastRite",
            "WinEnding",
            "RITE COMPLETE\nThe Hollow Saint is driven out.");
        SetStatusText(NSLOCTEXT(
            "TheLastRite",
            "WinStatus",
            "You chose the nursery sigil. The rite bites into the halo and the room goes still."));
        AddEvidenceLine(TEXT("RITE - correct anchor"));
    }
    else
    {
        ObjectiveText = NSLOCTEXT(
            "TheLastRite",
            "ObjectiveLost",
            "Case failed. The wrong rite fed the Hollow Saint.");
        EndingText = NSLOCTEXT(
            "TheLastRite",
            "LoseEnding",
            "WRONG RITE\nThe saint answers first.");
        SetStatusText(NSLOCTEXT(
            "TheLastRite",
            "LoseStatus",
            "You chose the wrong anchor. The false circle feeds the thing you were hunting."));
        AddEvidenceLine(TEXT("RITE - wrong anchor"));
    }

    UpdateWorldMood();
}

FText ATheLastRiteGameMode::GetObjectiveText() const
{
    return ObjectiveText;
}

FText ATheLastRiteGameMode::GetStatusText() const
{
    return StatusText;
}

FText ATheLastRiteGameMode::GetProgressText() const
{
    return ProgressText;
}

FText ATheLastRiteGameMode::GetEndingText() const
{
    return EndingText;
}

const TArray<FString>& ATheLastRiteGameMode::GetEvidenceLines() const
{
    return EvidenceLines;
}

bool ATheLastRiteGameMode::IsCaseResolved() const
{
    return bCaseResolved;
}

bool ATheLastRiteGameMode::DidPlayerWin() const
{
    return bPlayerWon;
}

void ATheLastRiteGameMode::BuildRoom()
{
    const FLinearColor FloorColor(0.18f, 0.18f, 0.20f);
    const FLinearColor WallColor(0.33f, 0.36f, 0.38f);

    SpawnRoomPiece(FVector(0.0f, 0.0f, 0.0f), FVector(14.0f, 14.0f, 0.25f), FRotator::ZeroRotator, FloorColor);
    SpawnRoomPiece(FVector(0.0f, 0.0f, 600.0f), FVector(14.0f, 14.0f, 0.25f), FRotator::ZeroRotator, WallColor);
    SpawnRoomPiece(FVector(1400.0f, 0.0f, 300.0f), FVector(0.25f, 14.0f, 6.0f), FRotator::ZeroRotator, WallColor);
    SpawnRoomPiece(FVector(-1400.0f, 0.0f, 300.0f), FVector(0.25f, 14.0f, 6.0f), FRotator::ZeroRotator, WallColor);
    SpawnRoomPiece(FVector(0.0f, 1400.0f, 300.0f), FVector(14.0f, 0.25f, 6.0f), FRotator::ZeroRotator, WallColor);
    SpawnRoomPiece(FVector(0.0f, -1400.0f, 300.0f), FVector(14.0f, 0.25f, 6.0f), FRotator::ZeroRotator, WallColor);
}

void ATheLastRiteGameMode::BuildCaseContent()
{
    auto SpawnProp = [this](const FVector& Location, const FVector& Scale3D, const FText& Name, const FText& Clue, bool bTrueClue)
    {
        AInspectableProp* Prop = GetWorld()->SpawnActor<AInspectableProp>(Location, FRotator::ZeroRotator);
        if (Prop != nullptr)
        {
            Prop->SetActorScale3D(Scale3D);
            Prop->ConfigureProp(Name, Clue, bTrueClue);
        }
    };

    auto SpawnAnchor = [this](const FVector& Location, const FText& Name, bool bCorrectAnchor)
    {
        ARitualAnchor* Anchor = GetWorld()->SpawnActor<ARitualAnchor>(Location, FRotator::ZeroRotator);
        if (Anchor != nullptr)
        {
            Anchor->SetActorScale3D(FVector(1.8f, 1.8f, 0.2f));
            Anchor->ConfigureAnchor(Name, bCorrectAnchor);
        }
    };

    SpawnProp(
        FVector(0.0f, -780.0f, 95.0f),
        FVector(0.7f, 0.7f, 1.9f),
        NSLOCTEXT("TheLastRite", "NannyName", "Nanny Eliza"),
        NSLOCTEXT("TheLastRite", "NannyClue", "Her dry wrist marks mirror each other too perfectly, like a saint pose forced onto skin."),
        true);

    SpawnProp(
        FVector(430.0f, 250.0f, 60.0f),
        FVector(1.2f, 1.8f, 0.8f),
        NSLOCTEXT("TheLastRite", "CradleName", "the cradle"),
        NSLOCTEXT("TheLastRite", "CradleClue", "Ash-white handprints ring the cradle in a perfect halo."),
        true);

    SpawnProp(
        FVector(-480.0f, 320.0f, 40.0f),
        FVector(0.8f, 0.8f, 0.8f),
        NSLOCTEXT("TheLastRite", "PrayerCardsName", "the prayer cards"),
        NSLOCTEXT("TheLastRite", "PrayerCardsClue", "The prayer cards are fused into a crown-like circle."),
        true);

    SpawnProp(
        FVector(160.0f, -520.0f, 55.0f),
        FVector(0.5f, 0.5f, 1.1f),
        NSLOCTEXT("TheLastRite", "MonitorName", "the baby monitor"),
        NSLOCTEXT("TheLastRite", "MonitorClue", "The monitor hums the same hymn on every channel."),
        true);

    SpawnProp(
        FVector(-520.0f, -450.0f, 70.0f),
        FVector(1.4f, 0.2f, 1.4f),
        NSLOCTEXT("TheLastRite", "WindowName", "the broken window latch"),
        NSLOCTEXT("TheLastRite", "WindowClue", "The latch is busted from outside. It is violence, not ritual."),
        false);

    SpawnProp(
        FVector(620.0f, -180.0f, 35.0f),
        FVector(0.8f, 0.8f, 0.3f),
        NSLOCTEXT("TheLastRite", "TicketName", "the pawn ticket pouch"),
        NSLOCTEXT("TheLastRite", "TicketClue", "A pawn ticket pouch sits near the sink. It is ordinary greed."),
        false);

    SpawnAnchor(
        FVector(730.0f, 620.0f, 20.0f),
        NSLOCTEXT("TheLastRite", "CorrectAnchor", "the nursery sigil"),
        true);

    SpawnAnchor(
        FVector(-730.0f, -620.0f, 20.0f),
        NSLOCTEXT("TheLastRite", "WrongAnchor", "the mirror circle"),
        false);
}

void ATheLastRiteGameMode::BuildSetDressing()
{
    const FLinearColor FurnitureColor(0.42f, 0.31f, 0.23f);
    const FLinearColor PaleColor(0.65f, 0.68f, 0.72f);
    const FLinearColor ClothColor(0.12f, 0.11f, 0.13f);
    const FLinearColor OldGoldColor(0.76f, 0.61f, 0.22f);
    const FLinearColor BloodlessSkinColor(0.68f, 0.66f, 0.62f);
    const FLinearColor MirrorColor(0.22f, 0.26f, 0.31f);

    SpawnRoomPiece(FVector(0.0f, -780.0f, 18.0f), FVector(2.7f, 1.2f, 0.08f), FRotator::ZeroRotator, ClothColor);
    SpawnRoomPiece(FVector(-52.0f, -780.0f, 65.0f), FVector(0.35f, 0.45f, 0.62f), FRotator::ZeroRotator, BloodlessSkinColor);
    SpawnRoomPiece(FVector(28.0f, -780.0f, 50.0f), FVector(0.75f, 0.36f, 0.16f), FRotator::ZeroRotator, BloodlessSkinColor);
    SpawnRoomPiece(FVector(0.0f, -780.0f, 35.0f), FVector(2.0f, 0.04f, 0.04f), FRotator::ZeroRotator, OldGoldColor);
    SpawnRoomPiece(FVector(0.0f, -780.0f, 38.0f), FVector(2.0f, 0.04f, 0.04f), FRotator(0.0f, 45.0f, 0.0f), OldGoldColor);
    SpawnRoomPiece(FVector(0.0f, -780.0f, 41.0f), FVector(2.0f, 0.04f, 0.04f), FRotator(0.0f, 90.0f, 0.0f), OldGoldColor);
    SpawnRoomPiece(FVector(0.0f, -780.0f, 44.0f), FVector(2.0f, 0.04f, 0.04f), FRotator(0.0f, 135.0f, 0.0f), OldGoldColor);

    SpawnRoomPiece(FVector(860.0f, 620.0f, 110.0f), FVector(1.8f, 0.9f, 1.1f), FRotator::ZeroRotator, FurnitureColor);
    SpawnRoomPiece(FVector(860.0f, 860.0f, 80.0f), FVector(0.8f, 0.8f, 0.8f), FRotator::ZeroRotator, PaleColor);
    SpawnRoomPiece(FVector(-920.0f, 780.0f, 145.0f), FVector(0.6f, 3.6f, 2.9f), FRotator::ZeroRotator, FurnitureColor);
    SpawnRoomPiece(FVector(-780.0f, -830.0f, 110.0f), FVector(2.4f, 0.8f, 1.1f), FRotator::ZeroRotator, FurnitureColor);
    SpawnRoomPiece(FVector(-860.0f, -520.0f, 55.0f), FVector(1.0f, 1.0f, 0.5f), FRotator::ZeroRotator, PaleColor);
    SpawnRoomPiece(FVector(0.0f, 980.0f, 145.0f), FVector(4.4f, 0.15f, 2.9f), FRotator::ZeroRotator, PaleColor);
    SpawnRoomPiece(FVector(420.0f, 980.0f, 145.0f), FVector(0.15f, 1.2f, 2.9f), FRotator::ZeroRotator, PaleColor);
    SpawnRoomPiece(FVector(-250.0f, -920.0f, 60.0f), FVector(1.1f, 1.1f, 0.5f), FRotator::ZeroRotator, FurnitureColor);
    SpawnRoomPiece(FVector(-1330.0f, 0.0f, 150.0f), FVector(0.08f, 2.1f, 3.0f), FRotator::ZeroRotator, MirrorColor);
    SpawnRoomPiece(FVector(730.0f, 620.0f, 30.0f), FVector(2.9f, 0.05f, 0.05f), FRotator::ZeroRotator, OldGoldColor);
    SpawnRoomPiece(FVector(730.0f, 620.0f, 34.0f), FVector(2.9f, 0.05f, 0.05f), FRotator(0.0f, 90.0f, 0.0f), OldGoldColor);
    SpawnRoomPiece(FVector(-730.0f, -620.0f, 30.0f), FVector(2.9f, 0.05f, 0.05f), FRotator(0.0f, 45.0f, 0.0f), MirrorColor);
    SpawnRoomPiece(FVector(-730.0f, -620.0f, 34.0f), FVector(2.9f, 0.05f, 0.05f), FRotator(0.0f, 135.0f, 0.0f), MirrorColor);
}

void ATheLastRiteGameMode::SpawnRoomPiece(const FVector& Location, const FVector& Scale3D, const FRotator& Rotation, const FLinearColor& Color)
{
    AStaticMeshActor* Piece = GetWorld()->SpawnActor<AStaticMeshActor>(Location, Rotation);
    if (Piece == nullptr)
    {
        return;
    }

    UStaticMeshComponent* MeshComponent = Piece->GetStaticMeshComponent();
    if (MeshComponent == nullptr)
    {
        return;
    }

    if (UStaticMesh* CubeMesh = LoadBasicMesh(TEXT("/Engine/BasicShapes/Cube.Cube")))
    {
        MeshComponent->SetStaticMesh(CubeMesh);
    }

    if (UMaterialInterface* BaseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")))
    {
        UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, Piece);
        if (DynamicMaterial != nullptr)
        {
            DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
            MeshComponent->SetMaterial(0, DynamicMaterial);
        }
    }

    Piece->SetActorScale3D(Scale3D);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
}

void ATheLastRiteGameMode::SpawnLights()
{
    APointLight* CenterLight = GetWorld()->SpawnActor<APointLight>(FVector(0.0f, 0.0f, 480.0f), FRotator::ZeroRotator);
    if (CenterLight != nullptr)
    {
        CaseLights.Add(CenterLight);
        CenterLight->SetActorScale3D(FVector(6.0f, 6.0f, 6.0f));
        if (UPointLightComponent* LightComponent = Cast<UPointLightComponent>(CenterLight->GetLightComponent()))
        {
            LightComponent->Intensity = 18000.0f;
            LightComponent->AttenuationRadius = 2500.0f;
        }
    }

    APointLight* NurseryLight = GetWorld()->SpawnActor<APointLight>(FVector(700.0f, 650.0f, 220.0f), FRotator::ZeroRotator);
    if (NurseryLight != nullptr)
    {
        CaseLights.Add(NurseryLight);
        NurseryLight->SetActorScale3D(FVector(3.0f, 3.0f, 3.0f));
        if (UPointLightComponent* LightComponent = Cast<UPointLightComponent>(NurseryLight->GetLightComponent()))
        {
            LightComponent->Intensity = 8000.0f;
            LightComponent->AttenuationRadius = 1200.0f;
        }
    }
}

void ATheLastRiteGameMode::UpdateWorldMood()
{
    FLinearColor LightColor(0.96f, 0.92f, 0.82f);
    float IntensityScale = 1.0f;

    if (bCaseResolved)
    {
        LightColor = bPlayerWon ? FLinearColor(0.65f, 1.0f, 0.72f) : FLinearColor(1.0f, 0.20f, 0.14f);
        IntensityScale = bPlayerWon ? 1.15f : 0.55f;
    }
    else
    {
        const float ClueProgress = RequiredTrueClues > 0 ? static_cast<float>(FoundTrueClues) / static_cast<float>(RequiredTrueClues) : 0.0f;
        LightColor = FMath::Lerp(FLinearColor(0.96f, 0.92f, 0.82f), FLinearColor(1.0f, 0.78f, 0.30f), ClueProgress);
        IntensityScale = FMath::Clamp(1.0f - (static_cast<float>(FoundFalseLeads) * 0.16f), 0.62f, 1.15f);
    }

    for (int32 Index = 0; Index < CaseLights.Num(); ++Index)
    {
        APointLight* Light = CaseLights[Index];
        if (Light == nullptr)
        {
            continue;
        }

        if (UPointLightComponent* LightComponent = Cast<UPointLightComponent>(Light->GetLightComponent()))
        {
            const float BaseIntensity = Index == 0 ? 18000.0f : 8000.0f;
            LightComponent->SetIntensity(BaseIntensity * IntensityScale);
            LightComponent->SetLightColor(LightColor);
        }
    }
}

void ATheLastRiteGameMode::UpdateProgressText()
{
    ProgressText = FText::Format(
        NSLOCTEXT("TheLastRite", "Progress", "True clues: {0}/{1} | False leads: {2}/2"),
        FText::AsNumber(FoundTrueClues),
        FText::AsNumber(RequiredTrueClues),
        FText::AsNumber(FoundFalseLeads));
}

void ATheLastRiteGameMode::SetStatusText(const FText& NewStatusText)
{
    StatusText = NewStatusText;
}

void ATheLastRiteGameMode::AddEvidenceLine(const FString& NewLine)
{
    EvidenceLines.Add(NewLine);
}
