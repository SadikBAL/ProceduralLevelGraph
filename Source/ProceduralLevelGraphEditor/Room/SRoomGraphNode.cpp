#include "SRoomGraphNode.h"

#include "HallGraphNode.h"
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
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle("NormalFont");
	TitleFont.Size = 24;
	UpdateGraphNode();
	
	GetOrAddSlot(ENodeZone::Center)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
	   [
		  SNew(SBox)
		  .WidthOverride(GetNodeWidth())
		  .HeightOverride(GetNodeHeight())
		  [
			 SNew(SBorder)
			 .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
			 .BorderBackgroundColor(FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f)))
			 .HAlign(HAlign_Center)
			   .VAlign(VAlign_Center)
			 [
				SNew(STextBlock)
				.Text(FText::FromString("ROOM"))
				 .Font(TitleFont)
			 ]
		  ]
	   ]
		
		+ SOverlay::Slot()
		.HAlign(HAlign_Center).VAlign(VAlign_Top).Padding(0,PinPadding,0,0)
		[ UpPin.IsValid() ? UpPin.ToSharedRef() : SNullWidget::NullWidget ]

		+ SOverlay::Slot()
		.HAlign(HAlign_Center).VAlign(VAlign_Bottom).Padding(0,0,0,PinPadding)
		[ DownPin.IsValid() ? DownPin.ToSharedRef() : SNullWidget::NullWidget ]

		+ SOverlay::Slot()
		.HAlign(HAlign_Left).VAlign(VAlign_Center).Padding(PinPadding-4,0,0,0)
		[ LeftPin.IsValid() ? LeftPin.ToSharedRef() : SNullWidget::NullWidget ]

		+ SOverlay::Slot()
		.HAlign(HAlign_Right).VAlign(VAlign_Center).Padding(0,0,PinPadding,0)
		[ RightPin.IsValid() ? RightPin.ToSharedRef() : SNullWidget::NullWidget ]
	];
}

void SRoomGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	const FName PinName = PinToAdd->GetPinObj()->GetFName();
	TSharedPtr<SGraphPin> BasePinPtr = PinToAdd;
	if (PinName == FName("Up"))
	{
		UpPin = StaticCastSharedPtr<SRoomGraphNodePin>(BasePinPtr);
		UpPin->PinLocation = EMazeDirection::Up;
	}
	else if (PinName == FName("Down"))
	{
		DownPin = StaticCastSharedPtr<SRoomGraphNodePin>(BasePinPtr);
		DownPin->PinLocation = EMazeDirection::Down;
	}
	else if (PinName == FName("Left"))
	{
		LeftPin = StaticCastSharedPtr<SRoomGraphNodePin>(BasePinPtr);
		LeftPin->PinLocation = EMazeDirection::Left;
	}
	else if (PinName == FName("Right"))
	{
		RightPin = StaticCastSharedPtr<SRoomGraphNodePin>(BasePinPtr);
		RightPin->PinLocation = EMazeDirection::Right;
	}
	
	SGraphNode::AddPin(PinToAdd);
}

TSharedPtr<SGraphPin> SRoomGraphNode::CreatePinWidget(UEdGraphPin* Pin) const
{
	if (Pin->Direction == EGPD_Input)
	{
		return SNew(SGraphPin, Pin);
	}
	else
	{
		TSharedPtr<SGraphPin> PinWidget = SNew(SRoomGraphNodePin, Pin);
		PinWidget->SetShowLabel(false);
		return PinWidget;
	}

}

void SRoomGraphNode::GetAllPinWidgets(TArray<TSharedPtr<SGraphPin>>& OutPinWidgets) const
{
	if(UpPin.IsValid()) OutPinWidgets.Add(UpPin);
	if(DownPin.IsValid()) OutPinWidgets.Add(DownPin);
	if(LeftPin.IsValid()) OutPinWidgets.Add(LeftPin);
	if(RightPin.IsValid()) OutPinWidgets.Add(RightPin);
	SMazeGraphNodeBase::GetAllPinWidgets(OutPinWidgets);
}

FOptionalSize SRoomGraphNode::GetNodeHeight() const
{
	if (URoomGraphNode* RoomNode = Cast<URoomGraphNode>(GraphNode))
	{
		return FMath::Max(20.0f, RoomNode->RoomHeight * 100); 
	}
	return 300.0f;
}

FOptionalSize SRoomGraphNode::GetNodeWidth() const
{
	if (URoomGraphNode* RoomNode = Cast<URoomGraphNode>(GraphNode))
	{
		return FMath::Max(20.0f, RoomNode->RoomWith * 100); 
	}
	return 300.0f;
}
#undef LOCTEXT_NAMESPACE
