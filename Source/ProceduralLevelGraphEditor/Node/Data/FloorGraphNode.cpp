#include "FloorGraphNode.h"

#define LOCTEXT_NAMESPACE "FloorNode"

UFloorGraphNode::UFloorGraphNode()
{
	NodeWidth = 1000.0f;
	NodeHeight = 1000.0f;
}

void UFloorGraphNode::AllocateDefaultPins()
{
}

bool UFloorGraphNode::CanUserDeleteNode() const
{
	return true;
}

FText UFloorGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("FloorNodeTitle", "Floor Node");
}

FLinearColor UFloorGraphNode::GetNodeTitleColor() const
{
	return FLinearColor(0.1f, 0.1f, 0.1f, 0.5f); 
}

#undef LOCTEXT_NAMESPACE