#include "EntranceGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/RoomLevelInstance.h"

#define LOCTEXT_NAMESPACE "EntranceGraphNode"

UEntranceGraphNode::UEntranceGraphNode()
{
	static ConstructorHelpers::FClassFinder<ALevelInstance> BP_LevelInstance_Finder(
		TEXT("Blueprint'/Game/LevelPrototyping/MazeLevelInstances/Specials/LI_Entrance.LI_Entrance_C'")
	);
	if (BP_LevelInstance_Finder.Succeeded())
	{
		EntranceLevelInstanceRef = BP_LevelInstance_Finder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UEntranceGraphNode constructor: LI_Entrance class not found!"));
	}
	OnTileBlueprintsChanged();
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

void UEntranceGraphNode::OnTileBlueprintsChanged()
{
	if (EntranceLevelInstanceRef) 
	{
		const ARoomLevelInstance* DefaultTile = EntranceLevelInstanceRef->GetDefaultObject<ARoomLevelInstance>();
		if (DefaultTile)
		{
			RoomWidth = DefaultTile->Width;
			RoomHeight = DefaultTile->Height;
		}
	}
}
#undef LOCTEXT_NAMESPACE
