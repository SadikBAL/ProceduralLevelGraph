#pragma once

#include "CoreMinimal.h"
#include "MazeNodeBase.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/RoomLevelInstance.h"
#include "RouterNode.generated.h"

UCLASS(BlueprintType)
class PROCEDURALLEVELGRAPHRUNTIME_API URouterNode : public UMazeNodeBase
{
	GENERATED_BODY()
	URouterNode();
public:
	UPROPERTY(BlueprintReadOnly, Category = "Entrance Node")
	float RoomWidth;
	UPROPERTY(BlueprintReadOnly, Category = "Entrance Node")
	float RoomHeight;
	virtual float GetHalfDistanceOfRoom(EMazeOrientation Orientation) override;
	UPROPERTY(BlueprintReadOnly, Category = "Entrance Node")
	TSubclassOf<ARoomLevelInstance> RouterInstanceRef;
	virtual void SpawnMazeObject(UWorld* World, FVector Position, EMazeDirection Direction) override;
};