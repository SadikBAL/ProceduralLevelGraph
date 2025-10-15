
#include "MazeNodesFactory.h"

#include "HallGraphNode.h"
#include "RoomGraphNode.h"
#include "SHallGraphNode.h"
#include "SRoomGraphNode.h"

TSharedPtr<SGraphNode> FSRoomGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (URoomGraphNode* RoomNode = Cast<URoomGraphNode>(Node))
	{
		return SNew(SRoomGraphNode, RoomNode);
	}
	return nullptr;
}
TSharedPtr<SGraphNode> FSHallGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (UHallGraphNode* HallNode = Cast<UHallGraphNode>(Node))
	{
		return SNew(SHallGraphNode, HallNode);
	}
	return nullptr;
}
