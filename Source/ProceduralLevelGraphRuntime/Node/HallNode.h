#pragma once

#include "CoreMinimal.h"
#include "MazeNodeBase.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/HallLevelInstance.h"
#include "HallNode.generated.h"

UCLASS(BlueprintType)
class PROCEDURALLEVELGRAPHRUNTIME_API UHallNode : public UMazeNodeBase
{
	GENERATED_BODY()
	UHallNode();
public:
	virtual float GetHalfDistanceOfRoom(EMazeOrientation Orientation) override;
	virtual void SpawnMazeObject(UWorld* World, FVector Position, EMazeDirection Direction) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Hall Node")
	float HallLength;
	UPROPERTY(BlueprintReadOnly, Category = "Hall Node")
	int RoomTile;
	TSubclassOf<AHallLevelInstance> HallLevelInstanceRef;
};