#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MazeNodeBase.generated.h"
UENUM(BlueprintType)
enum class EMazeDirection : uint8
{
	Up,
	Down,
	Left,
	Right,
	None
};
UENUM(BlueprintType)
enum class EMazeOrientation : uint8
{
	Vertical,
	Horizontal
};
UCLASS(BlueprintType)
class PROCEDURALLEVELGRAPHRUNTIME_API UMazeNodeBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Maze Node")
	FGameplayTagContainer GameplayTags;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Node")
	FGuid NodeGuid;

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

	UPROPERTY()
	TObjectPtr<AActor> MazeObject;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> ActorToSpawnClass;

	UFUNCTION(BlueprintCallable)
	virtual  AActor* SpawnMazeObject(UObject* WorldContextObject, FVector Position);
	UFUNCTION(BlueprintCallable)
	virtual FVector GetEdgePosition(EMazeDirection Direction);
	UFUNCTION(BlueprintCallable)
	virtual float GetHalfDistanceOfRoom(EMazeOrientation Orientation);
};