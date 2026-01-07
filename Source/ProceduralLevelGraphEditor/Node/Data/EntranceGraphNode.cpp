#include "EntranceGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/RoomLevelInstance.h"

#define LOCTEXT_NAMESPACE "EntranceGraphNode"

	UEntranceGraphNode::UEntranceGraphNode() : URoomGraphNode()
{
	static ConstructorHelpers::FClassFinder<ALevelInstance> BP_LevelInstance_Finder(
		TEXT("Blueprint'/Game/LevelPrototyping/MazeLevelInstances/Specials/BP_Entrance.BP_Entrance_C'")
	);
	if (BP_LevelInstance_Finder.Succeeded())
	{
		TSubclassOf<ARoomLevelInstance> DefaultLevelInstance {BP_LevelInstance_Finder.Class};
		RoomLevelInstanceRef.Add(DefaultLevelInstance);
	}
	else
	{
		UE_LOG_EDITOR(LogTemp, Error, TEXT("UEntranceGraphNode constructor: BP_Entrance class not found!"));
	}
}

FText UEntranceGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("EntranceNodeTitle", "Entrance");
}

FText UEntranceGraphNode::GetNodeName() const
{
	return FText::FromString("ENTRANCE");
}

FSlateColor UEntranceGraphNode::GetNodeBackgroundColor() const
{
	if (DoorData.Num() > 0)
		return FSlateColor(FLinearColor(0.5f, 0.7f, 0.5f, 0.45f));
	return FSlateColor(FLinearColor(1.0f, 0.1f, 0.1f, 0.6f));
}

bool UEntranceGraphNode::CanUserCopyNode() const
{
	return false;
}

bool UEntranceGraphNode::CanUserDeleteNode() const
{
	return false;
}

#undef LOCTEXT_NAMESPACE
