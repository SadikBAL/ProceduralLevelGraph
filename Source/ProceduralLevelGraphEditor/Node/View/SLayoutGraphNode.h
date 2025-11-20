#pragma once

#include "CoreMinimal.h"
#include "SGraphNodeComment.h"
#include "SLayoutGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/LayoutGraphNode.h"

class SLayoutGraphNode : public SGraphNodeComment
{
	
public:
	SLATE_BEGIN_ARGS(SLayoutGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, ULayoutGraphNode* InNode);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
};
