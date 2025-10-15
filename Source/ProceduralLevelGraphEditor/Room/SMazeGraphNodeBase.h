#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class UHallGraphNode;

class SMazeGraphNodeBase : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SMazeGraphNodeBase) {}
	SLATE_END_ARGS()

	virtual void GetAllPinWidgets(TArray<TSharedPtr<SGraphPin>>& OutPinWidgets) const;
};