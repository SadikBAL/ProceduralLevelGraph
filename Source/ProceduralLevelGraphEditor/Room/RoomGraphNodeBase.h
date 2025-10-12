#pragma once

#include "CoreMinimal.h"
#include "RoomGraphNodeBase.generated.h"

UCLASS()
class URoomGraphNodeBase : public UEdGraphNode
{
	GENERATED_BODY()

public:
    
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
};
