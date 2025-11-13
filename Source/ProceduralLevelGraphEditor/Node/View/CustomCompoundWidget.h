#pragma once

#include "ProceduralLevelGraphEditor/Node/Data/MazeGraphNodeBase.h"

class SRouteOverlay : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SRouteOverlay) {}
    SLATE_ATTRIBUTE(TSharedPtr<SGraphEditor>, GraphEditor)
    SLATE_ATTRIBUTE(TArray<UMazeGraphNodeBase*>, PathToDraw)
    SLATE_END_ARGS()
	void Construct(const FArguments& InArgs);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
private:
    TAttribute<TSharedPtr<SGraphEditor>> GraphEditor;
    TAttribute<TArray<UMazeGraphNodeBase*>> PathToDraw;
	
};
