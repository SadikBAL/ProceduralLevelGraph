#include "HallGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "HallGraphNode"

UHallGraphNode::UHallGraphNode()
{
	HallLength = 5.0f;
	static ConstructorHelpers::FClassFinder<AActor> BP_Room_ClassFinder(
		TEXT("Blueprint'/Game/LevelPrototyping/BP_Hall.BP_Hall_C'")
		);
	if (BP_Room_ClassFinder.Succeeded())
	{
		HallBlueprint = BP_Room_ClassFinder.Class;
	}
	else
	{
		HallBlueprint = nullptr;
		UE_LOG(LogTemp, Error, TEXT("UHallGraphNode constructor: BP_Hall_C sınıfı bulunamadı!"));
	}
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

void UHallGraphNode::UpdateHallLength(int Value)
{
	HallLength = Value;
	if (UEdGraph* MyGraph = GetGraph())
	{
		MyGraph->NotifyGraphChanged();
	}
}


#undef LOCTEXT_NAMESPACE
