#pragma once

#include "CoreMinimal.h"
#include "MazeGraphNodeBase.h"
#include "RoomGraphNode.generated.h"

UCLASS()
class URoomGraphNode : public UMazeGraphNodeBase
{
    GENERATED_BODY()
    URoomGraphNode();
public:
    
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void AllocateDefaultPins() override;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (UIMin = "1", UIMax = "10", ClampMin = "1", ClampMax = "10", MultipleOf = "1"))
    int RoomWith;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (UIMin = "1", UIMax = "10", ClampMin = "1", ClampMax = "10", MultipleOf = "1"))
    int RoomHeight;
};