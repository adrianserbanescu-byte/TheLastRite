#include "TheLastRiteInteractable.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ATheLastRiteInteractable::ATheLastRiteInteractable()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(MeshComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
        TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(CubeMesh.Object);
    }

    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
    MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
}

FText ATheLastRiteInteractable::GetPromptText() const
{
    return FText::Format(
        NSLOCTEXT("TheLastRite", "InteractPrompt", "Press E - Inspect {0}"),
        DisplayName.IsEmpty() ? NSLOCTEXT("TheLastRite", "Something", "something") : DisplayName);
}

void ATheLastRiteInteractable::Interact(ATheLastRiteCharacter* InteractingCharacter)
{
}

void ATheLastRiteInteractable::SetDisplayName(const FText& NewDisplayName)
{
    DisplayName = NewDisplayName;
}

UStaticMeshComponent* ATheLastRiteInteractable::GetMeshComponent() const
{
    return MeshComponent;
}

void ATheLastRiteInteractable::ApplyMeshColor(const FLinearColor& Color)
{
    UMaterialInterface* BasicMaterial = LoadObject<UMaterialInterface>(
        nullptr,
        TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    if (BasicMaterial == nullptr)
    {
        return;
    }

    UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BasicMaterial, this);
    if (DynamicMaterial == nullptr)
    {
        return;
    }

    DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
    MeshComponent->SetMaterial(0, DynamicMaterial);
}
