#include "HallGraphNode.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "HallGraphNode"

UHallGraphNode::UHallGraphNode()
{
	HallLength = 5.0f;
	static ConstructorHelpers::FClassFinder<ALevelInstance> BP_LevelInstance_Finder(
		TEXT("Blueprint'/Game/LevelPrototyping/MazeLevelInstances/Halls/LI_Hall_4X4.LI_Hall_4X4_C'")
	);
	if (BP_LevelInstance_Finder.Succeeded())
	{
		HallLevelInstanceRef = BP_LevelInstance_Finder.Class;
	}
	else
	{
		HallLevelInstanceRef = nullptr;
		UE_LOG(LogTemp, Error, TEXT("UHallGraphNode constructor: LI_Hall_4X4 class not found!"));
	}
	OnTileBlueprintsChanged();
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
	if (HallLevelInstanceRef) 
	{
		const AHallLevelInstance* DefaultTile = HallLevelInstanceRef->GetDefaultObject<AHallLevelInstance>();
		if (DefaultTile)
		{
			RoomWidth = DefaultTile->Width;
		}
	}
}

void UHallGraphNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) 
					   ? PropertyChangedEvent.Property->GetFName() 
					   : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UHallGraphNode, HallLevelInstanceRef))
	{
		OnTileBlueprintsChanged();
	}
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
