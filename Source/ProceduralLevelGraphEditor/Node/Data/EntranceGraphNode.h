#pragma once

#include "CoreMinimal.h"
#include "RoomGraphNode.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/RoomLevelInstance.h"
#include "EntranceGraphNode.generated.h"

UCLASS()
class UEntranceGraphNode : public URoomGraphNode
{
	GENERATED_BODY()
	UEntranceGraphNode();
public:
    
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetNodeName() const override;
	virtual FSlateColor GetNodeBackgroundColor() const override;
};