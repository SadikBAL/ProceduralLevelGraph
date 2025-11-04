#pragma once

#include "CoreMinimal.h"
#include "MazeNodeBase.h"
#include "RouterNode.generated.h"

UCLASS(BlueprintType)
class PROCEDURALLEVELGRAPHRUNTIME_API URouterNode : public UMazeNodeBase
{
	GENERATED_BODY()
	URouterNode();
public:
	virtual float GetHalfDistanceOfRoom(EMazeOrientation Orientation) override;
};