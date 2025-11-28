#pragma once

#include "LevelInstance/LevelInstanceActor.h"
#include "ProceduralLevelGraphRuntime/Node/MazeNodeBase.h"

#include "MazeTileLevelInstance.generated.h"
UCLASS(Abstract, HideDropdown)
class AMazeTileLevelInstance : public ALevelInstance
{
	GENERATED_BODY()
public:
	AMazeTileLevelInstance();
	virtual void OnLevelInstanceLoaded() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile", meta = (UIMin = "1", UIMax = "30", ClampMin = "1", ClampMax = "30", MultipleOf = "1"))
	int Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile", meta = (UIMin = "1", UIMax = "30", ClampMin = "1", ClampMax = "30", MultipleOf = "1"))
	int Height;
	
	UFUNCTION(BlueprintCallable, Category = "Tile")
	void SetNodeData(UMazeNodeBase* BaseNode);
	UFUNCTION()
	void UpdateMeshPartVisibilities(TArray<FName> SearchedTags, bool bVisibility);
	UFUNCTION()
	TArray<FName> GetMeshPartNames(EMazeDirection LocalDirection, int LocalRotation);
private:
	UPROPERTY()
	UMazeNodeBase* NodeData;
};
