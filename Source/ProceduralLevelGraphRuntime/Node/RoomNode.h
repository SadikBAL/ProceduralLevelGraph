#pragma once

#include "CoreMinimal.h"
#include "MazeNodeBase.h"
#include "ProceduralLevelGraphRuntime/Actor/HallActor.h"
#include "ProceduralLevelGraphRuntime/Actor/RoomActor.h"
#include "RoomNode.generated.h"

UCLASS(BlueprintType)
class PROCEDURALLEVELGRAPHRUNTIME_API URoomNode : public UMazeNodeBase
{
	GENERATED_BODY()
	URoomNode();
public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Room Node")
	float RoomWidth;
	UPROPERTY(BlueprintReadOnly, Category = "Room Node")
	float RoomHeight;
	
	UPROPERTY(BlueprintReadOnly, Category = "Room Node")
	TArray<TSubclassOf<ARoomActor>> RoomBlueprints;
	UPROPERTY(BlueprintReadOnly, Category = "Room Node")
	TArray<TSubclassOf<AHallActor>> HallBlueprints;
	
	virtual float GetHalfDistanceOfRoom(EMazeOrientation Orientation) override;
	virtual void SpawnMazeObject(UWorld* World, FVector Position, EMazeDirection Direction) override;
};