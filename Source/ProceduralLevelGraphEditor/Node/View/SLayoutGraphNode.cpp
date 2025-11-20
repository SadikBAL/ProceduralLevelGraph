#include "SLayoutGraphNode.h"

#include "ProceduralLevelGraphEditor/Node/Data/LayoutGraphNode.h"

void SLayoutGraphNode::Construct(const FArguments& InArgs, ULayoutGraphNode* InNode)
{
	SGraphNodeComment::Construct(SGraphNodeComment::FArguments(), InNode);
}

int32 SLayoutGraphNode::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (ULayoutGraphNode* ImageNode = Cast<ULayoutGraphNode>(GraphNode))
	{
		if (ImageNode->BackgroundImage)
		{
			FVector2D DrawSize = AllottedGeometry.GetLocalSize();
			
			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(),
				new FSlateImageBrush(ImageNode->BackgroundImage, DrawSize, FLinearColor(1.0f, 1.0f, 1.0f, ImageNode->ImageOpacity))
			);
		}
	}
	return SGraphNodeComment::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                  bParentEnabled);
}
