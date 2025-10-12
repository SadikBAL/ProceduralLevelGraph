#include "SRoomGraphNode.h"
#include "ProceduralLevelGraphEditor/Room/RoomGraphNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "SGraphPin.h"
#include "SRoomGraphNodePin.h"

#define LOCTEXT_NAMESPACE "SRoomGraphNode"

void SRoomGraphNode::Construct(const FArguments& InArgs, URoomGraphNode* InNode)
{
    this->GraphNode = InNode;

	UpdateGraphNode();
	
	GetOrAddSlot(ENodeZone::Center)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SNew(SOverlay)

		// Katman 1: Pinler (Kapılar)
		+ SOverlay::Slot()
		.HAlign(HAlign_Center).VAlign(VAlign_Top).Padding(0, -30, 0, 0)
		[ UpPin.IsValid() ? UpPin.ToSharedRef() : SNullWidget::NullWidget ]

		+ SOverlay::Slot()
		.HAlign(HAlign_Center).VAlign(VAlign_Bottom).Padding(0, 0, 0, -30)
		[ DownPin.IsValid() ? DownPin.ToSharedRef() : SNullWidget::NullWidget ]

		+ SOverlay::Slot()
		.HAlign(HAlign_Left).VAlign(VAlign_Center).Padding(-36, 0, 0, 0)
		[ LeftPin.IsValid() ? LeftPin.ToSharedRef() : SNullWidget::NullWidget ]

		+ SOverlay::Slot()
		.HAlign(HAlign_Right).VAlign(VAlign_Center).Padding(0, 0, -30, 0)
		[ RightPin.IsValid() ? RightPin.ToSharedRef() : SNullWidget::NullWidget ]

		+ SOverlay::Slot()
	   [
		  // SBox: Boyutu zorla ayarlamak için SBorder'ı bir SBox ile sarmalıyoruz.
		  SNew(SBox)
		  .WidthOverride(250.f)  // Genişliği 100 birim olarak ayarla
		  .HeightOverride(250.f)
	   	// Yüksekliği 100 birim olarak ayarla (tam kare)
		  [
			 SNew(SBorder)
			 // "NoBorder" fırçası, renklendirilebilen düz bir karedir. Yuvarlak köşeleri yoktur.
			 .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
			 // BorderBackgroundColor, Fırçanın arkasındaki rengi belirler.
			 // NOT: FLinearColor değerleri 0.0 ile 1.0 arasında olmalıdır.
			 .BorderBackgroundColor(FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f)))
			 .HAlign(HAlign_Center) // İçeriği (metni) yatayda ortala
			 .VAlign(VAlign_Center) // İçeriği (metni) dikeyde ortala
			 [
				SNew(STextBlock)
				.Text(FText::FromString("ROOM"))
			 ]
		  ]
	   ]
	];
}

void SRoomGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	const FName PinName = PinToAdd->GetPinObj()->GetFName();

	if (PinName == FName("Up"))
	{
		UpPin = PinToAdd;
	}
	else if (PinName == FName("Down"))
	{
		DownPin = PinToAdd;
	}
	else if (PinName == FName("Left"))
	{
		LeftPin = PinToAdd;
	}
	else if (PinName == FName("Right"))
	{
		RightPin = PinToAdd;
	}
	PinToAdd->SetOwner(SharedThis(this));
}

TSharedPtr<SGraphPin> SRoomGraphNode::CreatePinWidget(UEdGraphPin* Pin) const
{
	TSharedPtr<SGraphPin> PinWidget = SNew(SRoomGraphNodePin, Pin);
	PinWidget->SetShowLabel(false);
	return PinWidget;
}
#undef LOCTEXT_NAMESPACE
