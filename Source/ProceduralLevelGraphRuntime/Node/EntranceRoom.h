#pragma once

#include "CoreMinimal.h"
#include "MazeNodeBase.h"
#include "RoomNode.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/RoomLevelInstance.h"
#include "EntranceRoom.generated.h"

UCLASS(BlueprintType)
class PROCEDURALLEVELGRAPHRUNTIME_API UEntranceNode : public URoomNode
{
	GENERATED_BODY()
	
public:
	
	UEntranceNode();
	
	UPROPERTY(BlueprintReadOnly, Category = "Entrance Node")
	FVector RoomPosition;

};