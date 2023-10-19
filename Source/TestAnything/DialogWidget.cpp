#include "DialogWidget.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"

void SDialogWidget::Construct(const FArguments& InArgs)
{
    MessageText = InArgs._MessageText;
    OnConfirmClicked = InArgs._OnConfirmClicked;
    OnCancelClicked = InArgs._OnCancelClicked;
    DialogWindow = InArgs._DialogWindow;

    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(20)
        [
            SNew(STextBlock)
            .Text(MessageText)
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(20)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SAssignNew(ConfirmButton, SButton)
                .Text(FText::FromString("Confirm"))
                .IsFocusable(true)
                .OnClicked(this, &SDialogWidget::OnConfirmButtonClicked)
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SSpacer)
                .Size(FVector2D(20, 1))
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SButton)
                .Text(FText::FromString("Cancel"))
                .IsFocusable(true)
                .OnClicked_Lambda([this]() {
                    OnCancelClicked.ExecuteIfBound();
                    return FReply::Handled();
                })
            ]
        ]
    ];

    //FSlateApplication::Get().SetKeyboardFocus(ConfirmButton);
}

FReply SDialogWidget::OnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    // TODO: didn't be invoked... why?
    if (InKeyEvent.GetKey() == EKeys::Enter)
    {
        OnConfirmButtonClicked();
        return FReply::Handled();
    }

    return FReply::Unhandled();
}

TSharedPtr<SDialogWidget> DialogWidget;
TSharedPtr<SWindow> ModalWindow; // The host window
void SDialogWidget::ShowModal(FText Msg, FSimpleDelegate OnConfirmed)
{
    ModalWindow = SNew(SWindow)
        .Title(FText::FromString("Dialog"))
        .SizingRule(ESizingRule::Autosized);

    DialogWidget = SNew(SDialogWidget)
        .DialogWindow(ModalWindow)
        .MessageText(Msg)
        .OnConfirmClicked(OnConfirmed)
        .OnCancelClicked(FSimpleDelegate::CreateLambda([]() {
            ModalWindow->RequestDestroyWindow();
            ModalWindow.Reset();
            DialogWidget.Reset();
        }));

    ModalWindow->SetContent(DialogWidget.ToSharedRef());
    //FSlateApplication::Get().SetKeyboardFocus(ModalWindow, EKeyboardFocusCause::SetDirectly);
    FSlateApplication::Get().SetUnhandledKeyDownEventHandler(FOnKeyEvent::CreateLambda([](const FKeyEvent& InKeyEvent) -> FReply {
        if (InKeyEvent.GetKey() == EKeys::Escape)
        {
            ModalWindow->RequestDestroyWindow();
            return FReply::Handled();
        } 
        else if (InKeyEvent.GetKey() == EKeys::Enter)
        {
            DialogWidget->OnConfirmButtonClicked();
            return FReply::Handled();
        }
        return FReply::Unhandled();
        }));
    FSlateApplication::Get().AddModalWindow(ModalWindow.ToSharedRef(), nullptr);
}

FReply SDialogWidget::OnConfirmButtonClicked()
{
    if (ModalWindow.IsValid() && ModalWindow->IsVisible()) {
        ModalWindow->RequestDestroyWindow();
        ModalWindow.Reset();
        DialogWidget.Reset();
    }

    OnConfirmClicked.ExecuteIfBound();

    return FReply::Handled();
}