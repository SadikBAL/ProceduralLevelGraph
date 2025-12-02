#pragma once

#include "CoreMinimal.h"
#include "MazeGraphNodeBase.h"
#include "RouterGraphNode.generated.h"

class ARoomLevelInstance;

UCLASS()
class URouterGraphNode : public UMazeGraphNodeBase
{
	GENERATED_BODY()
	URouterGraphNode();
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
	void OnTileBlueprintsChanged();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Router", meta = (UIMin = "1", UIMax = "100", ClampMin = "1", ClampMax = "100", MultipleOf = "1"))
	int RoomWidth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Router", meta = (UIMin = "1", UIMax = "100", ClampMin = "1", ClampMax = "100", MultipleOf = "1"))
	int RoomHeight;
	UPROPERTY(VisibleAnywhere, Category = "Router")
	TSubclassOf<ARoomLevelInstance> RouterLevelInstanceRef;
};