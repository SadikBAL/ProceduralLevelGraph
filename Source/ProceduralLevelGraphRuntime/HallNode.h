#pragma once

#include "CoreMinimal.h"
#include "MazeNodeBase.h"
#include "HallNode.generated.h"

UCLASS(BlueprintType)
class PROCEDURALLEVELGRAPHRUNTIME_API UHallNode : public UMazeNodeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Hall Node")
	float HallLength;

	UPROPERTY(BlueprintReadOnly, Category = "Hall Node")
	bool bIsHorizontal;
};