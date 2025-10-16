#include "HallGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "HallGraphNode"

UHallGraphNode::UHallGraphNode()
{
	HallLength = 5.0f;
}
FText UHallGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("HallNodeTitle", "Hall");
}
void UHallGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Up"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Down"));
}
#undef LOCTEXT_NAMESPACE