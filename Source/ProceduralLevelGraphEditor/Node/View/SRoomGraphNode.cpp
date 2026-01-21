#include "SRoomGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/RoomGraphNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"

#include "SGraphPin.h"
#include "SPassageGraphNodePin.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

#define LOCTEXT_NAMESPACE "SRoomGraphNode"

void SRoomGraphNode::Construct(const FArguments& InArgs, URoomGraphNode* InNode)
{
	this->GraphNode = InNode;
	this->RoomGraphNodeRef =  Cast<URoomGraphNode>(GraphNode);
	this->SetPixelSnapping(EWidgetPixelSnapping::SnapToPixel);
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle("NormalFont");
	TitleFont.Size = 10;
	UpdateGraphNode();
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
	    	.BorderBackgroundColor(RoomGraphNodeRef->GetNodeBackgroundColor())
	        .HAlign(HAlign_Center)
	        .VAlign(VAlign_Center)
	        [
	            SNew(STextBlock)
	        	.Text(RoomGraphNodeRef->GetNodeName())
	            .Font(TitleFont)
	        ]
	    ]
	];
	for (int32 i = 0; i < Pins.Num(); ++i)
	{
	    TSharedPtr<SPassageGraphNodePin> CurrentPinWidget = Pins[i];
	    if (CurrentPinWidget.IsValid())
	    {
	        EHorizontalAlignment HAlign = HAlign_Center;
	        EVerticalAlignment VAlign = VAlign_Center;
	        FMargin Padding(0);
	    	int32 LocalOffset = 0;
	    	int32 LocalMult = 1;
	    	if (RoomGraphNodeRef->GetRotatedPinDirection(CurrentPinWidget->PinData.DoorLocation) == EMazeDirection::Up) {
	            VAlign = VAlign_Top;
	    		LocalOffset = (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Left || CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Right) 
	    		? CurrentPinWidget->PinData.DoorOffset.Y : CurrentPinWidget->PinData.DoorOffset.X;
				if (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Down 
					|| CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Left)
				{
					LocalMult = -1;
				}
	    		float ScaledOffset = LocalOffset * LocalMult * TILE_EDITOR_SCALE;
	    		//float AbsOffset2x = FMath::Abs(ScaledOffset) * 2.0f;

	    		if (ScaledOffset >= 0)
	    		{
	    			Padding = FMargin(ScaledOffset, PinPadding, 0, 0);
	    		}
	    		else
	    		{
	    			Padding = FMargin(0, PinPadding, ScaledOffset, 0);
	    		}
	        }
	    	else if (RoomGraphNodeRef->GetRotatedPinDirection(CurrentPinWidget->PinData.DoorLocation) == EMazeDirection::Right) {
	    		HAlign = HAlign_Right;
	    		LocalOffset = (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Left || CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Right) 
				? CurrentPinWidget->PinData.DoorOffset.Y : CurrentPinWidget->PinData.DoorOffset.X;
	    		if (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Down 
					||CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Left)
	    		{
	    			LocalMult = -1;
	    		}
	    		float ScaledOffset = LocalOffset * LocalMult * TILE_EDITOR_SCALE;
	    		//float AbsOffset2x = FMath::Abs(ScaledOffset) * 2.0f;

	    		if (ScaledOffset >= 0)
	    		{
	    			Padding = FMargin(0, ScaledOffset, PinPadding, 0);
	    		}
	    		else
	    		{
	    			Padding = FMargin(0, 0, PinPadding, ScaledOffset);
	    		}
	    	}
	        else if (RoomGraphNodeRef->GetRotatedPinDirection(CurrentPinWidget->PinData.DoorLocation) == EMazeDirection::Down) {
	        	VAlign = VAlign_Bottom;
	        	LocalOffset = (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Left || CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Right) 
				? CurrentPinWidget->PinData.DoorOffset.Y : CurrentPinWidget->PinData.DoorOffset.X;
	        	if (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Right 
					|| CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Up)
	        	{
	        		LocalMult = -1;
	        	}
	        	float ScaledOffset = LocalOffset * LocalMult * TILE_EDITOR_SCALE;
	        	//float AbsOffset2x = FMath::Abs(ScaledOffset) * 2.0f;

	        	if (ScaledOffset >= 0)
	        	{
	        		Padding = FMargin(ScaledOffset, 0, 0, PinPadding);
	        	}
	        	else
	        	{
	        		Padding = FMargin(0, 0, ScaledOffset, PinPadding);
	        	}
	        }
	        else if (RoomGraphNodeRef->GetRotatedPinDirection(CurrentPinWidget->PinData.DoorLocation) == EMazeDirection::Left) {
	        	HAlign = HAlign_Left;
	        	LocalOffset = (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Left || CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Right) 
				? CurrentPinWidget->PinData.DoorOffset.Y : CurrentPinWidget->PinData.DoorOffset.X;
	        	if (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Right
	        		|| CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Up)
	        	{
	        		LocalMult = -1;
	        	}
	        	float ScaledOffset = LocalOffset * LocalMult * TILE_EDITOR_SCALE;
	        	//float AbsOffset2x = FMath::Abs(ScaledOffset) * 2.0f;

	        	if (ScaledOffset >= 0)
	        	{
	        		Padding = FMargin(PinPadding-4, ScaledOffset, 0, 0);
	        	}
	        	else
	        	{
	        		Padding = FMargin(PinPadding-4, 0, 0, ScaledOffset);
	        	}
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
	UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const TSharedPtr<SGraphPin> BasePinPtr = PinToAdd;
	if (RoomGraphNodeRef && RoomGraphNodeRef->DoorData.Num() > 0)
	{
		TSharedPtr<SPassageGraphNodePin> TempPin = StaticCastSharedPtr<SPassageGraphNodePin>(BasePinPtr);
		
		int32 Index = RoomGraphNodeRef->Pins.IndexOfByKey(PinObj);
		if (Index != INDEX_NONE && Index >= 0 && Index < RoomGraphNodeRef->DoorData.Num())
		{
			TempPin->PinData = RoomGraphNodeRef->DoorData[Index];
			//TempPin->PinLocation = RoomGraphNodeRef->DoorData [Index].DoorLocation;
			//TempPin->PinOffset = RoomGraphNodeRef->DoorData [Index].DoorOffset;
			if (RoomGraphNodeRef->DoorData [Index].DoorLocation == EMazeDirection::Up 
				|| RoomGraphNodeRef->DoorData [Index].DoorLocation == EMazeDirection::Down)
			{
				TempPin->PinData.DoorDirection = EMazeOrientation::Vertical;
			}
			else
			{
				TempPin->PinData.DoorDirection = EMazeOrientation::Horizontal;
			}
			TempPin->SetToolTipText(
				TAttribute<FText>::Create(
					TAttribute<FText>::FGetter::CreateSP(TempPin.ToSharedRef(), &SPassageGraphNodePin::GetCustomToolTipText)
				)
			);
			Pins.Add(TempPin);
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
		TSharedPtr<SGraphPin> PinWidget = SNew(SPassageGraphNodePin, Pin);
		PinWidget->SetShowLabel(false);
		return PinWidget;
	}

}

void SRoomGraphNode::GetAllPinWidgets(TArray<TSharedPtr<SGraphPin>>& OutPinWidgets) const
{
	SMazeGraphNodeBase::GetAllPinWidgets(OutPinWidgets);
	for (const auto& Pin : Pins)
	{
		OutPinWidgets.AddUnique(Pin);
	}
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
	return EMPTY_SIZE;
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
	return EMPTY_SIZE;
}

#undef LOCTEXT_NAMESPACE
