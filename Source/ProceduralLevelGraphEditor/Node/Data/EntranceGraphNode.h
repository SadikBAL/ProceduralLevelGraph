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
	void OnTileBlueprintsChanged();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room", meta = (UIMin = "1", UIMax = "10", ClampMin = "1", ClampMax = "10", MultipleOf = "1"))
	int RoomWidth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room", meta = (UIMin = "1", UIMax = "10", ClampMin = "1", ClampMax = "10", MultipleOf = "1"))
	int RoomHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	FVector RoomPosition;
	UPROPERTY(VisibleAnywhere, Category = "Room")
	TSubclassOf<ARoomLevelInstance> EntranceLevelInstanceRef;
};