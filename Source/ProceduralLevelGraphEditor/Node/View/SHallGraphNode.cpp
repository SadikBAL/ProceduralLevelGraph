#include "SHallGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/HallGraphNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "SGraphPin.h"
#include "SPassageGraphNodePin.h"
#include "ProceduralLevelGraphEditor/ProceduralLevelGraphEditor.h"

#define LOCTEXT_NAMESPACE "SRoomGraphNode"

FReply SHallGraphNode::OnRotateBrushClicked(EMazeDirection ButtonLocation)
{
	if (HallGraphNodeRef)
	{
		HallGraphNodeRef->RotateRoomToRight();
	}
	return FReply::Handled();
}

FReply SHallGraphNode::OnMinusBrushClicked(EMazeDirection ButtonLocation)
{
	if (HallGraphNodeRef)
	{
		const bool bShiftDown = FSlateApplication::Get().GetModifierKeys().IsShiftDown();
		const int32 ShiftMult = bShiftDown ? 5 : 1;
		FVector2f LocalPosition = GetPosition2f();
		switch (ButtonLocation)
		{
			case EMazeDirection::Up:
				LocalPosition.Y += TILE_EDITOR_SCALE * ShiftMult;
			break;
			case EMazeDirection::Down:
			break;
			case EMazeDirection::Left:
				LocalPosition.X += TILE_EDITOR_SCALE * ShiftMult;
			break;
			case EMazeDirection::Right:
			break;
			default:
			break;
		}
		GraphNode->SetPosition(LocalPosition);
		HallGraphNodeRef->UpdateHallLength(HallGraphNodeRef->HallLength - ShiftMult);
	}
	return FReply::Handled();
}

FReply SHallGraphNode::OnPlusBrushClicked(EMazeDirection ButtonLocation)
{
	if (HallGraphNodeRef)
	{
		const bool bShiftDown = FSlateApplication::Get().GetModifierKeys().IsShiftDown();
		const int32 ShiftMult = bShiftDown ? 5 : 1;
		FVector2f LocalPosition = GetPosition2f();
		switch (ButtonLocation)
		{
			case EMazeDirection::Up:
				LocalPosition.Y -= (TILE_EDITOR_SCALE * ShiftMult);
				break;
			case EMazeDirection::Down:
				break;
			case EMazeDirection::Left:
				LocalPosition.X -= (TILE_EDITOR_SCALE * ShiftMult);
				break;
			case EMazeDirection::Right:
				break;
			default:
				break;
		}
		GraphNode->SetPosition(LocalPosition);
		HallGraphNodeRef->UpdateHallLength(HallGraphNodeRef->HallLength + ShiftMult);
	}
	return FReply::Handled();
}

void SHallGraphNode::Construct(const FArguments& InArgs, UHallGraphNode* InNode)
{
    this->GraphNode = InNode;
	this->HallGraphNodeRef = InNode;
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle("NormalFont");
	TitleFont.Size = 12;
	this->SetPixelSnapping(EWidgetPixelSnapping::SnapToPixel);
	UpdateGraphNode();
	const FVector2D IconSize(22.0f, 22.0f);
	if (UTexture2D* Texture2D = LoadObject<UTexture2D>(nullptr, TEXT("/ProceduralLevelGraph/Icon/T_Button_Icon_Minus.T_Button_Icon_Minus")))
		ButtonMinusBrush = MakeShareable(new FSlateImageBrush(Texture2D, IconSize));
	if (UTexture2D* Texture2D = LoadObject<UTexture2D>(nullptr, TEXT("/ProceduralLevelGraph/Icon/T_Button_Icon_Plus.T_Button_Icon_Plus")))
		ButtonPlusBrush = MakeShareable(new FSlateImageBrush(Texture2D, IconSize));
	if (UTexture2D* Texture2D = LoadObject<UTexture2D>(nullptr, TEXT("/ProceduralLevelGraph/Icon/T_Button_Icon_Rotate.T_Button_Icon_Rotate")))
		ButtonRotateBrush = MakeShareable(new FSlateImageBrush(Texture2D, IconSize));
    constexpr float EdgePadding = 10.0f;
	const bool bIsVertical = (HallGraphNodeRef->RoomRotation == 0 || HallGraphNodeRef->RoomRotation == 180);
	TSharedPtr<SOverlay> PinOverlay;
	SAssignNew(PinOverlay, SOverlay)
	+ SOverlay::Slot()
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
	    SNew(SBox)
	    .WidthOverride(this, &SHallGraphNode::GetNodeWidth)
	    .HeightOverride(this, &SHallGraphNode::GetNodeHeight)
	    [
	        SNew(SBorder)
	        .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
	    	.BorderBackgroundColor(HallGraphNodeRef->GetNodeBackgroundColor())
	        .HAlign(HAlign_Center)
	        .VAlign(VAlign_Center)
	        [
	            SNew(STextBlock)
	        	.Text(HallGraphNodeRef->GetNodeName())
	            .Font(TitleFont)
	        ]
	    ]
	]
	+ SOverlay::Slot()
	.HAlign(bIsVertical ? HAlign_Center : HAlign_Left)
	.VAlign(bIsVertical ? VAlign_Top : VAlign_Center)
	.Padding(bIsVertical ? FMargin(0, EdgePadding, 0, 0) : FMargin(EdgePadding, 0, 0, 0))
	[
		CreateButtonGroup(bIsVertical,true)
	]

	+ SOverlay::Slot()
	.HAlign(bIsVertical ? HAlign_Center : HAlign_Right)
	.VAlign(bIsVertical ? VAlign_Bottom : VAlign_Center)
	.Padding(bIsVertical ? FMargin(0, 0, 0, EdgePadding) : FMargin(0, 0, EdgePadding, 0))
	[
		CreateButtonGroup(bIsVertical,false)
	];
	for (int32 i = 0; i < Pins.Num(); ++i)
	{
	    TSharedPtr<SPassageGraphNodePin> CurrentPinWidget = Pins[i];
	    if (CurrentPinWidget.IsValid())
	    {
	        UEdGraphPin* PinObj = CurrentPinWidget->GetPinObj();
	        EHorizontalAlignment HAlign = HAlign_Center;
	        EVerticalAlignment VAlign = VAlign_Center;
	        FMargin Padding(0);
	    	int32 LocalOffset = 0;
	    	int32 LocalMult = 1;
	    	if (HallGraphNodeRef->GetRotatedPinDirection(CurrentPinWidget->PinData.DoorLocation) == EMazeDirection::Up) {
	    		//Up
	            VAlign = VAlign_Top;
	    		LocalOffset = (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Left || CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Right) 
	    		? CurrentPinWidget->PinData.DoorOffset.Y : CurrentPinWidget->PinData.DoorOffset.X;
				if (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Down 
					|| CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Left)
				{
					LocalMult = -1;
				}
	            Padding = FMargin(LocalOffset * LocalMult * TILE_EDITOR_SCALE, PinPadding, 0, 0);
	        }
	    	else if (HallGraphNodeRef->GetRotatedPinDirection(CurrentPinWidget->PinData.DoorLocation) == EMazeDirection::Right) {
	    		//Right
	    		HAlign = HAlign_Right;
	    		LocalOffset = (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Left || CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Right) 
				? CurrentPinWidget->PinData.DoorOffset.Y : CurrentPinWidget->PinData.DoorOffset.X;
	    		if (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Down 
					||CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Left)
	    		{
	    			LocalMult = -1;
	    		}
	    		Padding = FMargin(0, LocalOffset * LocalMult  * TILE_EDITOR_SCALE, PinPadding, 0);
	    	}
	        else if (HallGraphNodeRef->GetRotatedPinDirection(CurrentPinWidget->PinData.DoorLocation) == EMazeDirection::Down) {
	            //Left
	        	VAlign = VAlign_Bottom;
	        	LocalOffset = (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Left || CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Right) 
				? CurrentPinWidget->PinData.DoorOffset.Y : CurrentPinWidget->PinData.DoorOffset.X;
	        	if (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Right 
					|| CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Up)
	        	{
	        		LocalMult = -1;
	        	}
	        	Padding = FMargin(LocalOffset * LocalMult  * TILE_EDITOR_SCALE, 0, 2, PinPadding);
	        }
	        else if (HallGraphNodeRef->GetRotatedPinDirection(CurrentPinWidget->PinData.DoorLocation) == EMazeDirection::Left) {
	            //Down
	        	HAlign = HAlign_Left;
	        	LocalOffset = (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Left || CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Right) 
				? CurrentPinWidget->PinData.DoorOffset.Y : CurrentPinWidget->PinData.DoorOffset.X;
	        	if (CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Right
	        		|| CurrentPinWidget->PinData.DoorLocation == EMazeDirection::Up)
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

void SHallGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const TSharedPtr<SGraphPin> BasePinPtr = PinToAdd;
	if (HallGraphNodeRef && HallGraphNodeRef->DoorData.Num() > 0)
	{
		TSharedPtr<SPassageGraphNodePin> TempPin = StaticCastSharedPtr<SPassageGraphNodePin>(BasePinPtr);
		
		int32 Index = HallGraphNodeRef->Pins.IndexOfByKey(PinObj);
		if (Index != INDEX_NONE && Index >= 0 && Index < HallGraphNodeRef->DoorData.Num())
		{
			TempPin->PinData = HallGraphNodeRef->DoorData[Index];
			//TempPin->PinLocation = HallGraphNodeRef->DoorData [Index].DoorLocation;
			//TempPin->PinOffset = HallGraphNodeRef->DoorData [Index].DoorOffset;
			if (HallGraphNodeRef->DoorData [Index].DoorLocation == EMazeDirection::Up 
				|| HallGraphNodeRef->DoorData [Index].DoorLocation == EMazeDirection::Down)
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

TSharedPtr<SGraphPin> SHallGraphNode::CreatePinWidget(UEdGraphPin* Pin) const
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

void SHallGraphNode::GetAllPinWidgets(TArray<TSharedPtr<SGraphPin>>& OutPinWidgets) const
{
	SMazeGraphNodeBase::GetAllPinWidgets(OutPinWidgets);
	for (const auto& Pin : Pins)
	{
		OutPinWidgets.AddUnique(Pin);
	}
}

FOptionalSize SHallGraphNode::GetNodeHeight() const
{
	if (HallGraphNodeRef)
	{
		if (HallGraphNodeRef->RoomRotation == 0 || HallGraphNodeRef->RoomRotation == 180)
		{
			return HallGraphNodeRef->HallLength  * TILE_EDITOR_SCALE;
		}
		else
		{
			return HallGraphNodeRef->RoomWidth * TILE_EDITOR_SCALE;
		}
	}
	return EMPTY_SIZE;
}

FOptionalSize SHallGraphNode::GetNodeWidth() const
{
	if (HallGraphNodeRef)
	{
		if (HallGraphNodeRef->RoomRotation == 0 || HallGraphNodeRef->RoomRotation == 180)
		{
			return HallGraphNodeRef->RoomWidth * TILE_EDITOR_SCALE;
		}
		else
		{
			return HallGraphNodeRef->HallLength * TILE_EDITOR_SCALE;
		}
	}
	return EMPTY_SIZE;
}

TSharedRef<SWidget> SHallGraphNode::CreateButtonGroup(bool bForVerticalNode, bool bIsUpOrLeft)
{
	const FButtonStyle* ButtonStyle = &FAppStyle::Get().GetWidgetStyle<FButtonStyle>("NoBorder");
    if (bForVerticalNode)
    {
        return SNew(SHorizontalBox)
        	+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(1)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.ButtonStyle(ButtonStyle)
				.OnClicked(FOnClicked::CreateLambda([this,bIsUpOrLeft]() -> FReply
					{
						return OnPlusBrushClicked(bIsUpOrLeft ? EMazeDirection::Up : EMazeDirection::Down);
					}))
				.ToolTipText(FText::FromString("Increase Length"))
				[
					SNew(SImage)
					.Image(ButtonPlusBrush.IsValid() ? ButtonPlusBrush.Get() : FAppStyle::GetBrush("Icons.Error"))
				]
			]
        	+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(1)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.ButtonStyle(ButtonStyle)
				.OnClicked(FOnClicked::CreateLambda([this,bIsUpOrLeft]() -> FReply
					{
						return OnRotateBrushClicked(bIsUpOrLeft ? EMazeDirection::Up : EMazeDirection::Down);
					}))
				.ToolTipText(FText::FromString("Rotate Hall"))
				[
					SNew(SImage)
					.Image(ButtonRotateBrush.IsValid() ? ButtonRotateBrush.Get() : FAppStyle::GetBrush("Icons.Error"))
				]
			]
            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(1)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            [
                SNew(SButton)
                .ButtonStyle(ButtonStyle)
            	.OnClicked(FOnClicked::CreateLambda([this,bIsUpOrLeft]() -> FReply
					{
						return OnMinusBrushClicked(bIsUpOrLeft ? EMazeDirection::Up : EMazeDirection::Down);
					}))
                .ToolTipText(FText::FromString("Decrease Length"))
                [
                    SNew(SImage)
                    .Image(ButtonMinusBrush.IsValid() ? ButtonMinusBrush.Get() : FAppStyle::GetBrush("Icons.Error"))
                ]
            ];
    }
    else
    {
        return SNew(SVerticalBox)
        	+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(1)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.ButtonStyle(ButtonStyle)
				.OnClicked(FOnClicked::CreateLambda([this,bIsUpOrLeft]() -> FReply
					{
						return OnPlusBrushClicked(bIsUpOrLeft ? EMazeDirection::Left : EMazeDirection::Right);
					}))
				.ToolTipText(FText::FromString("Increase Length"))
				[
					SNew(SImage)
					.Image(ButtonPlusBrush.IsValid() ? ButtonPlusBrush.Get() : FAppStyle::GetBrush("Icons.Error"))
				]
			]
        	+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(1)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.ButtonStyle(ButtonStyle)
				.OnClicked(FOnClicked::CreateLambda([this,bIsUpOrLeft]() -> FReply
					{
						return OnRotateBrushClicked(bIsUpOrLeft ? EMazeDirection::Left : EMazeDirection::Right);
					}))
				.ToolTipText(FText::FromString("Rotate Hall"))
				[
					SNew(SImage)
					.Image(ButtonRotateBrush.IsValid() ? ButtonRotateBrush.Get() : FAppStyle::GetBrush("Icons.Error"))
				]
			]
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(1)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            [
                SNew(SButton)
                .ButtonStyle(ButtonStyle)
            	.OnClicked(FOnClicked::CreateLambda([this,bIsUpOrLeft]() -> FReply
					 {
						 return OnMinusBrushClicked(bIsUpOrLeft ? EMazeDirection::Left : EMazeDirection::Right);
					 }))
                .ToolTipText(FText::FromString("Decrease Length"))
                [
                    SNew(SImage)
                    .Image(ButtonMinusBrush.IsValid() ? ButtonMinusBrush.Get() : FAppStyle::GetBrush("Icons.Error"))
                ]
            ];
    }
}

#undef LOCTEXT_NAMESPACE
