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
    DrawPanel(18.0f, 18.0f, 760.0f, 180.0f, FLinearColor(0.02f, 0.03f, 0.05f, 0.72f));

    DrawText(GameMode->GetCaseTitleText().ToString(), FLinearColor(0.85f, 0.95f, 1.0f), X, Y, SmallFont, 1.15f, false);
    Y += 24.0f;
    DrawText(GameMode->GetTargetText().ToString(), FLinearColor(0.9f, 0.72f, 0.40f), X, Y, SmallFont, 1.05f, false);
    Y += 32.0f;
    Y = DrawWrappedTextLine(GameMode->GetObjectiveText().ToString(), FLinearColor::White, X, Y, 92, SmallFont, 1.15f);
    Y += 8.0f;
    DrawText(GameMode->GetProgressText().ToString(), FLinearColor(0.9f, 0.9f, 0.75f), X, Y, SmallFont, 1.1f, false);
    Y += 24.0f;
    DrawText(TEXT("WASD move | Mouse look | E inspect/use | R restart | Esc quit"), FLinearColor(0.7f, 0.8f, 1.0f), X, Y, SmallFont, 1.0f, false);
    Y += 34.0f;
    Y = DrawWrappedTextLine(GameMode->GetStatusText().ToString(), FLinearColor(1.0f, 0.9f, 0.55f), X, Y, 96, SmallFont, 1.05f);

    const TArray<FString>& EvidenceLines = GameMode->GetEvidenceLines();
    if (!EvidenceLines.IsEmpty())
    {
        const float JournalX = Canvas->ClipX - 360.0f;
        float JournalY = 30.0f;
        DrawPanel(JournalX - 12.0f, 18.0f, 342.0f, 178.0f, FLinearColor(0.02f, 0.03f, 0.05f, 0.72f));
        DrawText(TEXT("Case Notes"), FLinearColor(0.85f, 0.95f, 1.0f), JournalX, JournalY, SmallFont, 1.15f, false);
        JournalY += 24.0f;

        const int32 FirstLine = FMath::Max(0, EvidenceLines.Num() - 6);
        for (int32 Index = FirstLine; Index < EvidenceLines.Num(); ++Index)
        {
            FLinearColor LineColor(0.8f, 0.84f, 0.86f);
            if (EvidenceLines[Index].StartsWith(TEXT("TRUE")))
            {
                LineColor = FLinearColor(0.68f, 1.0f, 0.72f);
            }
            else if (EvidenceLines[Index].StartsWith(TEXT("FALSE")))
            {
                LineColor = FLinearColor(1.0f, 0.62f, 0.36f);
            }
            else if (EvidenceLines[Index].StartsWith(TEXT("RITE")))
            {
                LineColor = FLinearColor(0.95f, 0.82f, 0.35f);
            }

            DrawText(EvidenceLines[Index], LineColor, JournalX, JournalY, SmallFont, 1.0f, false);
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
            DrawPanel(24.0f, Canvas->ClipY - 92.0f, 500.0f, 48.0f, FLinearColor(0.02f, 0.03f, 0.05f, 0.72f));
            DrawText(PromptString, FLinearColor::White, PromptX, PromptY, SmallFont, 1.25f, false);
        }
    }

    const FString Crosshair = TEXT("+");
    const float CrosshairX = (Canvas->ClipX * 0.5f) - 6.0f;
    const float CrosshairY = (Canvas->ClipY * 0.5f) - 12.0f;
    DrawText(Crosshair, bHasInteractionPrompt ? FLinearColor(0.45f, 1.0f, 0.75f) : FLinearColor::White, CrosshairX, CrosshairY, LargeFont, 1.0f, false);

    const FText RecentEvent = GameMode->GetRecentEventText();
    if (!RecentEvent.IsEmpty() && !GameMode->IsCaseResolved())
    {
        const float EventX = Canvas->ClipX * 0.18f;
        float EventY = Canvas->ClipY * 0.64f;
        DrawPanel(EventX - 14.0f, EventY - 14.0f, 620.0f, 92.0f, FLinearColor(0.02f, 0.03f, 0.05f, 0.68f));
        DrawText(TEXT("Finding"), FLinearColor(0.85f, 0.95f, 1.0f), EventX, EventY, SmallFont, 1.15f, false);
        EventY += 26.0f;
        DrawWrappedTextLine(RecentEvent.ToString(), FLinearColor(1.0f, 0.92f, 0.55f), EventX, EventY, 82, SmallFont, 1.2f);
    }

    if (GameMode->IsCaseResolved())
    {
        const FString Ending = GameMode->GetEndingText().ToString();
        const float CenterX = Canvas->ClipX * 0.18f;
        const float CenterY = Canvas->ClipY * 0.28f;
        DrawPanel(CenterX - 22.0f, CenterY - 20.0f, 760.0f, 210.0f, FLinearColor(0.01f, 0.02f, 0.04f, 0.84f));
        DrawText(Ending, GameMode->DidPlayerWin() ? FLinearColor(0.5f, 1.0f, 0.5f) : FLinearColor(1.0f, 0.45f, 0.45f), CenterX, CenterY, LargeFont, 2.0f, false);
        DrawWrappedTextLine(GameMode->GetEndingDetailText().ToString(), FLinearColor::White, CenterX, CenterY + 70.0f, 68, SmallFont, 1.2f);
        DrawText(TEXT("Press R to restart the case"), FLinearColor::White, CenterX, CenterY + 132.0f, SmallFont, 1.25f, false);
    }
}

void ATheLastRiteHUD::DrawPanel(float X, float Y, float Width, float Height, const FLinearColor& Color)
{
    DrawRect(Color, X, Y, Width, Height);
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
