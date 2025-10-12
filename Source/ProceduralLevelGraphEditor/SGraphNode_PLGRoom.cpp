// SGraphNode_PLGRoom.cpp
#include "SGraphNode_PLGRoom.h"
#include "EdGraphNode_PLGRoom.h"
#include "SGraphPin.h"
#include "KismetPins/SGraphPinExec.h"
#include "Widgets/SBoxPanel.h"
#include "GraphEditorSettings.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "SGraphNode_PLGRoom"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGraphNode_PLGRoom::Construct(const FArguments& InArgs, UEdGraphNode_PLGRoom* InNode)
{
    GraphNode = InNode;

    // Önce widget'ların iskeletini OLUŞTURUYORUZ.
    // Bu sayede TSharedPtr'lar asla null olmaz.
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
                + SVerticalBox::Slot().AutoHeight() [ SAssignNew(TopPinBox, SHorizontalBox) ]
                
                // Orta Sıra
                + SVerticalBox::Slot().AutoHeight()
                [
                    SNew(SHorizontalBox)
                    + SHorizontalBox::Slot().AutoWidth() [ SAssignNew(LeftPinBox, SVerticalBox) ]
                    + SHorizontalBox::Slot().AutoWidth().Padding(10.0f) [ SAssignNew(NodeTitle, SNodeTitle, GraphNode) ]
                    + SHorizontalBox::Slot().AutoWidth() [ SAssignNew(RightPinBox, SVerticalBox) ]
                ]

                // Alt Pinler için Konteyner
                + SVerticalBox::Slot().AutoHeight() [ SAssignNew(BottomPinBox, SHorizontalBox) ]
            ]
        ]
    ];
    
    // Ana sınıfın Construct'ını çağırıyoruz. Bu, dolaylı olarak
    // aşağıda override ettiğimiz CreatePinWidgets() fonksiyonunu tetikleyecek.
    // Bu çağrı yapıldığında, yukarıdaki SAssignNew sayesinde ...PinBox işaretçileri
    // artık geçerli (valid) olduğu için çökme yaşanmayacak.
    SGraphNode::Construct();
}

void SGraphNode_PLGRoom::CreatePinWidgets()
{
    // UEdGraphNode'dan pin verilerini al
    UEdGraphNode_PLGRoom* RoomNode = CastChecked<UEdGraphNode_PLGRoom>(GraphNode);

    for (UEdGraphPin* Pin : RoomNode->Pins)
    {
        if (Pin && !Pin->bHidden)
        {
            TSharedPtr<SGraphPin> PinWidget = SNew(SGraphPinExec, Pin);
            
            // Pin widget'ını ana sınıfın bilmesi için ilgili dizilere ekle
            AddPin(PinWidget.ToSharedRef());
            
            // Pin'i ismine göre doğru kutuya yerleştir
            const FName PinName = Pin->GetFName();
            if (PinName == FName("North"))
            {
                TopPinBox->AddSlot().HAlign(HAlign_Center).AutoWidth().Padding(5)[PinWidget.ToSharedRef()];
            }
            else if (PinName == FName("South"))
            {
                BottomPinBox->AddSlot().HAlign(HAlign_Center).AutoWidth().Padding(5)[PinWidget.ToSharedRef()];
            }
            else if (PinName == FName("West"))
            {
                LeftPinBox->AddSlot().VAlign(VAlign_Center).AutoHeight().Padding(5)[PinWidget.ToSharedRef()];
            }
            else if (PinName == FName("East"))
            {
                RightPinBox->AddSlot().VAlign(VAlign_Center).AutoHeight().Padding(5)[PinWidget.ToSharedRef()];
            }
        }
    }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE