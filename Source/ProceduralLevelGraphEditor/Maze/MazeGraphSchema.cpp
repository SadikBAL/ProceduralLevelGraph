 #include "MazeGraphSchema.h"
#include "EdGraph/EdGraph.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ToolMenus.h"
#include "GraphEditor.h"
#include "Framework/Commands/UIAction.h"
#include "ProceduralLevelGraphEditor/Node/Data/EntranceGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/FloorGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/HallGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/RoomGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/RouterGraphNode.h"

#define LOCTEXT_NAMESPACE "MazeGraphSchema"

UEdGraphNode* FPLGGraphSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode)
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

 void FPLGConnectionDrawingPolicy::DrawSplineWithArrow(const FVector2f& StartPoint, const FVector2f& EndPoint,
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
    const FVector2f StartPoint = FGeometryHelper::CenterOf(StartGeom);
    const FVector2f EndPoint = FGeometryHelper::CenterOf(EndGeom);
    DrawSplineWithArrow(StartPoint, EndPoint, Params);
 }


 void FPLGConnectionDrawingPolicy::DrawConnection(int32 LayerId, const FVector2f& Start, const FVector2f& End,
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

    TSharedPtr<FPLGGraphSchemaAction_NewNode> NewNodeAction_Router(new FPLGGraphSchemaAction_NewNode(
       LOCTEXT("NodeCategory", "Procedural"),
       LOCTEXT("NewRoomNode", "Add Router Node"),
       LOCTEXT("NewRoomNodeTooltip", "Adds a router node."), 0));
    NewNodeAction_Router->NodeTemplate = NewObject<URouterGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
    ContextMenuBuilder.AddAction(NewNodeAction_Router);
    /* Its generetad on init one time. it coudnt doublcate or delete.
    TSharedPtr<FPLGGraphSchemaAction_NewNode> NewNodeAction_Entrance(new FPLGGraphSchemaAction_NewNode(
       LOCTEXT("NodeCategory", "Procedural"),
       LOCTEXT("NewRoomNode", "Add Entrance Node"),
       LOCTEXT("NewRoomNodeTooltip", "Adds a entrance node."), 0));
    NewNodeAction_Entrance->NodeTemplate = NewObject<UEntranceGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
    ContextMenuBuilder.AddAction(NewNodeAction_Entrance);
    */
    TSharedPtr<FPLGGraphSchemaAction_NewNode> NewNodeAction_Boundary(new FPLGGraphSchemaAction_NewNode(
       LOCTEXT("NodeCategory", "Procedural"),
       LOCTEXT("NewFloorNode", "Add New Floor Node"),
       LOCTEXT("NewBoundaryNodeTooltip", "Adds a node to act as a boundary for rooms."), 0));
    NewNodeAction_Boundary->NodeTemplate = NewObject<UFloorGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
    ContextMenuBuilder.AddAction(NewNodeAction_Boundary);
}

 FEnumPinType UMazeGraphSchema::GetPinType(const UEdGraphPin* A) const
 {
    if (A->GetName().Compare(FString("Up")) == 0)
    {
        return FEnumPinType::Up;
    }
    else if (A->GetName().Compare(FString("Down")) == 0)
    {
        return FEnumPinType::Down;
    }
    else if (A->GetName().Compare(FString("Left")) == 0)
    {
        return FEnumPinType::Left;
    }
    else if (A->GetName().Compare(FString("Right")) == 0)
    {
        return FEnumPinType::Right;
    }
    return FEnumPinType::None;
 }

const FPinConnectionResponse UMazeGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
   
    if (A->GetOwningNode() == B->GetOwningNode())
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameNode", "Cannot connect to the same node."));
    }
    else if (A->LinkedTo.Num() > 0 || B->LinkedTo.Num() > 0)
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameNode", "Cannot connect to the same node."));
    }
    if (A->bHidden || B->bHidden)
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameNode", "Cannot connect to the same node."));
    }
    
    FEnumPinType AType = GetPinType(A);
    FEnumPinType BType = GetPinType(B);
    if (UHallGraphNode* HallNodeA = Cast<UHallGraphNode>(A->GetOwningNode()))
    {
        if (HallNodeA->RoomRotation == 0 || HallNodeA->RoomRotation == 180)
        {
            if (AType == FEnumPinType::Left || AType == FEnumPinType::Right)
            {
                return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameNode", "Cannot connect to the same node."));
            }
        }
        else
        {
            if (AType == FEnumPinType::Up || AType == FEnumPinType::Down)
            {
                return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameNode", "Cannot connect to the same node."));
            }
        }
    }
    if (UHallGraphNode* HallNodeB = Cast<UHallGraphNode>(A->GetOwningNode()))
    {
        if (HallNodeB->RoomRotation == 0 || HallNodeB->RoomRotation == 180)
        {
            if (BType == FEnumPinType::Left || BType == FEnumPinType::Right)
            {
                return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameNode", "Cannot connect to the same node."));
            }
        }
        else
        {
            if (BType == FEnumPinType::Up || BType == FEnumPinType::Down)
            {
                return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameNode", "Cannot connect to the same node."));
            }
        }
    }
    if (AType == FEnumPinType::None || BType == FEnumPinType::None)
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameNode", "Cannot connect to the same node."));
    }
    if ((AType == FEnumPinType::Down  && BType != FEnumPinType::Up)    ||
        (AType == FEnumPinType::Up    && BType != FEnumPinType::Down)  ||
        (AType == FEnumPinType::Left  && BType != FEnumPinType::Right) ||
        (AType == FEnumPinType::Right && BType != FEnumPinType::Left)  )
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameNode", "Cannot connect to the same node."));
    }
    else
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, FText::GetEmpty());
    }

}

 class FConnectionDrawingPolicy* UMazeGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID,
     int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect,
     class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
 {
    return new FPLGConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
 }

#undef LOCTEXT_NAMESPACE
