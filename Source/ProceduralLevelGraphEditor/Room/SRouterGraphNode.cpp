#include "SRouterGraphNode.h"
#include "ProceduralLevelGraphEditor/Room/RouterGraphNode.h"
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
	TitleFont.Size = 24;
	UpdateGraphNode();
	
	GetOrAddSlot(ENodeZone::Center)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Center).VAlign(VAlign_Top).Padding(0, -18, 0, 0)
		[ UpPin.IsValid() ? UpPin.ToSharedRef() : SNullWidget::NullWidget ]

		+ SOverlay::Slot()
		.HAlign(HAlign_Center).VAlign(VAlign_Bottom).Padding(0, 0, 0, -18)
		[ DownPin.IsValid() ? DownPin.ToSharedRef() : SNullWidget::NullWidget ]

		+ SOverlay::Slot()
		.HAlign(HAlign_Left).VAlign(VAlign_Center).Padding(-24, 0, 0, 0)
		[ LeftPin.IsValid() ? LeftPin.ToSharedRef() : SNullWidget::NullWidget ]

		+ SOverlay::Slot()
		.HAlign(HAlign_Right).VAlign(VAlign_Center).Padding(0, 0, -18, 0)
		[ RightPin.IsValid() ? RightPin.ToSharedRef() : SNullWidget::NullWidget ]
		+ SOverlay::Slot()
	   [
		  SNew(SBox)
		  .WidthOverride(100.f)
		  .HeightOverride(100.f)
		  [
			 SNew(SBorder)
			 .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
			 .BorderBackgroundColor(FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f)))
			 .HAlign(HAlign_Center) // İçeriği (metni) yatayda ortala
		  	 .VAlign(VAlign_Center)
			 [
				SNew(STextBlock)
				.Text(FText::FromString("0"))
			 	.Font(TitleFont)
			 	
			 ]
		  ]
	   ]
	]; 
}

void SRouterGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
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
#undef LOCTEXT_NAMESPACE
