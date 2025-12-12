#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelStreamingDynamic.h"
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
	UPROPERTY(BlueprintReadOnly, Category = "Maze Node")
	TObjectPtr<UMazeNodeBase> DownNode;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Node")
	TObjectPtr<UMazeNodeBase> LeftNode;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Node")
	TObjectPtr<UMazeNodeBase> RightNode;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Node")
	TObjectPtr<UMazeNodeBase> UpNode;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Node")
	TArray<UMazeNodeBase*> Others;

	UFUNCTION(BlueprintCallable)
	virtual  void SpawnMazeObject(UWorld* World,FVector Position, EMazeDirection Direction);
	UFUNCTION(BlueprintCallable)
	virtual FVector GetEdgePosition(EMazeDirection Direction);
	UFUNCTION(BlueprintCallable)
	virtual float GetHalfDistanceOfRoom(EMazeOrientation Orientation);
	
protected:
	UPROPERTY()
	FVector SpawnLocation = FVector::Zero();
	
};