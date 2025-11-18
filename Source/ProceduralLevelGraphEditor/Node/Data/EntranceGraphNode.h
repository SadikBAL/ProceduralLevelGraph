#pragma once

#include "CoreMinimal.h"
#include "MazeGraphNodeBase.h"
#include "EntranceGraphNode.generated.h"

UCLASS()
class UEntranceGraphNode : public UMazeGraphNodeBase
{
	GENERATED_BODY()
	UEntranceGraphNode();
public:
    
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
    virtual bool CanUserDeleteNode() const override;
	virtual bool CanUserCopyNode() const override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (UIMin = "1", UIMax = "10", ClampMin = "1", ClampMax = "10", MultipleOf = "1"))
	int RoomWith;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (UIMin = "1", UIMax = "10", ClampMin = "1", ClampMax = "10", MultipleOf = "1"))
	int RoomHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	FVector RoomPosition;
};