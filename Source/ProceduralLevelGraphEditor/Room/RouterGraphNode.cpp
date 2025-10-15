#include "RouterGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "RouterGraphNode"

FText URouterGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("RouterNodeTitle", "Router");
}
void URouterGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Up"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Down"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Left"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Right"));
}
#undef LOCTEXT_NAMESPACE