#pragma once

#include "CoreMinimal.h"
#include "MazeNodeBase.h"
#include "HallNode.generated.h"

UCLASS(BlueprintType)
class PROCEDURALLEVELGRAPHRUNTIME_API UHallNode : public UMazeNodeBase
{
	GENERATED_BODY()
	UHallNode();
public:
	UPROPERTY(BlueprintReadOnly, Category = "Hall Node")
	float HallLength;
	UPROPERTY()
	float TileScale = 400.0f;
	virtual float GetHalfDistanceOfRoom(EMazeOrientation Orientation) override;
	virtual void SpawnMazeObject(UWorld* World, FVector Position, EMazeDirection Direction) override;
};