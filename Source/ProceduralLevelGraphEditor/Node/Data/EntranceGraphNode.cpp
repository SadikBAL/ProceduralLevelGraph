#include "EntranceGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/RoomLevelInstance.h"

#define LOCTEXT_NAMESPACE "EntranceGraphNode"

UEntranceGraphNode::UEntranceGraphNode() : URoomGraphNode()
{
}

FText UEntranceGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("EntranceNodeTitle", "Entrance");
}

#undef LOCTEXT_NAMESPACE
