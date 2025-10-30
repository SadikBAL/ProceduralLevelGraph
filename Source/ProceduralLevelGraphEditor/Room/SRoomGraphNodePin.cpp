#include "SRoomGraphNodePin.h"

#include "ConnectionDrawingPolicy.h"
#include "EditorStyleSet.h"
#include "Interfaces/IPluginManager.h"
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
	const FString PluginBaseDir = IPluginManager::Get().FindPlugin("ProceduralLevelGraph")->GetBaseDir();
	const FString IconPath = FPaths::Combine(*PluginBaseDir, TEXT("Content/Icons/door.png"));
	PinBrushVertical = MakeShareable(new FSlateImageBrush(IconPath, FVector2D(60.0f, 5.0f)));
	PinBrushHorizontal = MakeShareable(new FSlateImageBrush(IconPath, FVector2D(5.0f, 60.0f)));
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}

const FSlateBrush* SRoomGraphNodePin::GetPinIcon() const
{
	if (PinLocation == EMazeDirection::Down || PinLocation == EMazeDirection::Up)
	{
		return PinBrushVertical.Get();
	}
	else if (PinLocation == EMazeDirection::Left || PinLocation == EMazeDirection::Right)
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
