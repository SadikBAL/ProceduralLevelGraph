#pragma once

#include "CoreMinimal.h"
#include "MazeGraphNodeBase.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/RoomLevelInstance.h"
#include "EntranceGraphNode.generated.h"

UCLASS()
class UEntranceGraphNode : public UMazeGraphNodeBase
{
	GENERATED_BODY()
	UEntranceGraphNode();
public:
    
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
    virtual bool CanUserDeleteNode() const override;
	virtual bool CanUserCopyNode() const override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	
	void OnTileBlueprintsChanged();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room", meta = (UIMin = "1", UIMax = "100", ClampMin = "1", ClampMax = "100", MultipleOf = "1"))
	int RoomWidth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room", meta = (UIMin = "1", UIMax = "100", ClampMin = "1", ClampMax = "100", MultipleOf = "1"))
	int RoomHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	FVector RoomPosition;
	UPROPERTY(EditAnywhere, Category = "Room")
	TSubclassOf<ARoomLevelInstance> EntranceLevelInstanceRef;
};