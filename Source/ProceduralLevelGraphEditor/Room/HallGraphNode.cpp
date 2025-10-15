#include "HallGraphNode.h"

#include "GameplayTagContainer.h"
#include "EdGraph/EdGraphPin.h"
#include "Framework/Commands/GenericCommands.h"

#define LOCTEXT_NAMESPACE "HallGraphNode"

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