#pragma once

#include "CoreMinimal.h"
#include "RoomNode.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/RoomLevelInstance.h"
#include "RouterNode.generated.h"

UCLASS(BlueprintType)
class PROCEDURALLEVELGRAPHRUNTIME_API URouterNode : public URoomNode
{
	GENERATED_BODY()
	
public:
	
	URouterNode();
};