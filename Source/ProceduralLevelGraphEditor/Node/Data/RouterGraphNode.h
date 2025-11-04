#pragma once

#include "CoreMinimal.h"
#include "MazeGraphNodeBase.h"
#include "RouterGraphNode.generated.h"

UCLASS()
class URouterGraphNode : public UMazeGraphNodeBase
{
	GENERATED_BODY()
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
};