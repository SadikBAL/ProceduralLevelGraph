#pragma once

#include "CoreMinimal.h"
#include "SRoomGraphNode.h"

class URouterGraphNode;

class SRouterGraphNode : public SRoomGraphNode
{
public:
	SLATE_BEGIN_ARGS(SRouterGraphNode) {}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, URouterGraphNode* InNode);
};