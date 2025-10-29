#include "SProceduralGraphPanel.h"

int32 SProceduralGraphPanel::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                     const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                     const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const float GridStep = 16.0f; // sabit piksel aralığı
	const FVector2D LocalSize = AllottedGeometry.GetLocalSize();

	// Yatay çizgiler
	for (float Y = 0; Y < LocalSize.Y; Y += GridStep)
	{
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			{ FVector2D(0, Y), FVector2D(LocalSize.X, Y) },
			ESlateDrawEffect::None,
			FLinearColor(0.6f, 0.3f, 0.3f, 1.0f)
		);
	}

	// Dikey çizgiler
	for (float X = 0; X < LocalSize.X; X += GridStep)
	{
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			{ FVector2D(X, 0), FVector2D(X, LocalSize.Y) },
			ESlateDrawEffect::None,
			FLinearColor(0.7f, 0.7f, 0.7f, 1.0f)
		);
	}

	return SGraphEditor::OnPaint(
		Args, AllottedGeometry, MyCullingRect,
		OutDrawElements, LayerId + 1, InWidgetStyle, bParentEnabled);
}
