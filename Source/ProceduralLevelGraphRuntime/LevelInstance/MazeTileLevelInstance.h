#pragma once

#include "Engine/LevelStreamingDynamic.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#include "ProceduralLevelGraphRuntime/Node/MazeNodeBase.h"

#include "MazeTileLevelInstance.generated.h"
class ULevelInstanceManagerComponent;

UCLASS(Abstract, HideDropdown, PrioritizeCategories = "Level")
class PROCEDURALLEVELGRAPHRUNTIME_API AMazeTileLevelInstance : public ALevelInstance
{
	GENERATED_BODY()
public:
	AMazeTileLevelInstance();
	virtual void OnLevelInstanceLoaded() override;
	virtual bool IsLoadingEnabled() const override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "MazeTile")
	FString GetNetID();
	
#if WITH_EDITOR
	virtual  void PreSave(FObjectPreSaveContext SaveContext) override;
	void LoadMapData(TArray<AActor*>& IgnoreList);
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	DECLARE_MULTICAST_DELEGATE(FOnMazeTileLevelInstanceUpdated);
	FOnMazeTileLevelInstanceUpdated OnMazeTileLevelInstanceUpdated;
#endif
	
	UFUNCTION(BlueprintCallable)
	void SetNodeData(UMazeNodeBase* BaseNode);
	
	UFUNCTION(BlueprintCallable)
	void SetNodeDataFromHall(UMazeNodeBase* BaseNode, EMazeDirection DoorLocation);
	
	UPROPERTY(VisibleAnywhere, Category="Level")
	FMazeTileData TileData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	FString LevelName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Level", meta = (UIMin = "1", UIMax = "1000", ClampMin = "1", ClampMax = "1000", MultipleOf = "1"))
	int Width;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Level", meta = (UIMin = "1", UIMax = "1000", ClampMin = "1", ClampMax = "1000", MultipleOf = "1"))
	int Height;

	
	UPROPERTY()
	FMazeTileData NodeData;
	UPROPERTY()
	ULevelInstanceManagerComponent* LevelInstanceManager;
	UFUNCTION()
	void ApplyMazeTileData();
	UFUNCTION()
	void RecreateReplicatedActors();
	UFUNCTION()
	APassagePoint* FindOwnerPasssagePoint(ULevel* Level, AActor* Actor);
	UFUNCTION()
	void GroupActors();
	UFUNCTION()
	void OnEditorLevelLoadedAndShown();
	UFUNCTION(BlueprintCallable)
	void LoadLevelAsync();
	UFUNCTION()
	void OnLevelLoadedAndShown();
	UPROPERTY()
	bool bLevelLoadedAndShown = false;
	UPROPERTY()
	ULevelStreamingDynamic* LevelStreamingDynamic = nullptr;
	UPROPERTY(EditAnywhere, Category = "Level")
	TArray<FDoorData> DoorData;
	UPROPERTY()
	FGameplayTagContainer RoomTags;

};
