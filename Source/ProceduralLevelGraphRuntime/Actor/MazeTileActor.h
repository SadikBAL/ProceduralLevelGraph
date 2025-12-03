#pragma once
#include "ProceduralLevelGraphRuntime/Node/MazeNodeBase.h"
#include "MazeTileActor.generated.h"
UCLASS(Abstract, HideDropdown)
class AMazeTileActor : public AActor
{
	GENERATED_BODY()
public:
	AMazeTileActor();

	UFUNCTION(BlueprintCallable, Category = "Tile")
	void SetNodeData(UMazeNodeBase* BaseNode);
	UFUNCTION()
	void UpdateMeshPartVisibilities(TArray<FName> SearchedTags, bool bVisibility);
	UFUNCTION()
	TArray<FName> GetMeshPartNames(EMazeDirection LocalDirection, int LocalRotation);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	TObjectPtr<USceneComponent> SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile", meta = (UIMin = "1", UIMax = "30", ClampMin = "1", ClampMax = "30", MultipleOf = "1"))
	int Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile", meta = (UIMin = "1", UIMax = "30", ClampMin = "1", ClampMax = "30", MultipleOf = "1"))
	int Height;
	
};
