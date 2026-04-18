#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TheLastRiteHUD.generated.h"

class UFont;

UCLASS()
class THELASTRITE_API ATheLastRiteHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

private:
    float DrawWrappedTextLine(const FString& Text, const FLinearColor& Color, float X, float Y, int32 MaxCharactersPerLine, UFont* Font, float Scale);
};
