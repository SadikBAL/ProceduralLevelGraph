#include "SMazeGraphNodeBase.h"

#include "SGraphPanel.h"
#include "ProceduralLevelGraphEditor/Room/MazeGraphNodeBase.h"
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

void SMazeGraphNodeBase::MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty)
{
	SGraphNode::MoveTo(NewPosition, NodeFilter, bMarkDirty);
	    UMazeGraphNodeBase* MovedNode = Cast<UMazeGraphNodeBase>(GraphNode);
    if (!MovedNode)
    {
        return;
    }
    TSharedPtr<SGraphPanel> OwnerPanel = GetOwnerPanel();
    if(!OwnerPanel.IsValid()) return;

    constexpr float ConnectionThreshold = 50.0f;
    UEdGraph* Graph = MovedNode->GetGraph();
    for (UEdGraphPin* MovedNodePin : MovedNode->Pins)
    {
        FVector2D MovedNodePinPos = GetPinAbsolutePosition(OwnerPanel,MovedNodePin);
        TArray<UEdGraphPin*> LinkedPins = MovedNodePin->LinkedTo;
        for (UEdGraphPin* OtherPin : LinkedPins)
        {
            FVector2D OtherNodePinPos = GetPinAbsolutePosition(OwnerPanel,OtherPin);
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
                FVector2D MovedNodePinPos = GetPinAbsolutePosition(OwnerPanel,MovedNodePin);;
                FVector2D OtherNodePinPos = GetPinAbsolutePosition(OwnerPanel,OtherNodePin);
                if (FVector2D::Distance(MovedNodePinPos, OtherNodePinPos) < ConnectionThreshold)
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

FVector2D SMazeGraphNodeBase::GetPinAbsolutePosition(const TSharedPtr<SGraphPanel>& GraphPanel, const UEdGraphPin* Pin)
{
    if (!Pin || !GraphPanel.IsValid())
    {
        return FVector2D::ZeroVector;
    }
    if (UEdGraphNode* OwningNode = Pin->GetOwningNode())
    {
        TSharedPtr<SGraphNode> NodeWidget = GraphPanel->GetNodeWidgetFromGuid(OwningNode->NodeGuid);
        if (NodeWidget.IsValid())
        {
            TSharedPtr<SGraphPin> PinWidget = NodeWidget->FindWidgetForPin(const_cast<UEdGraphPin*>(Pin));
            if (PinWidget.IsValid())
            {
                const FGeometry& PinGeometry = PinWidget->GetCachedGeometry();
                const FVector2D LocalCenter = PinGeometry.GetLocalSize() * 0.5f;
                return PinGeometry.LocalToAbsolute(LocalCenter);
            }
        }
    }
    return FVector2D::ZeroVector;
}
#undef LOCTEXT_NAMESPACE
