#include "RouterGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/RoomLevelInstance.h"

#define LOCTEXT_NAMESPACE "RouterGraphNode"

URouterGraphNode::URouterGraphNode()
{
	static ConstructorHelpers::FClassFinder<ALevelInstance> BP_LevelInstance_Finder(
		TEXT("Blueprint'/Game/LevelPrototyping/MazeLevelInstances/Specials/LI_Router.LI_Router_C'")
	);
	if (BP_LevelInstance_Finder.Succeeded())
	{
		RouterLevelInstanceRef = BP_LevelInstance_Finder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UEntranceGraphNode constructor: LI_Entrance class not found!"));
	}
	OnTileBlueprintsChanged();
}

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

void URouterGraphNode::OnTileBlueprintsChanged()
{
	if (RouterLevelInstanceRef) 
	{
		const ARoomLevelInstance* DefaultTile = RouterLevelInstanceRef->GetDefaultObject<ARoomLevelInstance>();
		if (DefaultTile)
		{
			RoomWidth = DefaultTile->Width;
			RoomHeight = DefaultTile->Height;
		}
	}
}
#undef LOCTEXT_NAMESPACE
