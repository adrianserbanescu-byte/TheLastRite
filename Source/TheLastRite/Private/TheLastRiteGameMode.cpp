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
    BuildCaseContent();
    SpawnLights();
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
        ++FoundTrueClues;
        if (FoundTrueClues >= RequiredTrueClues)
        {
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
        SetStatusText(FText::Format(
            NSLOCTEXT("TheLastRite", "FalseLead", "{0} False lead. It looks ugly, but it is not the saint."),
            Prop->GetClueText()));
    }

    UpdateProgressText();
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
        EndingText = NSLOCTEXT(
            "TheLastRite",
            "WinEnding",
            "RITE COMPLETE\nThe Hollow Saint is driven out.");
        SetStatusText(NSLOCTEXT(
            "TheLastRite",
            "WinStatus",
            "You chose the nursery sigil. The rite bites into the halo and the room goes still."));
    }
    else
    {
        EndingText = NSLOCTEXT(
            "TheLastRite",
            "LoseEnding",
            "WRONG RITE\nThe saint answers first.");
        SetStatusText(NSLOCTEXT(
            "TheLastRite",
            "LoseStatus",
            "You chose the wrong anchor. The false circle feeds the thing you were hunting."));
    }
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
    SpawnRoomPiece(FVector(0.0f, 0.0f, 0.0f), FVector(14.0f, 14.0f, 0.25f), FRotator::ZeroRotator);
    SpawnRoomPiece(FVector(0.0f, 0.0f, 600.0f), FVector(14.0f, 14.0f, 0.25f), FRotator::ZeroRotator);
    SpawnRoomPiece(FVector(1400.0f, 0.0f, 300.0f), FVector(0.25f, 14.0f, 6.0f), FRotator::ZeroRotator);
    SpawnRoomPiece(FVector(-1400.0f, 0.0f, 300.0f), FVector(0.25f, 14.0f, 6.0f), FRotator::ZeroRotator);
    SpawnRoomPiece(FVector(0.0f, 1400.0f, 300.0f), FVector(14.0f, 0.25f, 6.0f), FRotator::ZeroRotator);
    SpawnRoomPiece(FVector(0.0f, -1400.0f, 300.0f), FVector(14.0f, 0.25f, 6.0f), FRotator::ZeroRotator);
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

void ATheLastRiteGameMode::SpawnRoomPiece(const FVector& Location, const FVector& Scale3D, const FRotator& Rotation)
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

    Piece->SetActorScale3D(Scale3D);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
}

void ATheLastRiteGameMode::SpawnLights()
{
    APointLight* CenterLight = GetWorld()->SpawnActor<APointLight>(FVector(0.0f, 0.0f, 480.0f), FRotator::ZeroRotator);
    if (CenterLight != nullptr)
    {
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
        NurseryLight->SetActorScale3D(FVector(3.0f, 3.0f, 3.0f));
        if (UPointLightComponent* LightComponent = Cast<UPointLightComponent>(NurseryLight->GetLightComponent()))
        {
            LightComponent->Intensity = 8000.0f;
            LightComponent->AttenuationRadius = 1200.0f;
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
