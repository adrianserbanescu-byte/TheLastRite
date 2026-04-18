#include "TheLastRiteHUD.h"

#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "TheLastRiteCharacter.h"
#include "TheLastRiteGameMode.h"

void ATheLastRiteHUD::DrawHUD()
{
    Super::DrawHUD();

    if (Canvas == nullptr || GEngine == nullptr)
    {
        return;
    }

    const ATheLastRiteGameMode* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ATheLastRiteGameMode>() : nullptr;
    if (GameMode == nullptr)
    {
        return;
    }

    UFont* SmallFont = GEngine->GetSmallFont();
    UFont* LargeFont = GEngine->GetLargeFont();

    float Y = 30.0f;
    const float X = 30.0f;

    DrawText(GameMode->GetObjectiveText().ToString(), FLinearColor::White, X, Y, SmallFont, 1.25f, false);
    Y += 28.0f;
    DrawText(GameMode->GetProgressText().ToString(), FLinearColor(0.9f, 0.9f, 0.75f), X, Y, SmallFont, 1.1f, false);
    Y += 24.0f;
    DrawText(TEXT("WASD move | Mouse look | E inspect/use | R restart"), FLinearColor(0.7f, 0.8f, 1.0f), X, Y, SmallFont, 1.0f, false);
    Y += 34.0f;
    DrawText(GameMode->GetStatusText().ToString(), FLinearColor(1.0f, 0.9f, 0.55f), X, Y, SmallFont, 1.1f, false);

    if (const ATheLastRiteCharacter* Character = Cast<ATheLastRiteCharacter>(GetOwningPawn()))
    {
        const FText Prompt = Character->GetInteractionPrompt();
        if (!Prompt.IsEmpty())
        {
            const FString PromptString = Prompt.ToString();
            const float PromptX = 40.0f;
            const float PromptY = Canvas->ClipY - 80.0f;
            DrawText(PromptString, FLinearColor::White, PromptX, PromptY, SmallFont, 1.25f, false);
        }
    }

    const FString Crosshair = TEXT("+");
    const float CrosshairX = (Canvas->ClipX * 0.5f) - 6.0f;
    const float CrosshairY = (Canvas->ClipY * 0.5f) - 12.0f;
    DrawText(Crosshair, FLinearColor::White, CrosshairX, CrosshairY, LargeFont, 1.0f, false);

    if (GameMode->IsCaseResolved())
    {
        const FString Ending = GameMode->GetEndingText().ToString();
        const float CenterX = Canvas->ClipX * 0.18f;
        const float CenterY = Canvas->ClipY * 0.28f;
        DrawText(Ending, GameMode->DidPlayerWin() ? FLinearColor(0.5f, 1.0f, 0.5f) : FLinearColor(1.0f, 0.45f, 0.45f), CenterX, CenterY, LargeFont, 2.0f, false);
        DrawText(TEXT("Press R to restart the case"), FLinearColor::White, CenterX, CenterY + 48.0f, SmallFont, 1.25f, false);
    }
}
