#pragma once

#include "LevelInstance/LevelInstanceActor.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#include "ProceduralLevelGraphRuntime/Node/MazeNodeBase.h"

#include "MazeTileLevelInstance.generated.h"
class ULevelInstanceManagerComponent;

UCLASS(Abstract, HideDropdown)
class PROCEDURALLEVELGRAPHRUNTIME_API AMazeTileLevelInstance : public ALevelInstance
{
	GENERATED_BODY()
public:
	AMazeTileLevelInstance();
	virtual void OnLevelInstanceLoaded() override;
	virtual bool IsLoadingEnabled() const override;
	
	
#if WITH_EDITOR
	virtual  void PreSave(FObjectPreSaveContext SaveContext) override;
	void LoadMapData(TArray<AActor*>& IgnoreList);
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UFUNCTION(BlueprintCallable)
	void SetNodeData(UMazeNodeBase* BaseNode);
	UFUNCTION()
	void UpdateMeshPartVisibilities(TArray<FName> SearchedTags, bool bVisibility);
	UFUNCTION()
	TArray<FName> GetMeshPartNames(EMazeDirection LocalDirection, int LocalRotation);
	
	UPROPERTY(VisibleAnywhere, Category="Level")
	FString LevelName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level", meta = (UIMin = "1", UIMax = "100", ClampMin = "1", ClampMax = "100", MultipleOf = "1"))
	int Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level", meta = (UIMin = "1", UIMax = "100", ClampMin = "1", ClampMax = "100", MultipleOf = "1"))
	int Height;

	
	UPROPERTY()
	FMazeTileData NodeData;
	UPROPERTY()
	ULevelInstanceManagerComponent* LevelInstanceManager;
	UFUNCTION()
	void ApplyMazeTileData();
	UFUNCTION()
	void GroupActors();
	UFUNCTION()
	void OnEditorLevelLoadedAndShown();
	UFUNCTION()
	void LoadLevelAsync();
	UFUNCTION()
	void OnLevelLoadedAndShown();
	UPROPERTY()
	bool bLevelLoadedAndShown = false;
	UPROPERTY()
	ULevelStreamingDynamic* LevelStreamingDynamic = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Level")
	TArray<FDoorData> DoorDatas;
};
