#pragma once

#include "ProceduralLevelGraphEditor/Node/Data/MazeGraphNodeBase.h"

using FRoutePathMap = TMap<ERouteType, TArray<UMazeGraphNodeBase*>>;

class SRouteOverlay : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SRouteOverlay) {}
		SLATE_ATTRIBUTE(TSharedPtr<SGraphEditor>, GraphEditor)
	    SLATE_ATTRIBUTE(FRoutePathMap, PathToDraw)
    SLATE_END_ARGS()
	void Construct(const FArguments& InArgs);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
private:
    TAttribute<TSharedPtr<SGraphEditor>> GraphEditor;
    TAttribute<TMap<ERouteType, TArray<UMazeGraphNodeBase*>>> PathToDraw;
	FLinearColor GetRouteColor(ERouteType Type) const;
	float GetRouteOffset(ERouteType Type) const;
	
};
