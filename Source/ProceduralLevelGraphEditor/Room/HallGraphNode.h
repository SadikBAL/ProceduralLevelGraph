#pragma once

#include "CoreMinimal.h"
#include "MazeGraphNodeBase.h"
#include "HallGraphNode.generated.h"

UCLASS()
class UHallGraphNode : public UMazeGraphNodeBase
{
	GENERATED_BODY()

public:
    
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
    
};