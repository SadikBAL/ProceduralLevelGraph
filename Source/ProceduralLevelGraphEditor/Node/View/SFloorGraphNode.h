#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/FloorGraphNode.h"

class SFloorGraphNode: public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SFloorGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UFloorGraphNode* InNode);
	virtual float GetLayerOffset() const;
	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
	virtual FOptionalSize GetNodeWidth() const;
	virtual FOptionalSize GetNodeHeight() const;
private:
	float NodeWidth = 100.0f;
	float NodeHeight = 100.0f;
};
