
#include "SRoomGraphNodeFactory.h"
#include "RoomGraphNode.h"
#include "SRoomGraphNode.h"

TSharedPtr<SGraphNode> FSRoomGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (URoomGraphNode* RoomNode = Cast<URoomGraphNode>(Node))
	{
		return SNew(SRoomGraphNode, RoomNode);
	}
	return nullptr;
}
