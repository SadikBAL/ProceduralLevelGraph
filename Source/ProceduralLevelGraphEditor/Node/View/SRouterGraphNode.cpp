#include "SRouterGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data//RouterGraphNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"

#include "SGraphPin.h"
#include "SRoomGraphNodePin.h"

#define LOCTEXT_NAMESPACE "SRouterGraphNode"

void SRouterGraphNode::Construct(const FArguments& InArgs, URouterGraphNode* InNode)
{
    this->GraphNode = InNode;
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle("NormalFont");
	TitleFont.Size = 10;
	UpdateGraphNode();
	UpdatePinTypes();
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
				.Text(FText::FromString("ROUTER"))
				 .Font(TitleFont)
			 ]
		  ]
	   ]
		
		+ SOverlay::Slot()
		.HAlign(HAlign_Center).VAlign(VAlign_Top).Padding(0,PinPadding,0,0)
		[
			(UpPin.IsValid() && UpPin->PinType != EMazePinType::Hidden)
			? UpPin.ToSharedRef()
			: SNullWidget::NullWidget
		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Center).VAlign(VAlign_Bottom).Padding(0,0,0,PinPadding)
		[
			(DownPin.IsValid() && DownPin->PinType != EMazePinType::Hidden)
			? DownPin.ToSharedRef()
			: SNullWidget::NullWidget
		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Left).VAlign(VAlign_Center).Padding(PinPadding-4,0,0,0)
		[
			(LeftPin.IsValid() && LeftPin->PinType != EMazePinType::Hidden)
			? LeftPin.ToSharedRef()
			: SNullWidget::NullWidget
		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Right).VAlign(VAlign_Center).Padding(0,0,PinPadding,0)
		[
			(RightPin.IsValid() && RightPin->PinType != EMazePinType::Hidden)
			? RightPin.ToSharedRef()
			: SNullWidget::NullWidget
		]
	];
}

void SRouterGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	const FName PinName = PinToAdd->GetPinObj()->GetFName();
	const TSharedPtr<SGraphPin> BasePinPtr = PinToAdd;
	if (PinName == FName("Up"))
	{
		UpPin = StaticCastSharedPtr<SRoomGraphNodePin>(BasePinPtr);
		UpPin->PinDirection = EMazeOrientation::Vertical;
	}
	else if (PinName == FName("Down"))
	{
		DownPin = StaticCastSharedPtr<SRoomGraphNodePin>(BasePinPtr);
		DownPin->PinDirection = EMazeOrientation::Vertical;
	}
	else if (PinName == FName("Left"))
	{
		LeftPin = StaticCastSharedPtr<SRoomGraphNodePin>(BasePinPtr);
		LeftPin->PinDirection = EMazeOrientation::Horizontal;
	}
	else if (PinName == FName("Right"))
	{
		RightPin = StaticCastSharedPtr<SRoomGraphNodePin>(BasePinPtr);
		RightPin->PinDirection = EMazeOrientation::Horizontal;
	}
	SGraphNode::AddPin(PinToAdd);
}

TSharedPtr<SGraphPin> SRouterGraphNode::CreatePinWidget(UEdGraphPin* Pin) const
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

void SRouterGraphNode::GetAllPinWidgets(TArray<TSharedPtr<SGraphPin>>& OutPinWidgets) const
{
	if(UpPin.IsValid()) OutPinWidgets.Add(UpPin);
	if(DownPin.IsValid()) OutPinWidgets.Add(DownPin);
	if(LeftPin.IsValid()) OutPinWidgets.Add(LeftPin);
	if(RightPin.IsValid()) OutPinWidgets.Add(RightPin);
	SMazeGraphNodeBase::GetAllPinWidgets(OutPinWidgets);
}

FOptionalSize SRouterGraphNode::GetNodeHeight() const
{
	if (const URouterGraphNode* NodeRef = Cast<URouterGraphNode>(GraphNode))
	{
		if (NodeRef->RoomRotation == 0 || NodeRef->RoomRotation == 180)
		{
			return NodeRef->RoomHeight * TILE_EDITOR_SCALE;
		}
		else if (NodeRef->RoomRotation == 90 || NodeRef->RoomRotation == 270)
		{
			return NodeRef->RoomWidth * TILE_EDITOR_SCALE;
		}
	}
	return 5.0f;
}

FOptionalSize SRouterGraphNode::GetNodeWidth() const
{
	if (const URouterGraphNode* NodeRef = Cast<URouterGraphNode>(GraphNode))
	{
		if (NodeRef->RoomRotation == 0 || NodeRef->RoomRotation == 180)
		{
			return NodeRef->RoomWidth * TILE_EDITOR_SCALE;
		}
		else if (NodeRef->RoomRotation == 90 || NodeRef->RoomRotation == 270)
		{
			return NodeRef->RoomHeight * TILE_EDITOR_SCALE;
		}
	}
	return 5.0f;
}
#undef LOCTEXT_NAMESPACE
