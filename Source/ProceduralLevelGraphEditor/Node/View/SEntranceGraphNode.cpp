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

#undef LOCTEXT_NAMESPACE
