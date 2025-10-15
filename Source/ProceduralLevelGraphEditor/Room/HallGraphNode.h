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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hall", meta = (UIMin = "2.0", UIMax = "2000.0", ClampMin = "100.0", ClampMax = "2000.0", MultipleOf = "2.0"))
	float HallLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hall")
	bool bHorizentalMode = false;
    
};