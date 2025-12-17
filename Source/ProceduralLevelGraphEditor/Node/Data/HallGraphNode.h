#pragma once

#include "CoreMinimal.h"
#include "MazeGraphNodeBase.h"
#include "ProceduralLevelGraphRuntime/Data/HallData.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/HallLevelInstance.h"
#include "HallGraphNode.generated.h"

UCLASS()
class UHallGraphNode : public UMazeGraphNodeBase
{
	GENERATED_BODY()
	UHallGraphNode();

public:
    
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
	virtual EMazePinType GetPinType() override;
	
	void OnTileBlueprintsChanged();
	void UpdateHallLength(int Value);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hall")
	int RoomWidth;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hall", meta = (UIMin = "1", UIMax = "100", ClampMin = "1", ClampMax = "100", MultipleOf = "1"))
	int HallLength;
	UPROPERTY(EditDefaultsOnly, Category = "Hall")
	TObjectPtr<UHallData> HallData;
};
