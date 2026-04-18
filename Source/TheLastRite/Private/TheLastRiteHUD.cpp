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

    Y = DrawWrappedTextLine(GameMode->GetObjectiveText().ToString(), FLinearColor::White, X, Y, 92, SmallFont, 1.15f);
    Y += 8.0f;
    DrawText(GameMode->GetProgressText().ToString(), FLinearColor(0.9f, 0.9f, 0.75f), X, Y, SmallFont, 1.1f, false);
    Y += 24.0f;
    DrawText(TEXT("WASD move | Mouse look | E inspect/use | R restart"), FLinearColor(0.7f, 0.8f, 1.0f), X, Y, SmallFont, 1.0f, false);
    Y += 34.0f;
    Y = DrawWrappedTextLine(GameMode->GetStatusText().ToString(), FLinearColor(1.0f, 0.9f, 0.55f), X, Y, 96, SmallFont, 1.05f);

    const TArray<FString>& EvidenceLines = GameMode->GetEvidenceLines();
    if (!EvidenceLines.IsEmpty())
    {
        const float JournalX = Canvas->ClipX - 360.0f;
        float JournalY = 30.0f;
        DrawText(TEXT("Evidence"), FLinearColor(0.85f, 0.95f, 1.0f), JournalX, JournalY, SmallFont, 1.15f, false);
        JournalY += 24.0f;

        const int32 FirstLine = FMath::Max(0, EvidenceLines.Num() - 6);
        for (int32 Index = FirstLine; Index < EvidenceLines.Num(); ++Index)
        {
            DrawText(EvidenceLines[Index], FLinearColor(0.8f, 0.84f, 0.86f), JournalX, JournalY, SmallFont, 1.0f, false);
            JournalY += 22.0f;
        }
    }

    bool bHasInteractionPrompt = false;
    if (const ATheLastRiteCharacter* Character = Cast<ATheLastRiteCharacter>(GetOwningPawn()))
    {
        const FText Prompt = Character->GetInteractionPrompt();
        if (!Prompt.IsEmpty())
        {
            bHasInteractionPrompt = true;
            const FString PromptString = Prompt.ToString();
            const float PromptX = 40.0f;
            const float PromptY = Canvas->ClipY - 80.0f;
            DrawText(PromptString, FLinearColor::White, PromptX, PromptY, SmallFont, 1.25f, false);
        }
    }

    const FString Crosshair = TEXT("+");
    const float CrosshairX = (Canvas->ClipX * 0.5f) - 6.0f;
    const float CrosshairY = (Canvas->ClipY * 0.5f) - 12.0f;
    DrawText(Crosshair, bHasInteractionPrompt ? FLinearColor(0.45f, 1.0f, 0.75f) : FLinearColor::White, CrosshairX, CrosshairY, LargeFont, 1.0f, false);

    if (GameMode->IsCaseResolved())
    {
        const FString Ending = GameMode->GetEndingText().ToString();
        const float CenterX = Canvas->ClipX * 0.18f;
        const float CenterY = Canvas->ClipY * 0.28f;
        DrawText(Ending, GameMode->DidPlayerWin() ? FLinearColor(0.5f, 1.0f, 0.5f) : FLinearColor(1.0f, 0.45f, 0.45f), CenterX, CenterY, LargeFont, 2.0f, false);
        DrawText(TEXT("Press R to restart the case"), FLinearColor::White, CenterX, CenterY + 48.0f, SmallFont, 1.25f, false);
    }
}

float ATheLastRiteHUD::DrawWrappedTextLine(const FString& Text, const FLinearColor& Color, float X, float Y, int32 MaxCharactersPerLine, UFont* Font, float Scale)
{
    if (Text.IsEmpty())
    {
        return Y;
    }

    TArray<FString> Words;
    Text.ParseIntoArrayWS(Words);

    FString Line;
    const float LineHeight = 24.0f * Scale;
    for (const FString& Word : Words)
    {
        const FString Candidate = Line.IsEmpty() ? Word : FString::Printf(TEXT("%s %s"), *Line, *Word);
        if (Candidate.Len() > MaxCharactersPerLine && !Line.IsEmpty())
        {
            DrawText(Line, Color, X, Y, Font, Scale, false);
            Y += LineHeight;
            Line = Word;
        }
        else
        {
            Line = Candidate;
        }
    }

    if (!Line.IsEmpty())
    {
        DrawText(Line, Color, X, Y, Font, Scale, false);
        Y += LineHeight;
    }

    return Y;
}
