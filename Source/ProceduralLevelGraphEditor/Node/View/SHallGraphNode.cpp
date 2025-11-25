#include "SHallGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/HallGraphNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"

#include "SGraphPin.h"
#include "SRoomGraphNodePin.h"
#include "ProceduralLevelGraphEditor/ProceduralLevelGraphEditor.h"
#include "Toolkits/ToolkitManager.h"

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
		FVector2f LocalPosition = GetPosition2f();
		switch (ButtonLocation)
		{
			case EMazeDirection::Up:
				LocalPosition.Y += TILE_EDITOR_SCALE;
			break;
			case EMazeDirection::Down:
			break;
			case EMazeDirection::Left:
				LocalPosition.X += TILE_EDITOR_SCALE;
			break;
			case EMazeDirection::Right:
			break;
			default:
			break;
		}
		GraphNode->SetPosition(LocalPosition);
		HallGraphNodeRef->UpdateHallLength(HallGraphNodeRef->HallLength - 1);
	}
	return FReply::Handled();
}

FReply SHallGraphNode::OnPlusBrushClicked(EMazeDirection ButtonLocation)
{
	if (HallGraphNodeRef)
	{
		FVector2f LocalPosition = GetPosition2f();
		switch (ButtonLocation)
		{
			case EMazeDirection::Up:
				LocalPosition.Y -= TILE_EDITOR_SCALE;
				break;
			case EMazeDirection::Down:
				break;
			case EMazeDirection::Left:
				LocalPosition.X -= TILE_EDITOR_SCALE;
				break;
			case EMazeDirection::Right:
				break;
			default:
				break;
		}
		GraphNode->SetPosition(LocalPosition);
		HallGraphNodeRef->UpdateHallLength(HallGraphNodeRef->HallLength + 1);
	}
	return FReply::Handled();
}

void SHallGraphNode::Construct(const FArguments& InArgs, UHallGraphNode* InNode)
{
    this->GraphNode = InNode;
	this->HallGraphNodeRef = InNode;
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle("NormalFont");
	TitleFont.Size = 12;
	UpdateGraphNode();
	UpdatePinTypes();
	const FVector2D IconSize(16.0f * (TILE_EDITOR_SCALE / 100.0f), 16.0f * (TILE_EDITOR_SCALE / 100.0f));
	if (UTexture2D* Texture2D = LoadObject<UTexture2D>(nullptr, TEXT("/ProceduralLevelGraph/Icons/T_Button_Icon_Minus.T_Button_Icon_Minus")))
		ButtonMinusBrush = MakeShareable(new FSlateImageBrush(Texture2D, IconSize));
	if (UTexture2D* Texture2D = LoadObject<UTexture2D>(nullptr, TEXT("/ProceduralLevelGraph/Icons/T_Button_Icon_Plus.T_Button_Icon_Plus")))
		ButtonPlusBrush = MakeShareable(new FSlateImageBrush(Texture2D, IconSize));
	if (UTexture2D* Texture2D = LoadObject<UTexture2D>(nullptr, TEXT("/ProceduralLevelGraph/Icons/T_Button_Icon_Rotate.T_Button_Icon_Rotate")))
		ButtonRotateBrush = MakeShareable(new FSlateImageBrush(Texture2D, IconSize));
	const float EdgePadding = 10.0f;
	const bool bIsVertical = (HallGraphNodeRef->RoomRotation == 0 || HallGraphNodeRef->RoomRotation == 180);
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
