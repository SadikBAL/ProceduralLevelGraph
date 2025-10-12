#include "EdGraphSchema_ProceduralLevelGraph.h"
#include "EdGraphNode_PLGRoom.h"
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

void UEdGraphSchema_ProceduralLevelGraph::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
    TSharedPtr<FPLGGraphSchemaAction_NewNode> NewNodeAction_Room(new FPLGGraphSchemaAction_NewNode(
        LOCTEXT("NodeCategory", "Procedural"),
        LOCTEXT("NewRoomNode", "Add Room Node"),
        LOCTEXT("NewRoomNodeTooltip", "Adds a room node."), 0));
    NewNodeAction_Room->NodeTemplate = NewObject<UEdGraphNode_PLGRoom>(ContextMenuBuilder.OwnerOfTemporaries);
    ContextMenuBuilder.AddAction(NewNodeAction_Room);
}

const FPinConnectionResponse UEdGraphSchema_ProceduralLevelGraph::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
    // Aynı node'a bağlanmayı engelle
    if (A->GetOwningNode() == B->GetOwningNode())
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameNode", "Cannot connect to the same node."));
    }

    // Input'u input'a, output'u output'a bağlamayı engelle
    if (A->Direction == B->Direction)
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameDirection", "Cannot connect pins with the same direction."));
    }

    return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, FText::GetEmpty());
}

#undef LOCTEXT_NAMESPACE
