#include "SPassageGraphNodePin.h"

#include "ConnectionDrawingPolicy.h"
#include "ProceduralLevelGraphEditor/Node/Data/MazeGraphNodeBase.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"

#define LOCTEXT_NAMESPACE "SRoomGraphNodePin"
FText SPassageGraphNodePin::GetCustomToolTipText()
{
	FString FloorName = StaticEnum<EMazeFloor>()->GetNameStringByValue(static_cast<int64>(AddFloor(PinBase->RoomFloor,PinData.DoorFloor)));
	FString DirectionName = StaticEnum<EMazeDirection>()->GetNameStringByValue(static_cast<int64>(PinBase->GetRotatedPinDirection(PinData.DoorLocation)));
	int32 Height = GetFloorHeight(PinData.DoorFloor);
	FString ToolTipString = FString::Printf(TEXT("Floor : %s\nHeight : %d \nStatus: %s \nDirection: %s" ), 
		*FloorName, 
		Height, 
		IsConnected() ? TEXT("Connect") : TEXT("Disconnect"),
		*DirectionName);
	return FText::FromString(ToolTipString);
}
void SPassageGraphNodePin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{

	GraphPinObj = InPin;
	this->SetCursor(EMouseCursor::None);
	this->bShowLabel = false;
	this->SetDiffHighlighted(false);
	this->SetIsEditable(false);
	check(GraphPinObj != nullptr);
	PinBrushVertical = MakeShareable(new FSlateColorBrush(FLinearColor::White));
	PinBrushVertical->ImageSize = FVector2D(TILE_EDITOR_PIN_SCALE, 8.0f);
	PinBrushHorizontal = MakeShareable(new FSlateColorBrush(FLinearColor::White));
	PinBrushHorizontal->ImageSize = FVector2D(8.0f, TILE_EDITOR_PIN_SCALE);
	PinBase = Cast<UMazeGraphNodeBase>(GraphPinObj->GetOwningNode());
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}

FReply SPassageGraphNodePin::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

const FSlateBrush* SPassageGraphNodePin::GetPinIcon() const
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

FSlateColor SPassageGraphNodePin::GetPinColor() const
{
	if (IsConnected())
	{
		return FSlateColor(FLinearColor(0.8f, 1.0f, 0.8f));
	}
	else
	{
		return GetPinColorWithHeight(AddFloor(PinBase->RoomFloor,PinData.DoorFloor));
	}
}

FCursorReply SPassageGraphNodePin::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	return FCursorReply::Cursor(EMouseCursor::Default);
}

void SPassageGraphNodePin::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);
}

void SPassageGraphNodePin::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SCompoundWidget::OnMouseLeave(MouseEvent);
}

#undef LOCTEXT_NAMESPACE
