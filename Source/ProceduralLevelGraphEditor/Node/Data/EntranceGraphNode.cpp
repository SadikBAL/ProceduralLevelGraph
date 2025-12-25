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

FText UEntranceGraphNode::GetNodeName() const
{
	return FText::FromString("ENTRANCE");
}

FSlateColor UEntranceGraphNode::GetNodeBackgroundColor() const
{
	return FSlateColor(FLinearColor(0.5f, 0.7f, 0.5f, 0.45f));
}

#undef LOCTEXT_NAMESPACE
