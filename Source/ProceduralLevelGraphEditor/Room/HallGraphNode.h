#pragma once

#include "CoreMinimal.h"
#include "MazeGraphNodeBase.h"
#include "HallGraphNode.generated.h"

UCLASS()
class UHallGraphNode : public UMazeGraphNodeBase
{
	GENERATED_BODY()
	UHallGraphNode();

public:
    
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hall", meta = (UIMin = "1", UIMax = "20", ClampMin = "1", ClampMax = "20", MultipleOf = "1"))
	int HallLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hall")
	bool bHorizentalMode = false;
    
};