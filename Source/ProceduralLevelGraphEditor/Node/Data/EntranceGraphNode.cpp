#include "EntranceGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "RoomGraphNode"

UEntranceGraphNode::UEntranceGraphNode()
{
	RoomHeight = 1;
	RoomWith = 3;
}

FText UEntranceGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("EntranceNodeTitle", "Entrance");
}
void UEntranceGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Up"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Down"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Left"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Right"));
}

bool UEntranceGraphNode::CanUserDeleteNode() const
{
	return false;
}

bool UEntranceGraphNode::CanUserCopyNode() const
{
	return false;
}
#undef LOCTEXT_NAMESPACE
