 #include "MazeGraphSchema.h"
#include "ProceduralLevelGraphEditor/Room/RoomGraphNode.h"
#include "EdGraph/EdGraph.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ToolMenus.h"
#include "GraphEditor.h"
#include "ToolMenus.h"
#include "GraphEditor.h"
#include "Framework/Commands/UIAction.h" // FUIAction için gerekli
#include "ProceduralLevelGraphEditor/Room/HallGraphNode.h"

#define LOCTEXT_NAMESPACE "MazeGraphSchema"

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

FPLGConnectionDrawingPolicy::FPLGConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj)
    : FConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements)
{
    
}

 void FPLGConnectionDrawingPolicy::DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin,
     FConnectionParams& Params)
 {
    Params.AssociatedPin1 = OutputPin;
    Params.AssociatedPin2 = InputPin;
    Params.WireThickness = 20.0f;
    Params.WireColor = FLinearColor::Gray;
 }

 void FPLGConnectionDrawingPolicy::DrawSplineWithArrow(const FVector2D& StartPoint, const FVector2D& EndPoint,
     const FConnectionParams& Params)
 {
    DrawConnection(
        WireLayerID,
        StartPoint,
        EndPoint,
        Params
        );
 }

 void FPLGConnectionDrawingPolicy::DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom,
     const FConnectionParams& Params)
 {
    const FVector2D StartPoint = FGeometryHelper::CenterOf(StartGeom);
    const FVector2D EndPoint = FGeometryHelper::CenterOf(EndGeom);
    DrawSplineWithArrow(StartPoint, EndPoint, Params);
 }


 void FPLGConnectionDrawingPolicy::DrawConnection(int32 LayerId, const FVector2D& Start, const FVector2D& End,
                                                  const FConnectionParams& Params)
 {
    FPinPair Connection(Params.AssociatedPin1, Params.AssociatedPin2);
    if (DrawnConnections.Contains(Connection))
    {
        return;
    }
    DrawnConnections.Add(Connection);
	FSlateDrawElement::MakeDrawSpaceSpline(
			DrawElementsList,
			LayerId,
			Start, FVector2D::ZeroVector,
			End, FVector2D::ZeroVector,
			Params.WireThickness,
			ESlateDrawEffect::None,
			Params.WireColor
		);
 }

 void UMazeGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
    TSharedPtr<FPLGGraphSchemaAction_NewNode> NewNodeAction_Room(new FPLGGraphSchemaAction_NewNode(
        LOCTEXT("NodeCategory", "Procedural"),
        LOCTEXT("NewRoomNode", "Add Room Node"),
        LOCTEXT("NewRoomNodeTooltip", "Adds a room node."), 0));
    NewNodeAction_Room->NodeTemplate = NewObject<URoomGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
    ContextMenuBuilder.AddAction(NewNodeAction_Room);

    TSharedPtr<FPLGGraphSchemaAction_NewNode> NewNodeAction_Hall(new FPLGGraphSchemaAction_NewNode(
        LOCTEXT("NodeCategory", "Procedural"),
        LOCTEXT("NewRoomNode", "Add Hall Node"),
        LOCTEXT("NewRoomNodeTooltip", "Adds a hall node."), 0));
    NewNodeAction_Hall->NodeTemplate = NewObject<UHallGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
    ContextMenuBuilder.AddAction(NewNodeAction_Hall);
}

const FPinConnectionResponse UMazeGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
    
    if (A->GetOwningNode() == B->GetOwningNode())
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameNode", "Cannot connect to the same node."));
    }
    return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, FText::GetEmpty());
}

 class FConnectionDrawingPolicy* UMazeGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID,
     int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect,
     class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
 {
    return new FPLGConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
 }

#undef LOCTEXT_NAMESPACE
