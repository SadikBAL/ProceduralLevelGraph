#include "CustomCompoundWidget.h"
#include "SGraphPanel.h"

void SRouteOverlay::Construct(const FArguments& InArgs)
{
    GraphEditor = InArgs._GraphEditor;
    PathToDraw = InArgs._PathToDraw;
}

int32 SRouteOverlay::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                             FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                             bool bParentEnabled) const
{
    SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
    TSharedPtr<SGraphEditor> GraphEditorPtr = GraphEditor.Get();
    if (!GraphEditorPtr.IsValid()) return LayerId;
    SGraphPanel* GraphPanel = GraphEditorPtr->GetGraphPanel();
    if (GraphPanel == nullptr) return LayerId;
    
    const TArray<UMazeGraphNodeBase*>& Path = PathToDraw.Get();
    if (Path.Num() < 2) return LayerId;
    TArray<FVector2f> Points;
    for (int32 i = 0; i < Path.Num(); ++i)
    {
        UMazeGraphNodeBase* Node = Path[i];
        TSharedPtr<SGraphNode> Widget = GraphPanel->GetNodeWidgetFromGuid(Node->NodeGuid);

        if (Widget.IsValid())
        {
            FVector2f PosA = Widget->GetPosition2f();
            FVector2f SizeA = Widget->GetDesiredSize();
            FVector2f CenterA_GraphSpace = PosA + SizeA * 0.5f;
            const FVector2f ViewOffset = GraphPanel->GetViewOffset();
            const float Zoom = GraphPanel->GetZoomAmount();
            FVector2f CenterA_LocalSpace = (CenterA_GraphSpace - ViewOffset) * Zoom;
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
               FLinearColor::Red,
               true,
               5.0f
               );
    return LayerId + 1;
}
