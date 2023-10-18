#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SDialogWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SDialogWidget)
        : _DialogWindow(nullptr)
    {}
    SLATE_ARGUMENT(TSharedPtr<SWindow>, DialogWindow)
    SLATE_ARGUMENT(FText, MessageText)
    SLATE_EVENT(FSimpleDelegate, OnConfirmClicked)
    SLATE_EVENT(FSimpleDelegate, OnCancelClicked)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    static void ShowModal(FText Msg, FSimpleDelegate OnConfirmed);

private:
    TSharedPtr<SWindow> DialogWindow;
    TSharedPtr<SButton> ConfirmButton;
    FText MessageText;
    FSimpleDelegate OnConfirmClicked;
    FSimpleDelegate OnCancelClicked;

    FReply OnConfirmButtonClicked();
};