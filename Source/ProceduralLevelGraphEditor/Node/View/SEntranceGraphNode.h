#pragma once
#include "SGraphPin.h"
#include "SRoomGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/EntranceGraphNode.h"

class SEntranceGraphNode : public SRoomGraphNode
{
public:
	SLATE_BEGIN_ARGS(SEntranceGraphNode) {}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, UEntranceGraphNode* InNode);
};
