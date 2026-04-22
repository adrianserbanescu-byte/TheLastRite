#include "Game/ShellHUD.h"

#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "Game/ShellGameMode.h"

void AShellHUD::DrawHUD()
{
    Super::DrawHUD();

    if (Canvas == nullptr || GEngine == nullptr)
    {
        return;
    }

    const AShellGameMode* ShellGameMode = GetWorld() ? GetWorld()->GetAuthGameMode<AShellGameMode>() : nullptr;
    if (ShellGameMode == nullptr)
    {
        return;
    }

    UFont* SmallFont = GEngine->GetSmallFont();
    UFont* LargeFont = GEngine->GetLargeFont();

    DrawRect(FLinearColor(0.01f, 0.01f, 0.02f, 1.0f), 0.0f, 0.0f, Canvas->ClipX, Canvas->ClipY);

    const FString TitleText = ShellGameMode->GetTitleText().ToString();
    const FString SubtitleText = ShellGameMode->GetSubtitleText().ToString();
    const FString PrimaryPromptText = ShellGameMode->GetPrimaryPromptText().ToString();
    const FString SecondaryPromptText = ShellGameMode->GetSecondaryPromptText().ToString();
    const FString StatusText = ShellGameMode->GetStatusText().ToString();
    const float LineWidth = 760.0f;
    const float PanelX = (Canvas->ClipX * 0.5f) - 420.0f;
    const float PanelY = (Canvas->ClipY * 0.5f) - 190.0f;
    const float TitleHeight = 64.0f;
    const float SubtitleHeight = MeasureWrappedTextHeight(SubtitleText, LineWidth, SmallFont, 1.15f);
    const float PrimaryPromptHeight = MeasureWrappedTextHeight(PrimaryPromptText, LineWidth, SmallFont, 1.08f);
    const float SecondaryPromptHeight = MeasureWrappedTextHeight(SecondaryPromptText, LineWidth, SmallFont, 1.0f);
    const float StatusHeight = MeasureWrappedTextHeight(StatusText, LineWidth, SmallFont, 1.0f);
    const float PanelHeight =
        60.0f +
        TitleHeight +
        24.0f +
        SubtitleHeight +
        24.0f +
        PrimaryPromptHeight +
        12.0f +
        SecondaryPromptHeight +
        20.0f +
        StatusHeight;

    DrawPanel(PanelX, PanelY, 840.0f, PanelHeight, FLinearColor(0.03f, 0.04f, 0.08f, 0.88f));

    float TextY = PanelY + 40.0f;
    DrawText(TitleText, FLinearColor(0.90f, 0.94f, 1.0f), PanelX + 40.0f, TextY, LargeFont, 1.9f, false);
    TextY += TitleHeight;
    DrawRect(FLinearColor(0.77f, 0.62f, 0.28f, 0.95f), PanelX + 40.0f, TextY, 220.0f, 3.0f);
    TextY += 24.0f;
    TextY = DrawWrappedTextLine(SubtitleText, FLinearColor(0.82f, 0.90f, 1.0f), PanelX + 40.0f, TextY, LineWidth, SmallFont, 1.15f);
    TextY += 24.0f;
    TextY = DrawWrappedTextLine(PrimaryPromptText, FLinearColor(0.97f, 0.88f, 0.54f), PanelX + 40.0f, TextY, LineWidth, SmallFont, 1.08f);
    TextY += 12.0f;
    TextY = DrawWrappedTextLine(SecondaryPromptText, FLinearColor(0.72f, 0.80f, 0.94f), PanelX + 40.0f, TextY, LineWidth, SmallFont, 1.0f);
    TextY += 20.0f;
    DrawPanel(PanelX + 30.0f, TextY - 10.0f, 780.0f, StatusHeight + 20.0f, FLinearColor(0.06f, 0.08f, 0.14f, 0.72f));
    DrawWrappedTextLine(StatusText, FLinearColor::White, PanelX + 48.0f, TextY, 744.0f, SmallFont, 1.0f);
}

void AShellHUD::DrawPanel(float X, float Y, float Width, float Height, const FLinearColor& Color)
{
    DrawRect(Color, X, Y, Width, Height);
}

float AShellHUD::DrawWrappedTextLine(const FString& Text, const FLinearColor& Color, float X, float Y, float MaxLineWidth, UFont* Font, float Scale)
{
    if (Text.IsEmpty())
    {
        return Y;
    }

    TArray<FString> Words;
    Text.ParseIntoArrayWS(Words);

    FString Line;
    float LineHeight = 24.0f * Scale;
    if (Font != nullptr)
    {
        float MeasuredWidth = 0.0f;
        GetTextSize(TEXT("Ag"), MeasuredWidth, LineHeight, Font, Scale);
    }

    for (const FString& Word : Words)
    {
        const FString Candidate = Line.IsEmpty() ? Word : FString::Printf(TEXT("%s %s"), *Line, *Word);
        float CandidateWidth = 0.0f;
        float CandidateHeight = 0.0f;
        GetTextSize(Candidate, CandidateWidth, CandidateHeight, Font, Scale);
        if (CandidateWidth > MaxLineWidth && !Line.IsEmpty())
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

float AShellHUD::MeasureWrappedTextHeight(const FString& Text, float MaxLineWidth, UFont* Font, float Scale)
{
    if (Text.IsEmpty())
    {
        return 0.0f;
    }

    TArray<FString> Words;
    Text.ParseIntoArrayWS(Words);

    FString Line;
    int32 LineCount = 0;
    float LineHeight = 24.0f * Scale;
    if (Font != nullptr)
    {
        float MeasuredWidth = 0.0f;
        GetTextSize(TEXT("Ag"), MeasuredWidth, LineHeight, Font, Scale);
    }

    for (const FString& Word : Words)
    {
        const FString Candidate = Line.IsEmpty() ? Word : FString::Printf(TEXT("%s %s"), *Line, *Word);
        float CandidateWidth = 0.0f;
        float CandidateHeight = 0.0f;
        GetTextSize(Candidate, CandidateWidth, CandidateHeight, Font, Scale);
        if (CandidateWidth > MaxLineWidth && !Line.IsEmpty())
        {
            ++LineCount;
            Line = Word;
        }
        else
        {
            Line = Candidate;
        }
    }

    if (!Line.IsEmpty())
    {
        ++LineCount;
    }

    return LineCount * LineHeight;
}
