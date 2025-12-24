#include "SRoomGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/RoomGraphNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"

#include "SGraphPin.h"
#include "SRoomGraphNodePin.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

#define LOCTEXT_NAMESPACE "SRoomGraphNode"

void SRoomGraphNode::Construct(const FArguments& InArgs, URoomGraphNode* InNode)
{
	this->GraphNode = InNode;
	this->RoomGraphNodeRef = Cast<URoomGraphNode>(GraphNode);
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle("NormalFont");
	TitleFont.Size = 24;
	UpdateGraphNode();
	InNode->AllocateDefaultPins();
	TSharedPtr<SOverlay> PinOverlay;
	SAssignNew(PinOverlay, SOverlay)
	+ SOverlay::Slot()
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
	    SNew(SBox)
	    .WidthOverride(this, &SRoomGraphNode::GetNodeWidth)
	    .HeightOverride(this, &SRoomGraphNode::GetNodeHeight)
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
	];
	
	for (int32 i = 0; i < RoomPins.Num(); ++i)
	{
	    TSharedPtr<SRoomGraphNodePin> CurrentPinWidget = RoomPins[i];
	    if (CurrentPinWidget.IsValid())
	    {
	        UEdGraphPin* PinObj = CurrentPinWidget->GetPinObj();
	        EHorizontalAlignment HAlign = HAlign_Center;
	        EVerticalAlignment VAlign = VAlign_Center;
	        FMargin Padding(0);
	    	int32 LocalOffset = 0;
	    	int32 LocalMult = 1;
	    	if (RoomGraphNodeRef->GetRotatedPinDirection(CurrentPinWidget->PinLocation) == EMazeDirection::Up) {
	    		//Up
	            VAlign = VAlign_Top;
	    		LocalOffset = (CurrentPinWidget->PinLocation == EMazeDirection::Left || CurrentPinWidget->PinLocation == EMazeDirection::Right) 
	    		? CurrentPinWidget->PinOffset.Y : CurrentPinWidget->PinOffset.X;
				if (CurrentPinWidget->PinLocation == EMazeDirection::Down 
					|| CurrentPinWidget->PinLocation == EMazeDirection::Left)
				{
					LocalMult = -1;
				}
	            Padding = FMargin(LocalOffset * LocalMult * TILE_EDITOR_SCALE, PinPadding, 0, 0);
	        }
	    	else if (RoomGraphNodeRef->GetRotatedPinDirection(CurrentPinWidget->PinLocation) == EMazeDirection::Right) {
	    		//Right
	    		HAlign = HAlign_Right;
	    		LocalOffset = (CurrentPinWidget->PinLocation == EMazeDirection::Left || CurrentPinWidget->PinLocation == EMazeDirection::Right) 
				? CurrentPinWidget->PinOffset.Y : CurrentPinWidget->PinOffset.X;
	    		if (CurrentPinWidget->PinLocation == EMazeDirection::Down 
					||CurrentPinWidget->PinLocation == EMazeDirection::Left)
	    		{
	    			LocalMult = -1;
	    		}
	    		Padding = FMargin(0, LocalOffset * LocalMult  * TILE_EDITOR_SCALE, PinPadding, 0);
	    	}
	        else if (RoomGraphNodeRef->GetRotatedPinDirection(CurrentPinWidget->PinLocation) == EMazeDirection::Down) {
	            //Left
	        	VAlign = VAlign_Bottom;
	        	LocalOffset = (CurrentPinWidget->PinLocation == EMazeDirection::Left || CurrentPinWidget->PinLocation == EMazeDirection::Right) 
				? CurrentPinWidget->PinOffset.Y : CurrentPinWidget->PinOffset.X;
	        	if (CurrentPinWidget->PinLocation == EMazeDirection::Right 
					|| CurrentPinWidget->PinLocation == EMazeDirection::Up)
	        	{
	        		LocalMult = -1;
	        	}
	        	Padding = FMargin(LocalOffset * LocalMult  * TILE_EDITOR_SCALE, 0, 2, PinPadding);
	        }
	        else if (RoomGraphNodeRef->GetRotatedPinDirection(CurrentPinWidget->PinLocation) == EMazeDirection::Left) {
	            //Down
	        	HAlign = HAlign_Left;
	        	LocalOffset = (CurrentPinWidget->PinLocation == EMazeDirection::Left || CurrentPinWidget->PinLocation == EMazeDirection::Right) 
				? CurrentPinWidget->PinOffset.Y : CurrentPinWidget->PinOffset.X;
	        	if (CurrentPinWidget->PinLocation == EMazeDirection::Right
	        		|| CurrentPinWidget->PinLocation == EMazeDirection::Up)
	        	{
	        		LocalMult = -1;
	        	}
	        	Padding = FMargin(PinPadding-4, LocalOffset * LocalMult  * TILE_EDITOR_SCALE,0 , 0);
	        }

	        PinOverlay->AddSlot()
	        .HAlign(HAlign)
	        .VAlign(VAlign)
	        .Padding(Padding)
	        [
	            CurrentPinWidget.ToSharedRef()
	        ];
	    }
	}

	this->GetOrAddSlot(ENodeZone::Center)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
	    PinOverlay.ToSharedRef()
	];
}

void SRoomGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	const FName PinName = PinToAdd->GetPinObj()->GetFName();
	UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const TSharedPtr<SGraphPin> BasePinPtr = PinToAdd;
	if (RoomGraphNodeRef && RoomGraphNodeRef->DoorDatas.Num() > 0)
	{
		TSharedPtr<SRoomGraphNodePin> TempPin = StaticCastSharedPtr<SRoomGraphNodePin>(BasePinPtr);
		
		int32 Index = RoomGraphNodeRef->Pins.IndexOfByKey(PinObj);
		if (Index != INDEX_NONE && Index >= 0 && Index < RoomGraphNodeRef->DoorDatas.Num())
		{
			TempPin->PinLocation = RoomGraphNodeRef->DoorDatas [Index].DoorType;
			TempPin->PinOffset = RoomGraphNodeRef->DoorDatas [Index].DoorOffset;
			if (RoomGraphNodeRef->DoorDatas [Index].DoorType == EMazeDirection::Up 
				|| RoomGraphNodeRef->DoorDatas [Index].DoorType == EMazeDirection::Down)
			{
				TempPin->PinDirection = EMazeOrientation::Vertical;
			}
			else
			{
				TempPin->PinDirection = EMazeOrientation::Horizontal;
			}
			RoomPins.Add(TempPin);
		}
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
	SMazeGraphNodeBase::GetAllPinWidgets(OutPinWidgets);
}

FOptionalSize SRoomGraphNode::GetNodeHeight() const
{
	if (RoomGraphNodeRef)
	{
		if (RoomGraphNodeRef->RoomRotation == 0 || RoomGraphNodeRef->RoomRotation == 180)
		{
			return RoomGraphNodeRef->RoomHeight * TILE_EDITOR_SCALE;
		}
		else if (RoomGraphNodeRef->RoomRotation == 90 || RoomGraphNodeRef->RoomRotation == 270)
		{
			return RoomGraphNodeRef->RoomWidth * TILE_EDITOR_SCALE;
		}
	}
	return 5.0f;
}

FOptionalSize SRoomGraphNode::GetNodeWidth() const
{
	if (RoomGraphNodeRef)
	{
		if (RoomGraphNodeRef->RoomRotation == 0 || RoomGraphNodeRef->RoomRotation == 180)
		{
			return RoomGraphNodeRef->RoomWidth * TILE_EDITOR_SCALE;
		}
		else if (RoomGraphNodeRef->RoomRotation == 90 || RoomGraphNodeRef->RoomRotation == 270)
		{
			return RoomGraphNodeRef->RoomHeight * TILE_EDITOR_SCALE;
		}
	}
	return 5.0f;
}
#undef LOCTEXT_NAMESPACE
