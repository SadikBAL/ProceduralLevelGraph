#include "SGraphNode_PLGRoom.h"
#include "EdGraphNode_PLGRoom.h"
#include "SGraphPin.h"
#include "KismetPins/SGraphPinExec.h" // Exec pinleri için özel stil
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "SCommentBubble.h"
#include "GraphEditorSettings.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "SGraphNode_PLGRoom"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGraphNode_PLGRoom::Construct(const FArguments& InArgs, UEdGraphNode_PLGRoom* InNode)
{
    GraphNode = InNode;

    // Widget'ın görsel iskeletini SADECE BİR KEZ burada kuruyoruz.
    GetOrAddSlot(ENodeZone::Center)
    .HAlign(HAlign_Center)
    .VAlign(VAlign_Center)
    [
        SNew(SBorder)
        .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
        .Padding(0)
        [
            SNew(SOverlay)
            + SOverlay::Slot()
            [
                SNew(SVerticalBox)
                // Üst Pinler için Konteyner
                + SVerticalBox::Slot().AutoHeight()
                [
                    SAssignNew(TopPinBox, SHorizontalBox)
                ]
                // Orta Sıra
                + SVerticalBox::Slot().AutoHeight()
                [
                    SNew(SHorizontalBox)
                    + SHorizontalBox::Slot().AutoWidth() [ SAssignNew(LeftPinBox, SVerticalBox) ]
                    + SHorizontalBox::Slot().AutoWidth().Padding(10.0f) [ SAssignNew(NodeTitle, SNodeTitle, GraphNode) ]
                    + SHorizontalBox::Slot().AutoWidth() [ SAssignNew(RightPinBox, SVerticalBox) ]
                ]
                // Alt Pinler için Konteyner
                + SVerticalBox::Slot().AutoHeight()
                [
                    SAssignNew(BottomPinBox, SHorizontalBox)
                ]
            ]
        ]
    ];
}

void SGraphNode_PLGRoom::UpdateGraphNode()
{
    // Önceki pinleri temizle
    InputPins.Empty();
    OutputPins.Empty();
    
    // Var olan konteynerlerin içini boşalt. YENİDEN OLUŞTURMA!
    TopPinBox->ClearChildren();
    BottomPinBox->ClearChildren();
    LeftPinBox->ClearChildren();
    RightPinBox->ClearChildren();

    // Pinleri oluştur ve VAR OLAN kutuların içine yerleştir
    for (UEdGraphPin* Pin : GraphNode->Pins)
    {
        if (Pin && !Pin->bHidden)
        {
            TSharedPtr<SGraphPin> PinWidget = SNew(SGraphPinExec, Pin);
            
            const FName PinName = Pin->GetFName();
            if (PinName == FName("North")) TopPinBox->AddSlot().HAlign(HAlign_Center).AutoWidth().Padding(5)[PinWidget.ToSharedRef()];
            else if (PinName == FName("South")) BottomPinBox->AddSlot().HAlign(HAlign_Center).AutoWidth().Padding(5)[PinWidget.ToSharedRef()];
            else if (PinName == FName("West")) LeftPinBox->AddSlot().VAlign(VAlign_Center).AutoHeight().Padding(5)[PinWidget.ToSharedRef()];
            else if (PinName == FName("East")) RightPinBox->AddSlot().VAlign(VAlign_Center).AutoHeight().Padding(5)[PinWidget.ToSharedRef()];
            
        }
    }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE