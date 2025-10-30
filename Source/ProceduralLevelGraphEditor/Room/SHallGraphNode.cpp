#include "SHallGraphNode.h"
#include "ProceduralLevelGraphEditor/Room/HallGraphNode.h"
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
	if (InNode->bHorizentalMode)
	{
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
			  .WidthOverride(GetNodeHeight())
			  .HeightOverride(GetNodeWidth())
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
	.HAlign(HAlign_Left).VAlign(VAlign_Center).Padding(PinPadding-4,0,0,0)
	[ UpPin.IsValid() ? UpPin.ToSharedRef() : SNullWidget::NullWidget ]

	+ SOverlay::Slot()
	.HAlign(HAlign_Right).VAlign(VAlign_Center).Padding(0,0,PinPadding,0)
	[ DownPin.IsValid() ? DownPin.ToSharedRef() : SNullWidget::NullWidget ]

		];
	}
	else
	{
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
			[ UpPin.IsValid() ? UpPin.ToSharedRef() : SNullWidget::NullWidget ]

			+ SOverlay::Slot()
			.HAlign(HAlign_Center).VAlign(VAlign_Bottom).Padding(0,0,0,PinPadding)
			[ DownPin.IsValid() ? DownPin.ToSharedRef() : SNullWidget::NullWidget ]
		];
	}

}

void SHallGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	const FName PinName = PinToAdd->GetPinObj()->GetFName();
	TSharedPtr<SGraphPin> BasePinPtr = PinToAdd;
	if (PinName == FName("Up"))
	{
		UpPin = StaticCastSharedPtr<SRoomGraphNodePin>(BasePinPtr);
		if (HallGraphNodeRef->bHorizentalMode)
		{
			UpPin->PinLocation = EMazeDirection::Left;
		}
		else
		{
			UpPin->PinLocation = EMazeDirection::Up;
		}
	}
	else if (PinName == FName("Down"))
	{
		DownPin = StaticCastSharedPtr<SRoomGraphNodePin>(BasePinPtr);
		if (HallGraphNodeRef->bHorizentalMode)
		{
			DownPin->PinLocation = EMazeDirection::Right;
		}
		else
		{
			DownPin->PinLocation = EMazeDirection::Down;
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
		TSharedPtr<SGraphPin> PinWidget = SNew(SRoomGraphNodePin, Pin);
		PinWidget->SetShowLabel(false);
		return PinWidget;
	}

}

void SHallGraphNode::GetAllPinWidgets(TArray<TSharedPtr<SGraphPin>>& OutPinWidgets) const
{
	if(UpPin.IsValid()) OutPinWidgets.Add(UpPin);
	if(DownPin.IsValid()) OutPinWidgets.Add(DownPin);
	SMazeGraphNodeBase::GetAllPinWidgets(OutPinWidgets);
}

FOptionalSize SHallGraphNode::GetNodeHeight() const
{
	if (UHallGraphNode* HallNode = Cast<UHallGraphNode>(GraphNode))
	{
		return FMath::Max(20.0f, HallNode->HallLength * 100); 
	}
	return 200.0f;
}

FOptionalSize SHallGraphNode::GetNodeWidth() const
{
	return FOptionalSize(100.0f);
}

#undef LOCTEXT_NAMESPACE
