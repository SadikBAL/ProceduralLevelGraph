#pragma once

#include "CoreMinimal.h"
#include "RoomGraphNode.h"
#include "RouterGraphNode.generated.h"

class ARoomLevelInstance;

UCLASS()
class URouterGraphNode : public URoomGraphNode
{
	GENERATED_BODY()
	URouterGraphNode();
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetNodeName() const override;
	virtual FSlateColor GetNodeBackgroundColor() const override;
	
};