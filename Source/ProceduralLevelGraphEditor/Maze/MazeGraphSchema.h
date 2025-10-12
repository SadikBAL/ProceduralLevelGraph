#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "ScopedTransaction.h"
#include "MazeGraphSchema.generated.h"

class UEdGraphNode_PLGBase;

USTRUCT()
struct FPLGGraphSchemaAction_NewNode : public FEdGraphSchemaAction
{
    GENERATED_BODY();

    FPLGGraphSchemaAction_NewNode() : NodeTemplate(nullptr) {}

    FPLGGraphSchemaAction_NewNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
        : FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr) {}
    
    virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

    UPROPERTY()
    TObjectPtr<UEdGraphNode> NodeTemplate;
};


UCLASS()
class UMazeGraphSchema : public UEdGraphSchema
{
    GENERATED_BODY()
public:
    virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
};