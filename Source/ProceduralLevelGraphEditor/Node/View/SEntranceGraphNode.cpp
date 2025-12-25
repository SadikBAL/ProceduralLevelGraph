#include "SEntranceGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/RoomGraphNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"

#define LOCTEXT_NAMESPACE "SEntranceGraphNode"

void SEntranceGraphNode::Construct(const FArguments& InArgs, UEntranceGraphNode* InNode)
{
	SRoomGraphNode::FArguments ParentArgs;
	SRoomGraphNode::Construct(ParentArgs, Cast<URoomGraphNode>(InNode));
}

FText SEntranceGraphNode::GetNodeName() const
{
	return FText::FromString("ENTRANCE");
}

FSlateColor SEntranceGraphNode::GetNodeBackgroundColor() const
{
	return FSlateColor(FLinearColor(0.5f, 0.7f, 0.5f, 0.6f));
}

#undef LOCTEXT_NAMESPACE
