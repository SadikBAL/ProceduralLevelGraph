#pragma once
#include "Node/MazeNodeBase.h"
#include "MazeTileActor.generated.h"
UCLASS()
class AMazeTileActor : public AActor
{
	GENERATED_BODY()
	AMazeTileActor();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(BlueprintReadOnly)
	float Width;
	
	UPROPERTY(BlueprintReadOnly)
	float Height;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMazeNodeBase> NodeData;

	UFUNCTION(BlueprintCallable, Category = "Tile")
	void SetNodeData(UMazeNodeBase* BaseNode);
	UFUNCTION()
	void UpdateMeshPartVisibilities(TArray<FName> SearchedTags, bool bVisibility);
	UFUNCTION()
	TArray<FName> GetMeshPartNames(EMazeDirection LocalDirection, int LocalRotation);	
};
