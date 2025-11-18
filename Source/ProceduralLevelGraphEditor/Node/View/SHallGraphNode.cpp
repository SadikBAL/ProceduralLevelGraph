#include "SHallGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/HallGraphNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"

#include "SGraphPin.h"
#include "SRoomGraphNodePin.h"

#define LOCTEXT_NAMESPACE "SRoomGraphNode"

void SHallGraphNode::Construct(const FArguments& InArgs, UHallGraphNode* InNode)
{
    this->GraphNode = InNode;
	this->HallGraphNodeRef = InNode;
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle("NormalFont");
	TitleFont.Size = 16;
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
					.Text(FText::FromString("HALL"))
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

void SHallGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	const FName PinName = PinToAdd->GetPinObj()->GetFName();
	TSharedPtr<SGraphPin> BasePinPtr = PinToAdd;
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

TSharedPtr<SGraphPin> SHallGraphNode::CreatePinWidget(UEdGraphPin* Pin) const
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

void SHallGraphNode::GetAllPinWidgets(TArray<TSharedPtr<SGraphPin>>& OutPinWidgets) const
{
	if(UpPin.IsValid()) OutPinWidgets.Add(UpPin);
	if(DownPin.IsValid()) OutPinWidgets.Add(DownPin);
	if(LeftPin.IsValid()) OutPinWidgets.Add(LeftPin);
	if(RightPin.IsValid()) OutPinWidgets.Add(RightPin);
	SMazeGraphNodeBase::GetAllPinWidgets(OutPinWidgets);
}

FOptionalSize SHallGraphNode::GetNodeHeight() const
{
	if (UHallGraphNode* HallNode = Cast<UHallGraphNode>(GraphNode))
	{
		if (HallNode->RoomRotation == 0 || HallNode->RoomRotation == 180)
		{
			return HallNode->HallLength * TILE_EDITOR_SCALE;
		}
		else
		{
			return TILE_EDITOR_SCALE;
		}
	}
	return 0.0f;
}

FOptionalSize SHallGraphNode::GetNodeWidth() const
{
	if (UHallGraphNode* HallNode = Cast<UHallGraphNode>(GraphNode))
	{
		if (HallNode->RoomRotation == 0 || HallNode->RoomRotation == 180)
		{
			return TILE_EDITOR_SCALE;
		}
		else
		{
			return HallNode->HallLength * TILE_EDITOR_SCALE;
		}
	}
	return 0.0f;
}

void SHallGraphNode::UpdatePinTypes()
{
	if (UpPin.IsValid() && (HallGraphNodeRef->RoomRotation == 0 || HallGraphNodeRef->RoomRotation == 180))
	{
		UpPin->PinType = EMazePinType::Tier1;
	}
	else if(UpPin.IsValid())
	{
		UpPin->PinType = EMazePinType::Hidden;
	}
	
	if (DownPin.IsValid() && (HallGraphNodeRef->RoomRotation == 0 || HallGraphNodeRef->RoomRotation == 180))
	{
		DownPin->PinType = EMazePinType::Tier1;
	}
	else if(DownPin.IsValid())
	{
		DownPin->PinType = EMazePinType::Hidden;
	}
	if (LeftPin.IsValid() && (HallGraphNodeRef->RoomRotation == 90 || HallGraphNodeRef->RoomRotation == 270))
	{
		LeftPin->PinType = EMazePinType::Tier1;
	}
	else if(LeftPin.IsValid())
	{
		LeftPin->PinType = EMazePinType::Hidden;
	}
	if (RightPin.IsValid() && (HallGraphNodeRef->RoomRotation == 90 || HallGraphNodeRef->RoomRotation == 270))
	{
		RightPin->PinType = EMazePinType::Tier1;
	}
	else if(RightPin.IsValid())
	{
		RightPin->PinType = EMazePinType::Hidden;
	}
}

#undef LOCTEXT_NAMESPACE
