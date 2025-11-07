#pragma once

#include "CoreMinimal.h"
#include "MazeNodeBase.h"
#include "ProceduralLevelGraphRuntime/MazeTileActor.h"
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
	TArray<TSubclassOf<AMazeTileActor>> RoomBlueprints;
	UPROPERTY(BlueprintReadOnly, Category = "Room Node")
	TArray<TSubclassOf<AMazeTileActor>> HallBlueprints;
	virtual float GetHalfDistanceOfRoom(EMazeOrientation Orientation) override;
	virtual AActor* SpawnMazeObject(UWorld* World, FVector Position) override;
	AActor* SpawnMazeExtraPart(UWorld* World, FVector Position);
};