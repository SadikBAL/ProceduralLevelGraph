#include "CustomCompoundWidget.h"
#include "SGraphPanel.h"
#include "Interfaces/IPluginManager.h"

void SRouteOverlay::Construct(const FArguments& InArgs)
{
    GraphEditor = InArgs._GraphEditor;
    PathToDraw = InArgs._PathToDraw;
	bShowRoutes = InArgs._bShowRoutes;
}

int32 SRouteOverlay::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                             FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                             bool bParentEnabled) const
{
    SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	if (!bShowRoutes.Get())
	{
		return LayerId;
	}
    TSharedPtr<SGraphEditor> GraphEditorPtr = GraphEditor.Get();
    if (!GraphEditorPtr.IsValid()) return LayerId;
    SGraphPanel* GraphPanel = GraphEditorPtr->GetGraphPanel();
    if (GraphPanel == nullptr) return LayerId;
    
    const TMap<ERouteType, TArray<UMazeGraphNodeBase*>>& Paths = PathToDraw.Get();
    for (auto Path : Paths)
    {
    	TArray<UMazeGraphNodeBase*> TempPath = Path.Value;
    	TArray<FVector2f> Points;
    	for (int32 i = 0; i < TempPath.Num(); ++i)
    	{
    		UMazeGraphNodeBase* Node = TempPath[i];
    		TSharedPtr<SGraphNode> Widget = GraphPanel->GetNodeWidgetFromGuid(Node->NodeGuid);

    		if (Widget.IsValid())
    		{
    			FVector2f PosA = Widget->GetPosition2f();
    			FVector2f SizeA = Widget->GetDesiredSize();
    			FVector2f CenterA_GraphSpace = PosA + SizeA * 0.5f;
    			const FVector2f ViewOffset = GraphPanel->GetViewOffset();
    			const float Zoom = GraphPanel->GetZoomAmount();
    			FVector2f CenterA_LocalSpace = (CenterA_GraphSpace - ViewOffset) * Zoom;
    			CenterA_LocalSpace = FVector2f(CenterA_LocalSpace.X + GetRouteOffset(Path.Key), CenterA_LocalSpace.Y + GetRouteOffset(Path.Key));
    			Points.Add(CenterA_LocalSpace);
    		}
    		else
    		{
    			break;
    		}
    	}
    	FSlateDrawElement::MakeLines(
				   OutDrawElements,
				   LayerId + 1,
				   AllottedGeometry.ToPaintGeometry(),
				   Points,
				   ESlateDrawEffect::DisabledEffect,
				   GetRouteColor(Path.Key),
				   true,
				   3.0f
				   );
    }
    return LayerId + 1;
}

FLinearColor SRouteOverlay::GetRouteColor(ERouteType Type) const
{
	switch (Type)
	{
	case ERouteType::RouteC:
		return FLinearColor(0.7f, 0.7f, 1.0f);
		case ERouteType::RouteB:
		return FLinearColor(1.0f, 0.4f, 0.4f);
		case ERouteType::RouteA:
		return FLinearColor(0.6f, 1.0f, 0.6f);
		case ERouteType::RouteD:
		return FLinearColor(1.0f, 1.0f, 0.4f);
	}
	return FLinearColor(0.9f, 0.9f, 0.9f);
}

float SRouteOverlay::GetRouteOffset(ERouteType Type) const
{
	switch (Type)
	{
	case ERouteType::RouteA:
		return -7.5f;
	case ERouteType::RouteB:
		return -2.5f;
	case ERouteType::RouteC:
		return 2.5f;
	case ERouteType::RouteD:
		return 7.5f;
	}
	return 0.0f;
}

void SGraphBackground::Construct(const FArguments& InArgs)
{
    GraphEditor = InArgs._GraphEditor;
    const FString PluginBaseDir = IPluginManager::Get().FindPlugin("ProceduralLevelGraph")->GetBaseDir();
    const FString IconPath = FPaths::Combine(*PluginBaseDir, TEXT("Content/Icons/ship_layout.jpg")); 
    const FVector2D ImageSize = FVector2D(847.0f, 2767.0f); 
    BackgroundBrush = MakeShareable(new FSlateImageBrush(IconPath, ImageSize));
}

int32 SGraphBackground::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    TSharedPtr<SGraphEditor> GraphEditorPtr = GraphEditor.Get();
    if (!GraphEditorPtr.IsValid() || !BackgroundBrush.IsValid()) return LayerId;
    SGraphPanel* GraphPanel = GraphEditorPtr->GetGraphPanel();
    if (GraphPanel == nullptr) return LayerId;

    const FVector2f ViewOffset = GraphPanel->GetViewOffset();
    const float Zoom = GraphPanel->GetZoomAmount();

    const FVector2f ImageSizeInGraphSpace = BackgroundBrush->ImageSize;

    const FVector2f ImagePositionInGraphSpace = FVector2f(
        -ImageSizeInGraphSpace.X * 0.5f, 
        -ImageSizeInGraphSpace.Y * 0.5f
    ); 
	
    const FVector2f TopLeft_LocalSpace = (ImagePositionInGraphSpace - ViewOffset) * Zoom;
    const FVector2f BottomRight_LocalSpace = ((ImagePositionInGraphSpace + ImageSizeInGraphSpace) - ViewOffset) * Zoom;
    const FVector2f ImageSize_LocalSpace = BottomRight_LocalSpace - TopLeft_LocalSpace;
	
	const FSlateLayoutTransform LayoutTransform(1.0f, TopLeft_LocalSpace);
    
	FPaintGeometry PaintGeometry = AllottedGeometry.ToPaintGeometry(
		ImageSize_LocalSpace,
		LayoutTransform
	);
    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,
        PaintGeometry,
        BackgroundBrush.Get(),
        ESlateDrawEffect::None,
        FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)
    );
    return LayerId + 1; 
}