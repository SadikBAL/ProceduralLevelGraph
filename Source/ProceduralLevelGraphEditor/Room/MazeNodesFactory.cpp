﻿
#include "MazeNodesFactory.h"

#include "HallGraphNode.h"
#include "RoomGraphNode.h"
#include "RouterGraphNode.h"
#include "SHallGraphNode.h"
#include "SRoomGraphNode.h"
#include "SRouterGraphNode.h"

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

TSharedPtr<SGraphNode> FSRouterGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (URouterGraphNode* RouteNode = Cast<URouterGraphNode>(Node))
	{
		return SNew(SRouterGraphNode, RouteNode);
	}
	return nullptr;
}
