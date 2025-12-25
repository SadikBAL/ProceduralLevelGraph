#include "SRouterGraphNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"


#define LOCTEXT_NAMESPACE "SRouterGraphNode"

void SRouterGraphNode::Construct(const FArguments& InArgs, URouterGraphNode* InNode)
{
	SRoomGraphNode::FArguments ParentArgs;
	SRoomGraphNode::Construct(ParentArgs, Cast<URoomGraphNode>(InNode));
}

#undef LOCTEXT_NAMESPACE
