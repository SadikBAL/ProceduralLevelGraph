#include "SMazeGraphNodeBase.h"

#include "SGraphPanel.h"
#include "ProceduralLevelGraphEditor/Node/Data/MazeGraphNodeBase.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "SGraphPin.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphNode.h"

#define LOCTEXT_NAMESPACE "SMazeGraphNodeBase"

void SMazeGraphNodeBase::GetAllPinWidgets(TArray<TSharedPtr<SGraphPin>>& OutPinWidgets) const
{
    
}

void SMazeGraphNodeBase::MoveTo(const FVector2f& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty)
{
    bOnDrag = FSlateApplication::Get().GetPressedMouseButtons().Contains(EKeys::LeftMouseButton);
    float DirectionX = GraphNode->GetNodePosX() - NewPosition.X;
    float DirectionY = GraphNode->GetNodePosY() - NewPosition.Y;
    if (DirectionX > (GRID_SNAP_SCALE * 0.5))
    {
        DirectionX = GRID_SNAP_SCALE;
    }
    else if (DirectionX < ((GRID_SNAP_SCALE * 0.5) * -1))
    {
        DirectionX = GRID_SNAP_SCALE * -1;
    }
    else
    {
        DirectionX = 0;
    }
    if (DirectionY > (GRID_SNAP_SCALE * 0.5))
    {
        DirectionY = GRID_SNAP_SCALE;
    }
    else if (DirectionY < ((GRID_SNAP_SCALE * 0.5) * -1))
    {
        DirectionY = GRID_SNAP_SCALE * -1;
    }
    else
    {
        DirectionY = 0;   
    }
    FVector2f SnipPosition;
    if (bOnDrag)
    {
        SnipPosition.X = FMath::GridSnap(NewPosition.X, GRID_SNAP_SCALE);
        SnipPosition.Y = FMath::GridSnap(NewPosition.Y, GRID_SNAP_SCALE);
    }
    else
    {
        SnipPosition.X = FMath::GridSnap(GraphNode->GetNodePosX() - DirectionX, GRID_SNAP_SCALE);
        SnipPosition.Y = FMath::GridSnap(GraphNode->GetNodePosY() - DirectionY, GRID_SNAP_SCALE);
    }
   
    SGraphNode::MoveTo(SnipPosition, NodeFilter, bMarkDirty);
    FSlateApplication::Get().Tick();
    UMazeGraphNodeBase* MovedNode = Cast<UMazeGraphNodeBase>(GraphNode);
    if (!MovedNode)
    {
        return;
    }
    const TSharedPtr<SGraphPanel> OwnerPanel = GetOwnerPanel();
    if(!OwnerPanel.IsValid()) return;

    constexpr float ConnectionThreshold = 10.0f;
    UEdGraph* Graph = MovedNode->GetGraph();
    for (UEdGraphPin* MovedNodePin : MovedNode->Pins)
    {
        FVector2D MovedNodePinPos = GetPinPositionInGraphSpace(OwnerPanel,MovedNodePin);
        TArray<UEdGraphPin*> LinkedPins = MovedNodePin->LinkedTo;
        for (UEdGraphPin* OtherPin : LinkedPins)
        {
            FVector2D OtherNodePinPos = GetPinPositionInGraphSpace(OwnerPanel,OtherPin);
            if (FVector2D::Distance(MovedNodePinPos, OtherNodePinPos) > ConnectionThreshold)
            {
                 const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakPinLink", "Break Pin Link"));
                 MovedNodePin->BreakLinkTo(OtherPin);
            }
        }
    }
    for (UEdGraphNode* OtherNode : Graph->Nodes)
    {
        if (MovedNode == OtherNode) continue;

        for (UEdGraphPin* MovedNodePin : MovedNode->Pins)
        {
            for (UEdGraphPin* OtherNodePin : OtherNode->Pins)
            {
                if (MovedNodePin->LinkedTo.Contains(OtherNodePin))
                {
                    continue;
                }
                FVector2D MovedNodePinPos = GetPinPositionInGraphSpace(OwnerPanel,MovedNodePin);;
                FVector2D OtherNodePinPos = GetPinPositionInGraphSpace(OwnerPanel,OtherNodePin);
                if (FVector2D::Distance(MovedNodePinPos, OtherNodePinPos) <= ConnectionThreshold)
                {
                    const UEdGraphSchema* Schema = Graph->GetSchema();
                    if (Schema && Schema->CanCreateConnection(MovedNodePin, OtherNodePin).Response == CONNECT_RESPONSE_MAKE)
                    {
                        const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_CreateConnection", "Create Connection"));
                        MovedNodePin->MakeLinkTo(OtherNodePin);
                    }
                }
            }
        }
    }
    
}

FVector2D SMazeGraphNodeBase::GetPinPositionInGraphSpace(const TSharedPtr<SGraphPanel>& GraphPanel, const UEdGraphPin* Pin)
{
    if (!Pin || !GraphPanel.IsValid())
    {
        return FVector2D::ZeroVector;
    }
    if (const UEdGraphNode* OwningNode = Pin->GetOwningNode())
    {
        TSharedPtr<SGraphNode> NodeWidget = GraphPanel->GetNodeWidgetFromGuid(OwningNode->NodeGuid);
        if (NodeWidget.IsValid())
        {
            TSharedPtr<SGraphPin> PinWidget = NodeWidget->FindWidgetForPin(const_cast<UEdGraphPin*>(Pin));
            if (PinWidget.IsValid())
            {
                const FGeometry& PinGeometry = PinWidget->GetTickSpaceGeometry();
                const FVector2D LocalCenter = PinGeometry.GetLocalSize() * 0.5f;
                const FVector2D AbsolutePosition = PinGeometry.LocalToAbsolute(LocalCenter);
                const FGeometry& PanelGeometry = GraphPanel->GetTickSpaceGeometry();
                FVector2D LocalPosition = PanelGeometry.AbsoluteToLocal(AbsolutePosition);
                FVector2D ViewOffset = GraphPanel->GetViewOffset();
                float Zoom = GraphPanel->GetZoomAmount();
                return (LocalPosition - ViewOffset) / Zoom;
            }
        }
    }
    return FVector2D::ZeroVector;
}

FOptionalSize SMazeGraphNodeBase::GetNodeHeight() const
{
    return EMPTY_SIZE;
}

FOptionalSize SMazeGraphNodeBase::GetNodeWidth() const
{
    return EMPTY_SIZE;
}

#undef LOCTEXT_NAMESPACE
