#pragma once

#include "CoreMinimal.h"
#include "EdGraphNode_PLGRoom.generated.h"

UCLASS()
class UEdGraphNode_PLGRoom : public UEdGraphNode
{
    GENERATED_BODY()

public:
    
    FString NodeTitle = "Room";
    
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void AllocateDefaultPins() override;
    virtual bool CanUserDeleteNode() const override;
    virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
    
};