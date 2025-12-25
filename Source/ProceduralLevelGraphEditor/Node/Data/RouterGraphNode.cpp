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

FText URouterGraphNode::GetNodeName() const
{
	return FText::FromString("ROUTER");
}

FSlateColor URouterGraphNode::GetNodeBackgroundColor() const
{
	return FSlateColor(FLinearColor(0.4f, 0.4f, 0.9f, 0.3f));
}

#undef LOCTEXT_NAMESPACE
