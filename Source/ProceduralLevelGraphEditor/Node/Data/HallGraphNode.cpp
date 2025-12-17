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
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Left"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, FName("Right"));
}

EMazePinType UHallGraphNode::GetPinType()
{
	return Super::GetPinType();
}

void UHallGraphNode::OnTileBlueprintsChanged()
{
	
}

void UHallGraphNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UHallGraphNode::UpdateHallLength(int Value)
{
	HallLength = Value;
	if (UEdGraph* MyGraph = GetGraph())
	{
		MyGraph->NotifyGraphChanged();
	}
}

#undef LOCTEXT_NAMESPACE
