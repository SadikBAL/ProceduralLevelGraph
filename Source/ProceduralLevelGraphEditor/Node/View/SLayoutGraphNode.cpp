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
	if (const ULayoutGraphNode* ImageNode = Cast<ULayoutGraphNode>(GraphNode))
	{
		if (ImageNode->BackgroundImage)
		{
			const FVector2D DrawSize(ImageNode->NodeWidth, ImageNode->NodeHeight);
			const FSlateImageBrush TempBrush(ImageNode->BackgroundImage, DrawSize);
			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(),
				&TempBrush,
				ESlateDrawEffect::None,
				FLinearColor(1.0f, 1.0f, 1.0f, ImageNode->ImageOpacity)
			);
		}
	}
	return SGraphNodeComment::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                  bParentEnabled);
}
