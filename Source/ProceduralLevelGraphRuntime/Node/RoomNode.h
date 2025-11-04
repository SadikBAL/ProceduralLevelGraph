#pragma once

#include "CoreMinimal.h"
#include "MazeNodeBase.h"
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

	virtual float GetHalfDistanceOfRoom(EMazeOrientation Orientation) override;

};