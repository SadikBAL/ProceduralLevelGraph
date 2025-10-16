#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MazeNodeBase.generated.h"

UCLASS(BlueprintType)
class PROCEDURALLEVELGRAPHRUNTIME_API UMazeNodeBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Maze Node")
	TArray<UMazeNodeBase*> ConnectedNodes;
	
	UPROPERTY(BlueprintReadOnly, Category = "Maze Node")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(BlueprintReadOnly, Category = "Maze Node")
	FGuid NodeGuid;
};