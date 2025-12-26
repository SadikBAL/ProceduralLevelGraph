#pragma once

#include "CoreMinimal.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#include "MazeNodeBase.generated.h"

class AMazeTileLevelInstance;

UCLASS(BlueprintType)
class PROCEDURALLEVELGRAPHRUNTIME_API UMazeNodeBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Maze Node")
	FGuid NodeGuid;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Node")
	int RoomRotation = 0;
	
	UPROPERTY(BlueprintReadOnly, Category = "Node Process")
	TArray<FDoorData> DoorData;

	UFUNCTION(BlueprintCallable)
	virtual  void SpawnMazeObject(UWorld* World,FVector Position, EMazeDirection Direction);
	UFUNCTION(BlueprintCallable)
	virtual FVector GetEdgePosition(EMazeDirection Direction);
	UFUNCTION(BlueprintCallable)
	virtual float GetHalfDistanceOfRoom(EMazeOrientation Orientation);
	UFUNCTION(BlueprintCallable)
	EMazeDirection GetRotatedPinDirection(EMazeDirection Unrotated);
	UFUNCTION(BlueprintCallable)
	FVector GetDoorPosition(EMazeDirection Direction,FDoorData Door);
	
protected:
	UPROPERTY()
	FVector SpawnLocation = FVector::Zero();
	
};