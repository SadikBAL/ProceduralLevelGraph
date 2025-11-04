#include "RoomGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "RoomGraphNode"

URoomGraphNode::URoomGraphNode()
{
    RoomHeight = 5;
    RoomWith = 5;
}

FText URoomGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("RoomNodeTitle", "Room");
}
void URoomGraphNode::AllocateDefaultPins()
{
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Up"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Down"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Left"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Right"));
}
#undef LOCTEXT_NAMESPACE
