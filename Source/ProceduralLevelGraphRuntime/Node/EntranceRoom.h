#pragma once

#include "CoreMinimal.h"
#include "MazeNodeBase.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/RoomLevelInstance.h"
#include "EntranceRoom.generated.h"

UCLASS(BlueprintType)
class PROCEDURALLEVELGRAPHRUNTIME_API UEntranceNode : public UMazeNodeBase
{
	GENERATED_BODY()
	UEntranceNode();
public:
	
	virtual float GetHalfDistanceOfRoom(EMazeOrientation Orientation) override;
	virtual void SpawnMazeObject(UWorld* World, FVector Position, EMazeDirection Direction) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Entrance Node")
	float RoomWidth;
	UPROPERTY(BlueprintReadOnly, Category = "Entrance Node")
	float RoomHeight;
	UPROPERTY(BlueprintReadOnly, Category = "Entrance Node")
	FVector RoomPosition;
	UPROPERTY(BlueprintReadOnly, Category = "Entrance Node")
	TSubclassOf<ARoomLevelInstance> EntranceInstanceRef;
};