#include "SFloorGraphNode.h"

void SFloorGraphNode::Construct(const FArguments& InArgs, UFloorGraphNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

float SFloorGraphNode::GetLayerOffset() const
{
	return -1.0f;
}

TSharedRef<SWidget> SFloorGraphNode::CreateNodeContentArea()
{
	UFloorGraphNode* BoundaryNode = Cast<UFloorGraphNode>(GraphNode);

	return SNew(SBox)
		.WidthOverride(this, &SFloorGraphNode::GetNodeHeight)
		.HeightOverride(this, &SFloorGraphNode::GetNodeHeight)
		[
			SNew(SBorder)
			.BorderImage(TAttribute<const FSlateBrush*>::CreateLambda([this]()->const FSlateBrush* {
				if(UFloorGraphNode* Node = Cast<UFloorGraphNode>(GraphNode))
				{
					return &Node->BackgroundBrush;
				}
				return FAppStyle::GetBrush("NoBrush");
			}))
			.BorderBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f)) // Opaklık için
		];
}
FOptionalSize SFloorGraphNode::GetNodeWidth() const
{
	if (SFloorGraphNode* Node = Cast<SFloorGraphNode>(GraphNode))
	{
		return Node->NodeWidth;
	}
	return 1000.0f;
}

FOptionalSize SFloorGraphNode::GetNodeHeight() const
{
	if (SFloorGraphNode* Node = Cast<SFloorGraphNode>(GraphNode))
	{
		return Node->NodeHeight;
	}
	return 1000.0f;
}