#pragma once

#include "CoreMinimal.h"
#include "RoomGraphNodeBase.h"
#include "RoomGraphNode.generated.h"

UCLASS()
class URoomGraphNode : public URoomGraphNodeBase
{
    GENERATED_BODY()

public:
    
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void AllocateDefaultPins() override;
    
};