#pragma once

#include "ProceduralLevelGraphEditor/Node/Data/MazeGraphNodeBase.h"

using FRoutePathMap = TMap<ERouteType, TArray<UMazeGraphNodeBase*>>;

class SRouteOverlay : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SRouteOverlay)
	    : _bShowRoutes(true)
    {}
		SLATE_ATTRIBUTE(TSharedPtr<SGraphEditor>, GraphEditor)
	    SLATE_ATTRIBUTE(FRoutePathMap, PathToDraw)
    	SLATE_ATTRIBUTE(bool, bShowRoutes)
    SLATE_END_ARGS()
	void Construct(const FArguments& InArgs);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
private:
    TAttribute<TSharedPtr<SGraphEditor>> GraphEditor;
    TAttribute<TMap<ERouteType, TArray<UMazeGraphNodeBase*>>> PathToDraw;
	FLinearColor GetRouteColor(ERouteType Type) const;
	float GetRouteOffset(ERouteType Type) const;
	TAttribute<bool> bShowRoutes;
};
