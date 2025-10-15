#pragma once

#include "CoreMinimal.h"
#include "RoomGraphNodeBase.h"
#include "HallGraphNode.generated.h"

UCLASS()
class UHallGraphNode : public URoomGraphNodeBase
{
	GENERATED_BODY()

public:
    
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
    
};