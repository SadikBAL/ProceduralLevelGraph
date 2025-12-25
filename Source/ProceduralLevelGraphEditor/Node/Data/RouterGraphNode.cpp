#include "RouterGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/RoomLevelInstance.h"

#define LOCTEXT_NAMESPACE "RouterGraphNode"

URouterGraphNode::URouterGraphNode() : URoomGraphNode()
{
}

FText URouterGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("RouterNodeTitle", "Router");
}

#undef LOCTEXT_NAMESPACE
