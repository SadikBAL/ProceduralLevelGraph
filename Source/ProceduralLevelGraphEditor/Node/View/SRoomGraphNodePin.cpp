#include "SRoomGraphNodePin.h"

#include "ConnectionDrawingPolicy.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Styling/SlateTypes.h"


void SRoomGraphNodePin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SGraphPin::FArguments Args;
	GraphPinObj = InPin;
	this->SetCursor(EMouseCursor::Crosshairs);
	this->bShowLabel = false;
	check(GraphPinObj != nullptr);
	PinBrushVertical = MakeShareable(new FSlateColorBrush(FLinearColor::White));
	PinBrushVertical->ImageSize = FVector2D(TILE_EDITOR_PIN_SCALE, 8.0f);
	PinBrushHorizontal = MakeShareable(new FSlateColorBrush(FLinearColor::White));
	PinBrushHorizontal->ImageSize = FVector2D(8.0f, TILE_EDITOR_PIN_SCALE);
	
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}

FReply SRoomGraphNodePin::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

const FSlateBrush* SRoomGraphNodePin::GetPinIcon() const
{
	if (PinDirection == EMazeOrientation::Vertical)
	{
		return PinBrushVertical.Get();
	}
	else if (PinDirection == EMazeOrientation::Horizontal)
	{
		return PinBrushHorizontal.Get();
	}
	else
	{
		return nullptr;
	}
	
}

FSlateColor SRoomGraphNodePin::GetPinColor() const
{
	if (IsConnected())
	{
		return FSlateColor(FLinearColor(0.8f, 1.0f, 0.8f));
	}
	else
	{
		return FSlateColor(FLinearColor(0.9f, 0.2f, 0.1f));
	}
}
