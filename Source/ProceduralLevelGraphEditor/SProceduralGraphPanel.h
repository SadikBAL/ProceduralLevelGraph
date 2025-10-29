#pragma once
class SProceduralGraphPanel : public SGraphEditor

{
public:
	SProceduralGraphPanel()
		: SGraphEditor()
	{
	}
protected:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
};
