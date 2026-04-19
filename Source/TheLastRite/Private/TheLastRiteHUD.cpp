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
    const float WorldTimeSeconds = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    const float PhasePulseOpacity = GameMode->GetPhasePulseOpacity();
    const TArray<FString>& EvidenceLines = GameMode->GetEvidenceLines();

    if (PhasePulseOpacity > 0.0f)
    {
        FLinearColor PulseColor = GameMode->GetPhasePulseColor();
        PulseColor.A = PhasePulseOpacity;
        DrawRect(PulseColor, 0.0f, 0.0f, Canvas->ClipX, Canvas->ClipY);
    }

    if (GameMode->GetCasePhase() == ETheLastRiteCasePhase::ClosedFail)
    {
        DrawRect(FLinearColor(0.22f, 0.02f, 0.02f, 0.18f), 0.0f, 0.0f, Canvas->ClipX, Canvas->ClipY);
    }
    else if (GameMode->GetCasePhase() == ETheLastRiteCasePhase::ClosedWin)
    {
        DrawRect(FLinearColor(0.06f, 0.12f, 0.06f, 0.14f), 0.0f, 0.0f, Canvas->ClipX, Canvas->ClipY);
    }

    float Y = 30.0f;
    const float X = 30.0f;
    DrawPanel(18.0f, 18.0f, 760.0f, 232.0f, FLinearColor(0.02f, 0.03f, 0.05f, 0.72f));

    DrawText(GameMode->GetCaseTitleText().ToString(), FLinearColor(0.85f, 0.95f, 1.0f), X, Y, SmallFont, 1.15f, false);
    Y += 24.0f;
    DrawText(GameMode->GetTargetText().ToString(), FLinearColor(0.9f, 0.72f, 0.40f), X, Y, SmallFont, 1.05f, false);
    Y += 32.0f;
    Y = DrawWrappedTextLine(GameMode->GetObjectiveText().ToString(), FLinearColor::White, X, Y, 92, SmallFont, 1.15f);
    Y += 8.0f;
    Y = DrawWrappedTextLine(GameMode->GetCurrentObjectiveText().ToString(), FLinearColor(0.65f, 0.90f, 1.0f), X, Y, 92, SmallFont, 1.05f);
    Y += 6.0f;
    DrawText(GameMode->GetProgressText().ToString(), FLinearColor(0.9f, 0.9f, 0.75f), X, Y, SmallFont, 1.1f, false);
    Y += 24.0f;
    Y = DrawWrappedTextLine(GameMode->GetDeductionText().ToString(), FLinearColor(0.78f, 0.90f, 1.0f), X, Y, 92, SmallFont, 1.0f);
    Y += 6.0f;
    DrawText(TEXT("WASD move | Mouse look | E inspect/use | R restart | Esc quit"), FLinearColor(0.7f, 0.8f, 1.0f), X, Y, SmallFont, 1.0f, false);
    Y += 34.0f;
    Y = DrawWrappedTextLine(GameMode->GetStatusText().ToString(), FLinearColor(1.0f, 0.9f, 0.55f), X, Y, 96, SmallFont, 1.05f);

    const bool bShowStarterGuidance = !GameMode->IsCaseResolved() && (WorldTimeSeconds < 12.0f || EvidenceLines.IsEmpty());
    if (bShowStarterGuidance)
    {
        const float IntroX = Canvas->ClipX * 0.22f;
        const float IntroY = Canvas->ClipY * 0.18f;
        DrawPanel(IntroX - 24.0f, IntroY - 26.0f, 760.0f, 264.0f, FLinearColor(0.01f, 0.02f, 0.04f, 0.82f));
        DrawText(GameMode->GetCaseTitleText().ToString(), FLinearColor(0.85f, 0.95f, 1.0f), IntroX, IntroY, LargeFont, 1.35f, false);
        DrawText(GameMode->GetTargetText().ToString(), FLinearColor(0.95f, 0.78f, 0.42f), IntroX, IntroY + 40.0f, SmallFont, 1.25f, false);
        DrawWrappedTextLine(GameMode->GetObjectiveText().ToString(), FLinearColor::White, IntroX, IntroY + 74.0f, 62, SmallFont, 1.2f);
        DrawWrappedTextLine(GameMode->GetCurrentObjectiveText().ToString(), FLinearColor(0.72f, 0.90f, 1.0f), IntroX, IntroY + 132.0f, 62, SmallFont, 1.1f);
        DrawText(TEXT("White labels mark usable objects. Start at the body, then cradle, then prayer mess."), FLinearColor(0.78f, 0.86f, 1.0f), IntroX, IntroY + 174.0f, SmallFont, 1.05f, false);
        DrawText(TEXT("The notes panel on the right tracks true clues, false leads, and the current read."), FLinearColor(0.78f, 0.86f, 1.0f), IntroX, IntroY + 204.0f, SmallFont, 1.05f, false);
    }

    {
        const float JournalX = Canvas->ClipX - 360.0f;
        const float JournalPanelTop = 18.0f;
        const float JournalPanelHeight = FMath::Max(560.0f, Canvas->ClipY - 52.0f);
        float JournalY = 30.0f;
        DrawPanel(JournalX - 12.0f, JournalPanelTop, 342.0f, JournalPanelHeight, FLinearColor(0.02f, 0.03f, 0.05f, 0.72f));
        DrawText(TEXT("Case Notes"), FLinearColor(0.85f, 0.95f, 1.0f), JournalX, JournalY, SmallFont, 1.15f, false);
        JournalY += 24.0f;

        auto DrawChecklistEntry = [&](const FString& FullLine, const FString& Label, bool bGood)
        {
            const bool bFound = GameMode->HasEvidenceLine(FullLine);
            const FString Prefix = bFound ? TEXT("[x] ") : TEXT("[ ] ");
            const FLinearColor LineColor = bFound
                ? (bGood ? FLinearColor(0.68f, 1.0f, 0.72f) : FLinearColor(1.0f, 0.72f, 0.44f))
                : FLinearColor(0.62f, 0.67f, 0.74f);
            DrawText(Prefix + Label, LineColor, JournalX, JournalY, SmallFont, 0.92f, false);
            JournalY += 22.0f;
        };

        DrawText(TEXT("Opening sweep"), FLinearColor(0.85f, 0.95f, 1.0f), JournalX, JournalY, SmallFont, 1.1f, false);
        JournalY += 22.0f;
        DrawChecklistEntry(TEXT("TRUE - Nanny Eliza - mirrored wrist marks"), TEXT("Body - Nanny Eliza"), true);
        DrawChecklistEntry(TEXT("TRUE - the cradle - halo of ash-white handprints"), TEXT("Cradle"), true);
        DrawChecklistEntry(TEXT("TRUE - the prayer cards - fused into a crown"), TEXT("Prayer mess"), true);
        JournalY += 10.0f;

        if (EvidenceLines.IsEmpty())
        {
            JournalY = DrawWrappedTextLine(TEXT("No notes yet. Start with the body, the cradle, and the prayer mess."), FLinearColor(0.95f, 0.88f, 0.60f), JournalX, JournalY, 36, SmallFont, 0.98f);
            JournalY += 8.0f;
            JournalY = DrawWrappedTextLine(TEXT("White labels mark the objects you can inspect."), FLinearColor(0.78f, 0.86f, 1.0f), JournalX, JournalY, 36, SmallFont, 0.95f);
            JournalY += 12.0f;
        }
        else
        {
            const int32 FirstLine = FMath::Max(0, EvidenceLines.Num() - 4);
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

                JournalY = DrawWrappedTextLine(EvidenceLines[Index], LineColor, JournalX, JournalY, 36, SmallFont, 0.95f);
                JournalY += 4.0f;
            }
        }

        JournalY += 10.0f;
        DrawText(TEXT("Current read"), FLinearColor(0.85f, 0.95f, 1.0f), JournalX, JournalY, SmallFont, 1.1f, false);
        JournalY += 24.0f;
        JournalY = DrawWrappedTextLine(GameMode->GetDeductionText().ToString(), FLinearColor(0.76f, 0.88f, 1.0f), JournalX, JournalY, 38, SmallFont, 0.95f);
        JournalY += 10.0f;

        DrawText(TEXT("Ritual read"), FLinearColor(0.85f, 0.95f, 1.0f), JournalX, JournalY, SmallFont, 1.1f, false);
        JournalY += 24.0f;
        JournalY = DrawWrappedTextLine(GameMode->GetRitualReadText().ToString(), FLinearColor(0.92f, 0.84f, 0.50f), JournalX, JournalY, 38, SmallFont, 0.95f);
        JournalY += 10.0f;

        DrawText(TEXT("Next move"), FLinearColor(0.85f, 0.95f, 1.0f), JournalX, JournalY, SmallFont, 1.1f, false);
        JournalY += 24.0f;
        JournalY = DrawWrappedTextLine(GameMode->GetNextMoveText().ToString(), FLinearColor(0.95f, 0.88f, 0.60f), JournalX, JournalY, 38, SmallFont, 0.95f);
        JournalY += 12.0f;

        DrawText(TEXT("Later checks"), FLinearColor(0.85f, 0.95f, 1.0f), JournalX, JournalY, SmallFont, 1.1f, false);
        JournalY += 22.0f;
        DrawChecklistEntry(TEXT("TRUE - the baby monitor - hymn repeating on every channel"), TEXT("Monitor hymn loop"), true);
        DrawChecklistEntry(TEXT("TRUE - the nursery wallpaper - child's sun turned into a halo"), TEXT("Nursery mural halo"), true);
        JournalY += 10.0f;
        DrawText(TEXT("False leads"), FLinearColor(0.85f, 0.95f, 1.0f), JournalX, JournalY, SmallFont, 1.1f, false);
        JournalY += 22.0f;
        DrawChecklistEntry(TEXT("FALSE - the broken window latch - forced from outside"), TEXT("Broken window latch"), false);
        DrawChecklistEntry(TEXT("FALSE - the pawn ticket pouch - ordinary greed"), TEXT("Pawn ticket pouch"), false);
        DrawChecklistEntry(TEXT("FALSE - the kitchen knife - grease, not offering blood"), TEXT("Kitchen knife"), false);
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
        DrawPanel(EventX - 14.0f, EventY - 14.0f, 680.0f, 92.0f, FLinearColor(0.02f, 0.03f, 0.05f, 0.68f));
        DrawText(TEXT("Finding"), FLinearColor(0.85f, 0.95f, 1.0f), EventX, EventY, SmallFont, 1.15f, false);
        EventY += 26.0f;
        DrawWrappedTextLine(RecentEvent.ToString(), FLinearColor(1.0f, 0.92f, 0.55f), EventX, EventY, 82, SmallFont, 1.2f);
    }

    if (GameMode->GetCasePhase() == ETheLastRiteCasePhase::SealedAwaitingExit)
    {
        const float BannerX = Canvas->ClipX * 0.33f;
        const float BannerY = Canvas->ClipY * 0.16f;
        DrawPanel(BannerX - 20.0f, BannerY - 18.0f, 520.0f, 94.0f, FLinearColor(0.03f, 0.08f, 0.04f, 0.80f));
        DrawText(TEXT("SEAL HOLDS"), FLinearColor(0.58f, 1.0f, 0.64f), BannerX, BannerY, LargeFont, 1.35f, false);
        DrawText(TEXT("Leave through the front door now."), FLinearColor::White, BannerX, BannerY + 40.0f, SmallFont, 1.15f, false);
    }
    else if (GameMode->GetCasePhase() == ETheLastRiteCasePhase::RiteReady)
    {
        const float BannerX = Canvas->ClipX * 0.30f;
        const float BannerY = Canvas->ClipY * 0.16f;
        DrawPanel(BannerX - 20.0f, BannerY - 18.0f, 600.0f, 96.0f, FLinearColor(0.10f, 0.08f, 0.02f, 0.80f));
        DrawText(TEXT("RITE READY"), FLinearColor(1.0f, 0.88f, 0.44f), BannerX, BannerY, LargeFont, 1.35f, false);
        DrawText(TEXT("Act on the child-facing pattern. Ignore the mirror bait."), FLinearColor::White, BannerX, BannerY + 40.0f, SmallFont, 1.12f, false);
    }

    if (GameMode->IsCaseClosed())
    {
        const FString Ending = GameMode->GetEndingText().ToString();
        const float CenterX = Canvas->ClipX * 0.18f;
        const float CenterY = Canvas->ClipY * 0.28f;
        float ReportY = CenterY + 108.0f;
        const TArray<FString>& FinalReportLines = GameMode->GetFinalReportLines();
        float OverlayHeight = 220.0f;
        if (!FinalReportLines.IsEmpty())
        {
            float ReportBodyHeight = 26.0f;
            for (const FString& Line : FinalReportLines)
            {
                ReportBodyHeight += Line.IsEmpty()
                    ? 8.0f
                    : (MeasureWrappedTextHeight(Line, 68, 1.0f) + 2.0f);
            }

            OverlayHeight = FMath::Clamp(108.0f + ReportBodyHeight + 72.0f, 320.0f, Canvas->ClipY - (CenterY - 20.0f) - 24.0f);
        }
        DrawPanel(CenterX - 22.0f, CenterY - 20.0f, 760.0f, OverlayHeight, FLinearColor(0.01f, 0.02f, 0.04f, 0.84f));
        DrawText(Ending, GameMode->DidPlayerWin() ? FLinearColor(0.5f, 1.0f, 0.5f) : FLinearColor(1.0f, 0.45f, 0.45f), CenterX, CenterY, LargeFont, 2.0f, false);
        DrawWrappedTextLine(GameMode->GetEndingDetailText().ToString(), FLinearColor::White, CenterX, CenterY + 70.0f, 68, SmallFont, 1.2f);

        if (!FinalReportLines.IsEmpty())
        {
            DrawPanel(CenterX - 10.0f, ReportY - 10.0f, 710.0f, OverlayHeight - 176.0f, FLinearColor(0.04f, 0.05f, 0.08f, 0.62f));
            DrawText(TEXT("Final report"), FLinearColor(0.85f, 0.95f, 1.0f), CenterX, ReportY, SmallFont, 1.15f, false);
            ReportY += 26.0f;

            for (const FString& Line : FinalReportLines)
            {
                FLinearColor ReportColor(0.82f, 0.86f, 0.90f);
                if (Line.IsEmpty())
                {
                    ReportY += 8.0f;
                    continue;
                }
                else if (Line.Contains(TEXT("missed")) || Line.Contains(TEXT("unchecked")))
                {
                    ReportColor = FLinearColor(0.95f, 0.88f, 0.60f);
                }
                else if (Line.Contains(TEXT("Conclusion")) || Line.Contains(TEXT("correct anchor")) || Line.Contains(TEXT("Seal result")) || Line.Contains(TEXT("True clue")))
                {
                    ReportColor = FLinearColor(0.68f, 1.0f, 0.72f);
                }
                else if (Line.Contains(TEXT("Correct read")))
                {
                    ReportColor = FLinearColor(0.76f, 0.88f, 1.0f);
                }
                else if (Line.Contains(TEXT("Wrong rite")) || Line.Contains(TEXT("Outcome")) || Line.Contains(TEXT("What went wrong")))
                {
                    ReportColor = FLinearColor(1.0f, 0.45f, 0.45f);
                }
                else if (Line.Contains(TEXT("Recovery")))
                {
                    ReportColor = FLinearColor(0.95f, 0.88f, 0.60f);
                }
                else if (Line.Contains(TEXT("False lead")) || Line.Contains(TEXT("Discarded false leads")))
                {
                    ReportColor = FLinearColor(1.0f, 0.72f, 0.44f);
                }
                else if (Line.Contains(TEXT("Confirmed true clues")) || Line.Contains(TEXT("Case title")) || Line.Contains(TEXT("Demon")))
                {
                    ReportColor = FLinearColor(0.86f, 0.92f, 1.0f);
                }

                ReportY = DrawWrappedTextLine(Line, ReportColor, CenterX, ReportY, 68, SmallFont, 1.0f);
                ReportY += 2.0f;
            }
        }

        DrawText(GameMode->GetPostCaseControlText().ToString(), FLinearColor::White, CenterX, CenterY + OverlayHeight - 48.0f, SmallFont, 1.25f, false);
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

float ATheLastRiteHUD::MeasureWrappedTextHeight(const FString& Text, int32 MaxCharactersPerLine, float Scale) const
{
    if (Text.IsEmpty())
    {
        return 0.0f;
    }

    TArray<FString> Words;
    Text.ParseIntoArrayWS(Words);

    FString Line;
    int32 LineCount = 0;
    for (const FString& Word : Words)
    {
        const FString Candidate = Line.IsEmpty() ? Word : FString::Printf(TEXT("%s %s"), *Line, *Word);
        if (Candidate.Len() > MaxCharactersPerLine && !Line.IsEmpty())
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

    return LineCount * 24.0f * Scale;
}
