#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TheLastRiteHUD.generated.h"

UCLASS()
class THELASTRITE_API ATheLastRiteHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;
};
