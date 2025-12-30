#include "SRoomGraphNodePin.h"

#include "ConnectionDrawingPolicy.h"
#include "ProceduralLevelGraphEditor/Node/Data/MazeGraphNodeBase.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Styling/SlateTypes.h"
#include "Widgets/Colors/SColorBlock.h"

#define LOCTEXT_NAMESPACE "SRoomGraphNodePin"

FText SRoomGraphNodePin::GetCustomToolTipText()
{
	FString FloorName = StaticEnum<EMazeFloor>()->GetNameStringByValue((int64)PinData.DoorFloor);
	int32 Height = GetFloorHeight(PinData.DoorFloor);
	FString ToolTipString = FString::Printf(TEXT("Floor : %s\nHeight : %d \nStatus: %s"), 
		*FloorName, 
		Height, 
		IsConnected() ? TEXT("Connect") : TEXT("Disconnect"));

	return FText::FromString(ToolTipString);
}

void SRoomGraphNodePin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SGraphPin::FArguments Args;
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
	//this->SetToolTipText(TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateSP(this, &SRoomGraphNodePin::GetCustomToolTipText)));
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

TSharedPtr<IToolTip> SRoomGraphNodePin::GetToolTip()
{
	FText FloorName = GetCustomToolTipText();
	int32 Height = GetFloorHeight(PinData.DoorFloor);
	FSlateColor FloorColor = GetPinColor();

	return SNew(SToolTip)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Details")))
			.Font(FCoreStyle::Get().GetFontStyle("BoldFont"))
			.ColorAndOpacity(FLinearColor::White)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5, 2)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(STextBlock).Text(FText::Format(LOCTEXT("FloorLbl", "Floor : {0}"), FloorName))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(10, 0, 0, 0)
			[
				// Katın rengini gösteren küçük bir kutucuk
				SNew(SColorBlock)
				.Color(FloorColor.GetSpecifiedColor())
				.Size(FVector2D(16.0f, 16.0f))
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5, 2)
		[
			SNew(STextBlock)
			.Text(FText::Format(LOCTEXT("HeightLbl", "Height : {0}"), FText::AsNumber(Height)))
			.ColorAndOpacity(FSlateColor(FLinearColor(0.7f, 0.7f, 0.7f)))
		]
	];
}
#undef LOCTEXT_NAMESPACE