#pragma once

#include "CoreMinimal.h"
#include "RoomGraphNode.generated.h"

UCLASS()
class URoomGraphNode : public UEdGraphNode
{
    GENERATED_BODY()

public:
    
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void AllocateDefaultPins() override;
    
};