#pragma once

#include "CoreMinimal.h"
#include "SMazeGraphNodeBase.h"
#include "SRoomGraphNode.h"
#include "SRoomGraphNodePin.h"

class URouterGraphNode;

class SRouterGraphNode : public SRoomGraphNode
{
public:
	SLATE_BEGIN_ARGS(SRouterGraphNode) {}
	SLATE_END_ARGS()
};