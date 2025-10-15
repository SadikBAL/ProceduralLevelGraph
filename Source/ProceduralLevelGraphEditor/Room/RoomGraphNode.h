#pragma once

#include "CoreMinimal.h"
#include "MazeGraphNodeBase.h"
#include "RoomGraphNode.generated.h"

UCLASS()
class URoomGraphNode : public UMazeGraphNodeBase
{
    GENERATED_BODY()

public:
    
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void AllocateDefaultPins() override;
    
};