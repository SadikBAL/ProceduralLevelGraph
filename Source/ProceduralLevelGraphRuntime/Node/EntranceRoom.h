#pragma once

#include "CoreMinimal.h"
#include "MazeNodeBase.h"
#include "EntranceRoom.generated.h"

UCLASS(BlueprintType)
class PROCEDURALLEVELGRAPHRUNTIME_API UEntranceNode : public UMazeNodeBase
{
	GENERATED_BODY()
	UEntranceNode();
public:
	UPROPERTY(BlueprintReadOnly, Category = "Entrance Node")
	float RoomWidth;
	UPROPERTY(BlueprintReadOnly, Category = "Entrance Node")
	float RoomHeight;
	UPROPERTY(BlueprintReadOnly, Category = "Entrance Node")
	FVector RoomPosition;

	virtual float GetHalfDistanceOfRoom(EMazeOrientation Orientation) override;

};