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

    const float X = 30.0f;
    const FString ObjectiveText = GameMode->GetObjectiveText().ToString();
    const FString CurrentObjectiveText = GameMode->GetCurrentObjectiveText().ToString();
    const FString FocusZoneText = GameMode->GetFocusZoneText().ToString();
    const FString TheoryChainText = GameMode->GetTheoryChainText().ToString();
    const FString RiskText = GameMode->GetRiskText().ToString();
    const FString ProgressText = GameMode->GetProgressText().ToString();
    const FString DeductionText = GameMode->GetDeductionText().ToString();
    const FString StatusText = GameMode->GetStatusText().ToString();
    const FString ControlText = TEXT("WASD move | Mouse look | E inspect/use | R restart | Esc to quit");
    const float MainPanelLineWidth = 700.0f;
    const float StatusLineWidth = 720.0f;
    const float ObjectiveHeight = MeasureWrappedTextHeight(ObjectiveText, MainPanelLineWidth, SmallFont, 1.15f);
    const float CurrentObjectiveHeight = MeasureWrappedTextHeight(CurrentObjectiveText, MainPanelLineWidth, SmallFont, 1.05f);
    const float FocusZoneHeight = MeasureWrappedTextHeight(FocusZoneText, MainPanelLineWidth, SmallFont, 1.0f);
    const float TheoryChainHeight = MeasureWrappedTextHeight(TheoryChainText, MainPanelLineWidth, SmallFont, 1.0f);
    const float RiskHeight = MeasureWrappedTextHeight(RiskText, MainPanelLineWidth, SmallFont, 1.0f);
    const float ProgressHeight = MeasureWrappedTextHeight(ProgressText, MainPanelLineWidth, SmallFont, 1.1f);
    const float DeductionHeight = MeasureWrappedTextHeight(DeductionText, MainPanelLineWidth, SmallFont, 1.0f);
    const float ControlHeight = MeasureWrappedTextHeight(ControlText, MainPanelLineWidth, SmallFont, 1.0f);
    const float StatusHeight = MeasureWrappedTextHeight(StatusText, StatusLineWidth, SmallFont, 1.05f);
    const float MainPanelHeight = FMath::Max(
        232.0f,
        12.0f
            + 24.0f
            + 32.0f
            + ObjectiveHeight
            + 8.0f
            + CurrentObjectiveHeight
            + 6.0f
            + FocusZoneHeight
            + 6.0f
            + TheoryChainHeight
            + 6.0f
            + RiskHeight
            + 6.0f
            + ProgressHeight
            + 8.0f
            + DeductionHeight
            + 6.0f
            + ControlHeight
            + 10.0f
            + StatusHeight
            + 24.0f);

    float Y = 30.0f;
    DrawPanel(18.0f, 18.0f, 760.0f, MainPanelHeight, FLinearColor(0.02f, 0.03f, 0.05f, 0.72f));

    DrawText(GameMode->GetCaseTitleText().ToString(), FLinearColor(0.85f, 0.95f, 1.0f), X, Y, SmallFont, 1.15f, false);
    Y += 24.0f;
    DrawText(GameMode->GetTargetText().ToString(), FLinearColor(0.9f, 0.72f, 0.40f), X, Y, SmallFont, 1.05f, false);
    Y += 32.0f;
    Y = DrawWrappedTextLine(ObjectiveText, FLinearColor::White, X, Y, MainPanelLineWidth, SmallFont, 1.15f);
    Y += 8.0f;
    Y = DrawWrappedTextLine(CurrentObjectiveText, FLinearColor(0.65f, 0.90f, 1.0f), X, Y, MainPanelLineWidth, SmallFont, 1.05f);
    Y += 6.0f;
    Y = DrawWrappedTextLine(FocusZoneText, FLinearColor(0.94f, 0.90f, 0.62f), X, Y, MainPanelLineWidth, SmallFont, 1.0f);
    Y += 6.0f;
    Y = DrawWrappedTextLine(TheoryChainText, FLinearColor(0.82f, 0.90f, 1.0f), X, Y, MainPanelLineWidth, SmallFont, 1.0f);
    Y += 6.0f;
    Y = DrawWrappedTextLine(RiskText, FLinearColor(1.0f, 0.76f, 0.56f), X, Y, MainPanelLineWidth, SmallFont, 1.0f);
    Y += 6.0f;
    Y = DrawWrappedTextLine(ProgressText, FLinearColor(0.9f, 0.9f, 0.75f), X, Y, MainPanelLineWidth, SmallFont, 1.1f);
    Y += 8.0f;
    Y = DrawWrappedTextLine(DeductionText, FLinearColor(0.78f, 0.90f, 1.0f), X, Y, MainPanelLineWidth, SmallFont, 1.0f);
    Y += 6.0f;
    Y = DrawWrappedTextLine(ControlText, FLinearColor(0.7f, 0.8f, 1.0f), X, Y, MainPanelLineWidth, SmallFont, 1.0f);
    Y += 10.0f;
    Y = DrawWrappedTextLine(StatusText, FLinearColor(1.0f, 0.9f, 0.55f), X, Y, StatusLineWidth, SmallFont, 1.05f);

    const bool bShowStarterGuidance = !GameMode->IsCaseResolved() && !GameMode->IsOpeningSweepComplete();
    if (bShowStarterGuidance)
    {
        const float IntroX = Canvas->ClipX * 0.22f;
        const float IntroY = Canvas->ClipY * 0.18f;
        const FString IntroReminderText = TEXT("White labels mark usable objects. The notes panel tracks what is real, what is bait, and what to check next.");
        const float IntroLineWidth = 700.0f;
        const float ObjectiveY = IntroY + 74.0f;
        const float IntroObjectiveHeight = MeasureWrappedTextHeight(GameMode->GetObjectiveText().ToString(), IntroLineWidth, SmallFont, 1.2f);
        const float CurrentObjectiveY = ObjectiveY + IntroObjectiveHeight + 10.0f;
        const float IntroCurrentObjectiveHeight = MeasureWrappedTextHeight(GameMode->GetCurrentObjectiveText().ToString(), IntroLineWidth, SmallFont, 1.1f);
        const float NextMoveY = CurrentObjectiveY + IntroCurrentObjectiveHeight + 14.0f;
        const float NextMoveHeight = MeasureWrappedTextHeight(GameMode->GetNextMoveText().ToString(), IntroLineWidth, SmallFont, 1.05f);
        const float ReminderY = NextMoveY + NextMoveHeight + 14.0f;
        const float ReminderHeight = MeasureWrappedTextHeight(IntroReminderText, IntroLineWidth, SmallFont, 1.05f);
        const float IntroPanelHeight = (ReminderY - (IntroY - 26.0f)) + ReminderHeight + 24.0f;

        DrawPanel(IntroX - 24.0f, IntroY - 26.0f, 760.0f, IntroPanelHeight, FLinearColor(0.01f, 0.02f, 0.04f, 0.82f));
        DrawText(GameMode->GetCaseTitleText().ToString(), FLinearColor(0.85f, 0.95f, 1.0f), IntroX, IntroY, LargeFont, 1.35f, false);
        DrawText(GameMode->GetTargetText().ToString(), FLinearColor(0.95f, 0.78f, 0.42f), IntroX, IntroY + 40.0f, SmallFont, 1.25f, false);
        DrawWrappedTextLine(GameMode->GetObjectiveText().ToString(), FLinearColor::White, IntroX, ObjectiveY, IntroLineWidth, SmallFont, 1.2f);
        DrawWrappedTextLine(GameMode->GetCurrentObjectiveText().ToString(), FLinearColor(0.72f, 0.90f, 1.0f), IntroX, CurrentObjectiveY, IntroLineWidth, SmallFont, 1.1f);
        DrawWrappedTextLine(GameMode->GetNextMoveText().ToString(), FLinearColor(0.95f, 0.88f, 0.60f), IntroX, NextMoveY, IntroLineWidth, SmallFont, 1.05f);
        DrawWrappedTextLine(IntroReminderText, FLinearColor(0.78f, 0.86f, 1.0f), IntroX, ReminderY, IntroLineWidth, SmallFont, 1.05f);
    }

    {
        const float JournalX = Canvas->ClipX - 360.0f;
        const float JournalPanelTop = 18.0f;
        const float JournalPanelHeight = FMath::Max(560.0f, Canvas->ClipY - 52.0f);
        const float JournalLineWidth = 300.0f;
        const bool bShowAnchorRead = GameMode->IsOpeningSweepComplete();
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
            JournalY = DrawWrappedTextLine(TEXT("No notes yet. Start with the body, the cradle, and the prayer mess."), FLinearColor(0.95f, 0.88f, 0.60f), JournalX, JournalY, JournalLineWidth, SmallFont, 0.98f);
            JournalY += 8.0f;
            JournalY = DrawWrappedTextLine(TEXT("White labels mark the objects you can inspect."), FLinearColor(0.78f, 0.86f, 1.0f), JournalX, JournalY, JournalLineWidth, SmallFont, 0.95f);
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

                JournalY = DrawWrappedTextLine(EvidenceLines[Index], LineColor, JournalX, JournalY, JournalLineWidth, SmallFont, 0.95f);
                JournalY += 4.0f;
            }
        }

        JournalY += 10.0f;
        DrawText(TEXT("Current read"), FLinearColor(0.85f, 0.95f, 1.0f), JournalX, JournalY, SmallFont, 1.1f, false);
        JournalY += 24.0f;
        JournalY = DrawWrappedTextLine(GameMode->GetDeductionText().ToString(), FLinearColor(0.76f, 0.88f, 1.0f), JournalX, JournalY, JournalLineWidth, SmallFont, 0.95f);
        JournalY += 10.0f;

        DrawText(TEXT("Ritual read"), FLinearColor(0.85f, 0.95f, 1.0f), JournalX, JournalY, SmallFont, 1.1f, false);
        JournalY += 24.0f;
        JournalY = DrawWrappedTextLine(GameMode->GetRitualReadText().ToString(), FLinearColor(0.92f, 0.84f, 0.50f), JournalX, JournalY, JournalLineWidth, SmallFont, 0.95f);
        JournalY += 10.0f;

        if (bShowAnchorRead)
        {
            DrawText(TEXT("Anchor read"), FLinearColor(0.85f, 0.95f, 1.0f), JournalX, JournalY, SmallFont, 1.1f, false);
            JournalY += 24.0f;
            JournalY = DrawWrappedTextLine(GameMode->GetCorrectAnchorReadText().ToString(), FLinearColor(0.70f, 1.0f, 0.72f), JournalX, JournalY, JournalLineWidth, SmallFont, 0.95f);
            JournalY += 8.0f;
            JournalY = DrawWrappedTextLine(GameMode->GetWrongAnchorReadText().ToString(), FLinearColor(1.0f, 0.72f, 0.44f), JournalX, JournalY, JournalLineWidth, SmallFont, 0.95f);
            JournalY += 10.0f;
        }

        DrawText(TEXT("Next move"), FLinearColor(0.85f, 0.95f, 1.0f), JournalX, JournalY, SmallFont, 1.1f, false);
        JournalY += 24.0f;
        JournalY = DrawWrappedTextLine(GameMode->GetNextMoveText().ToString(), FLinearColor(0.95f, 0.88f, 0.60f), JournalX, JournalY, JournalLineWidth, SmallFont, 0.95f);
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
            const float PromptLineWidth = 600.0f;
            const float PromptScale = 1.1f;
            const float PromptHeight = MeasureWrappedTextHeight(PromptString, PromptLineWidth, SmallFont, PromptScale);
            const float PromptPanelHeight = FMath::Max(48.0f, PromptHeight + 20.0f);
            const float PromptPanelY = Canvas->ClipY - (PromptPanelHeight + 28.0f);
            const float PromptY = PromptPanelY + 10.0f;
            DrawPanel(24.0f, PromptPanelY, 640.0f, PromptPanelHeight, FLinearColor(0.02f, 0.03f, 0.05f, 0.72f));
            DrawWrappedTextLine(PromptString, FLinearColor::White, PromptX, PromptY, PromptLineWidth, SmallFont, PromptScale);
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
        const float EventLineWidth = 640.0f;
        const float EventTextHeight = MeasureWrappedTextHeight(RecentEvent.ToString(), EventLineWidth, SmallFont, 1.2f);
        const float EventPanelHeight = FMath::Max(92.0f, 40.0f + EventTextHeight + 22.0f);
        DrawPanel(EventX - 14.0f, EventY - 14.0f, 680.0f, EventPanelHeight, FLinearColor(0.02f, 0.03f, 0.05f, 0.68f));
        DrawText(TEXT("Finding"), FLinearColor(0.85f, 0.95f, 1.0f), EventX, EventY, SmallFont, 1.15f, false);
        EventY += 26.0f;
        DrawWrappedTextLine(RecentEvent.ToString(), FLinearColor(1.0f, 0.92f, 0.55f), EventX, EventY, EventLineWidth, SmallFont, 1.2f);
    }

    if (GameMode->GetCasePhase() == ETheLastRiteCasePhase::SealedAwaitingExit)
    {
        const float BannerX = Canvas->ClipX * 0.33f;
        const float BannerY = Canvas->ClipY * 0.16f;
        const FString BannerDetailText = TEXT("Leave through the front door and file the case.");
        const float BannerLineWidth = 460.0f;
        const float BannerDetailHeight = MeasureWrappedTextHeight(BannerDetailText, BannerLineWidth, SmallFont, 1.15f);
        const float BannerPanelHeight = FMath::Max(94.0f, 58.0f + BannerDetailHeight);
        DrawPanel(BannerX - 20.0f, BannerY - 18.0f, 520.0f, BannerPanelHeight, FLinearColor(0.03f, 0.08f, 0.04f, 0.80f));
        DrawText(TEXT("SEAL HOLDS"), FLinearColor(0.58f, 1.0f, 0.64f), BannerX, BannerY, LargeFont, 1.35f, false);
        DrawWrappedTextLine(BannerDetailText, FLinearColor::White, BannerX, BannerY + 40.0f, BannerLineWidth, SmallFont, 1.15f);
    }
    else if (GameMode->GetCasePhase() == ETheLastRiteCasePhase::RiteReady)
    {
        const float BannerX = Canvas->ClipX * 0.30f;
        const float BannerY = Canvas->ClipY * 0.16f;
        const FString BannerChoiceText = GameMode->GetCorrectAnchorReadText().ToString();
        const FString BannerWarningText = GameMode->GetWrongAnchorReadText().ToString();
        const float BannerLineWidth = 540.0f;
        const float BannerChoiceHeight = MeasureWrappedTextHeight(BannerChoiceText, BannerLineWidth, SmallFont, 1.08f);
        const float BannerWarningHeight = MeasureWrappedTextHeight(BannerWarningText, BannerLineWidth, SmallFont, 1.04f);
        const float BannerPanelHeight = FMath::Max(128.0f, 72.0f + BannerChoiceHeight + 10.0f + BannerWarningHeight);
        DrawPanel(BannerX - 20.0f, BannerY - 18.0f, 600.0f, BannerPanelHeight, FLinearColor(0.10f, 0.08f, 0.02f, 0.80f));
        DrawText(TEXT("RITE READY"), FLinearColor(1.0f, 0.88f, 0.44f), BannerX, BannerY, LargeFont, 1.35f, false);
        float BannerTextY = BannerY + 40.0f;
        BannerTextY = DrawWrappedTextLine(BannerChoiceText, FLinearColor(0.72f, 1.0f, 0.74f), BannerX, BannerTextY, BannerLineWidth, SmallFont, 1.08f);
        BannerTextY += 10.0f;
        DrawWrappedTextLine(BannerWarningText, FLinearColor(1.0f, 0.76f, 0.50f), BannerX, BannerTextY, BannerLineWidth, SmallFont, 1.04f);
    }
    else if (GameMode->GetCasePhase() == ETheLastRiteCasePhase::Investigating && GameMode->IsOpeningSweepComplete())
    {
        const float BannerX = Canvas->ClipX * 0.28f;
        const float BannerY = Canvas->ClipY * 0.16f;
        const FString BannerDetailText = GameMode->GetFocusZoneText().ToString();
        const float BannerLineWidth = 620.0f;
        const float BannerDetailHeight = MeasureWrappedTextHeight(BannerDetailText, BannerLineWidth, SmallFont, 1.08f);
        const float BannerPanelHeight = FMath::Max(96.0f, 58.0f + BannerDetailHeight);
        DrawPanel(BannerX - 20.0f, BannerY - 18.0f, 680.0f, BannerPanelHeight, FLinearColor(0.04f, 0.05f, 0.08f, 0.78f));
        DrawText(TEXT("FOLLOW THE WEIGHT"), FLinearColor(0.84f, 0.92f, 1.0f), BannerX, BannerY, LargeFont, 1.2f, false);
        DrawWrappedTextLine(BannerDetailText, FLinearColor::White, BannerX, BannerY + 40.0f, BannerLineWidth, SmallFont, 1.08f);
    }

    if (GameMode->IsCaseClosed())
    {
        const FString Ending = GameMode->GetEndingText().ToString();
        const FString EndingDetail = GameMode->GetEndingDetailText().ToString();
        const float CenterX = Canvas->ClipX * 0.18f;
        const float CenterY = Canvas->ClipY * 0.28f;
        const float EndingDetailY = CenterY + 70.0f;
        const float ReportLineWidth = 670.0f;
        const float EndingDetailHeight = MeasureWrappedTextHeight(EndingDetail, ReportLineWidth, SmallFont, 1.2f);
        float ReportY = EndingDetailY + EndingDetailHeight + 18.0f;
        const TArray<FString>& FinalReportLines = GameMode->GetFinalReportLines();
        float OverlayHeight = 220.0f;
        if (!FinalReportLines.IsEmpty())
        {
            float ReportBodyHeight = 26.0f;
            for (const FString& Line : FinalReportLines)
            {
                ReportBodyHeight += Line.IsEmpty()
                    ? 8.0f
                    : (MeasureWrappedTextHeight(Line, ReportLineWidth, SmallFont, 1.0f) + 2.0f);
            }

            OverlayHeight = FMath::Clamp(
                (ReportY - CenterY) + ReportBodyHeight + 72.0f,
                320.0f,
                Canvas->ClipY - (CenterY - 20.0f) - 24.0f);
        }
        DrawPanel(CenterX - 22.0f, CenterY - 20.0f, 760.0f, OverlayHeight, FLinearColor(0.01f, 0.02f, 0.04f, 0.84f));
        DrawText(Ending, GameMode->DidPlayerWin() ? FLinearColor(0.5f, 1.0f, 0.5f) : FLinearColor(1.0f, 0.45f, 0.45f), CenterX, CenterY, LargeFont, 2.0f, false);
        DrawWrappedTextLine(EndingDetail, FLinearColor::White, CenterX, EndingDetailY, ReportLineWidth, SmallFont, 1.2f);

        if (!FinalReportLines.IsEmpty())
        {
            DrawPanel(CenterX - 10.0f, ReportY - 10.0f, 710.0f, OverlayHeight - 176.0f, FLinearColor(0.04f, 0.05f, 0.08f, 0.62f));
            DrawText(TEXT("Final report"), FLinearColor(0.85f, 0.95f, 1.0f), CenterX, ReportY, SmallFont, 1.15f, false);
            ReportY += 26.0f;

            for (const FString& Line : FinalReportLines)
            {
                if (Line.IsEmpty())
                {
                    ReportY += 8.0f;
                    continue;
                }
                const FLinearColor ReportColor = GetFinalReportLineColor(Line);

                ReportY = DrawWrappedTextLine(Line, ReportColor, CenterX, ReportY, ReportLineWidth, SmallFont, 1.0f);
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

float ATheLastRiteHUD::DrawWrappedTextLine(const FString& Text, const FLinearColor& Color, float X, float Y, float MaxLineWidth, UFont* Font, float Scale)
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

float ATheLastRiteHUD::MeasureWrappedTextHeight(const FString& Text, float MaxLineWidth, UFont* Font, float Scale)
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

FLinearColor ATheLastRiteHUD::GetFinalReportLineColor(const FString& Line) const
{
    const auto ContainsIgnoreCase = [&Line](const TCHAR* Needle)
    {
        return Line.Contains(Needle, ESearchCase::IgnoreCase);
    };

    if (ContainsIgnoreCase(TEXT("missed")) || ContainsIgnoreCase(TEXT("unchecked")))
    {
        return FLinearColor(0.95f, 0.88f, 0.60f);
    }

    if (ContainsIgnoreCase(TEXT("Conclusion"))
        || ContainsIgnoreCase(TEXT("correct anchor"))
        || ContainsIgnoreCase(TEXT("Why the nursery was correct"))
        || ContainsIgnoreCase(TEXT("Seal result"))
        || ContainsIgnoreCase(TEXT("True clue")))
    {
        return FLinearColor(0.68f, 1.0f, 0.72f);
    }

    if (ContainsIgnoreCase(TEXT("Correct read")))
    {
        return FLinearColor(0.76f, 0.88f, 1.0f);
    }

    if (ContainsIgnoreCase(TEXT("Wrong rite")) || ContainsIgnoreCase(TEXT("Outcome")) || ContainsIgnoreCase(TEXT("What went wrong")) || ContainsIgnoreCase(TEXT("Why the mirror was wrong")))
    {
        return FLinearColor(1.0f, 0.45f, 0.45f);
    }

    if (ContainsIgnoreCase(TEXT("Recovery")) || ContainsIgnoreCase(TEXT("audit")))
    {
        return FLinearColor(0.95f, 0.88f, 0.60f);
    }

    if (ContainsIgnoreCase(TEXT("False lead")) || ContainsIgnoreCase(TEXT("Discarded false leads")))
    {
        return FLinearColor(1.0f, 0.72f, 0.44f);
    }

    if (ContainsIgnoreCase(TEXT("Confirmed true clues")) || ContainsIgnoreCase(TEXT("Case title")) || ContainsIgnoreCase(TEXT("Demon")))
    {
        return FLinearColor(0.86f, 0.92f, 1.0f);
    }

    return FLinearColor(0.82f, 0.86f, 0.90f);
}
