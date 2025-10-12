 #include "MazeGraphSchema.h"
#include "ProceduralLevelGraphEditor/Room/RoomGraphNode.h"
#include "EdGraph/EdGraph.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ToolMenus.h"
#include "GraphEditor.h"
#include "ToolMenus.h"
#include "GraphEditor.h"
#include "Framework/Commands/UIAction.h" // FUIAction için gerekli

#define LOCTEXT_NAMESPACE "EdGraphSchema_ProceduralLevelGraph"

UEdGraphNode* FPLGGraphSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
    UEdGraphNode* ResultNode = nullptr;

    if (NodeTemplate)
    {
        const FScopedTransaction Transaction(LOCTEXT("NewNodeTransaction", "New Node"));
        ParentGraph->Modify();
        if (FromPin)
        {
            FromPin->Modify();
        }

        NodeTemplate->Rename(nullptr, ParentGraph);
        ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

        NodeTemplate->CreateNewGuid();
        NodeTemplate->PostPlacedNewNode();
        NodeTemplate->AllocateDefaultPins();
        NodeTemplate->bCanRenameNode = true;

        NodeTemplate->NodePosX = Location.X;
        NodeTemplate->NodePosY = Location.Y;

        ResultNode = NodeTemplate;
    }

    return ResultNode;
}

void UMazeGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
    TSharedPtr<FPLGGraphSchemaAction_NewNode> NewNodeAction_Room(new FPLGGraphSchemaAction_NewNode(
        LOCTEXT("NodeCategory", "Procedural"),
        LOCTEXT("NewRoomNode", "Add Room Node"),
        LOCTEXT("NewRoomNodeTooltip", "Adds a room node."), 0));
    NewNodeAction_Room->NodeTemplate = NewObject<URoomGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
    ContextMenuBuilder.AddAction(NewNodeAction_Room);
}

const FPinConnectionResponse UMazeGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
    
    return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, FText::GetEmpty());
}

 class FConnectionDrawingPolicy* UMazeGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID,
     int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect,
     class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
 {
     return Super::CreateConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect,
                                                 InDrawElements, InGraphObj);
 }

#undef LOCTEXT_NAMESPACE
