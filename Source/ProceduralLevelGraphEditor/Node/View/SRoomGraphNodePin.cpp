#include "SRoomGraphNodePin.h"

#include "ConnectionDrawingPolicy.h"
#include "ProceduralLevelGraphEditor/Node/Data/MazeGraphNodeBase.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Styling/SlateTypes.h"
#include "Widgets/Colors/SColorBlock.h"

#define LOCTEXT_NAMESPACE "SRoomGraphNodePin"

void SRoomGraphNodePin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{

	GraphPinObj = InPin;
	//this->SetCursor(EMouseCursor::None);
	//this->bShowLabel = false;
	//this->SetDiffHighlighted(false);
	//this->SetIsEditable(false);
	check(GraphPinObj != nullptr);
	PinBrushVertical = MakeShareable(new FSlateColorBrush(FLinearColor::White));
	PinBrushVertical->ImageSize = FVector2D(TILE_EDITOR_PIN_SCALE, 8.0f);
	PinBrushHorizontal = MakeShareable(new FSlateColorBrush(FLinearColor::White));
	PinBrushHorizontal->ImageSize = FVector2D(8.0f, TILE_EDITOR_PIN_SCALE);
	PinBase = Cast<UMazeGraphNodeBase>(GraphPinObj->GetOwningNode());
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}

FReply SRoomGraphNodePin::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

const FSlateBrush* SRoomGraphNodePin::GetPinIcon() const
{
	if (PinBase)
	{
		if (PinBase->RoomRotation == 0 || PinBase->RoomRotation == 180)
		{
			if (PinData.DoorDirection == EMazeOrientation::Vertical)
			{
				return PinBrushVertical.Get();
			}
			else if (PinData.DoorDirection == EMazeOrientation::Horizontal)
			{
				return PinBrushHorizontal.Get();
			}
		}
		else
		{
			if (PinData.DoorDirection == EMazeOrientation::Horizontal)
			{
				return PinBrushVertical.Get();
			}
			else if (PinData.DoorDirection == EMazeOrientation::Vertical)
			{
				return PinBrushHorizontal.Get();
			}
		}
	}
	if (PinData.DoorDirection == EMazeOrientation::Vertical)
	{
		return PinBrushVertical.Get();
	}
	else if (PinData.DoorDirection == EMazeOrientation::Horizontal)
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
		return GetPinColorWithHeight(PinData.DoorFloor);
	}
}

#undef LOCTEXT_NAMESPACE