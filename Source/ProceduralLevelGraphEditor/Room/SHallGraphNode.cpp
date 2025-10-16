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
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle("NormalFont");
	TitleFont.Size = 24;
	UpdateGraphNode();
	if (InNode->bHorizentalMode)
	{
		GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
				
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Left).VAlign(VAlign_Center).Padding(-24, 0, 0, 0)
			[ UpPin.IsValid() ? UpPin.ToSharedRef() : SNullWidget::NullWidget ]

			+ SOverlay::Slot()
			.HAlign(HAlign_Right).VAlign(VAlign_Center).Padding(0, 0,-18, 0)
			[ DownPin.IsValid() ? DownPin.ToSharedRef() : SNullWidget::NullWidget ]
			+ SOverlay::Slot()
		   [
			  SNew(SBox)
			  .HeightOverride(80.f)
		   	  .WidthOverride(this, &SHallGraphNode::GetNodeHeight)
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
			.HAlign(HAlign_Center).VAlign(VAlign_Top).Padding(0, -18, 0, 0)
			[ UpPin.IsValid() ? UpPin.ToSharedRef() : SNullWidget::NullWidget ]

			+ SOverlay::Slot()
			.HAlign(HAlign_Center).VAlign(VAlign_Bottom).Padding(0, 0,0, -18)
			[ DownPin.IsValid() ? DownPin.ToSharedRef() : SNullWidget::NullWidget ]
			+ SOverlay::Slot()
		   [
			  SNew(SBox)
			  .WidthOverride(80.f)
				 .HeightOverride(this, &SHallGraphNode::GetNodeHeight)
			  [
				 SNew(SBorder)
				 .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
				 .BorderBackgroundColor(FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f)))
				 .HAlign(HAlign_Center) // İçeriği (metni) yatayda ortala
			  	 .VAlign(VAlign_Center)
				 [
					SNew(STextBlock)
					.Text(FText::FromString("HALL"))
					 .Font(TitleFont)
				 ]
			  ]
		   ]
		]; 
	}

}

void SHallGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
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

#undef LOCTEXT_NAMESPACE
