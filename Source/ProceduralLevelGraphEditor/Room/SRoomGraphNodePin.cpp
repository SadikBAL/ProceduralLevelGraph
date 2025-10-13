#include "SRoomGraphNodePin.h"

#include "ConnectionDrawingPolicy.h"
#include "Interfaces/IPluginManager.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Styling/SlateStyleRegistry.h" // Brush oluşturmak için
#include "Styling/SlateTypes.h"         // FSlateImageBrush için


void SRoomGraphNodePin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	GraphPinObj = InPin;
	this->SetCursor(EMouseCursor::Crosshairs);
	this->bShowLabel = false;
	check(GraphPinObj != nullptr);
	const FString PluginBaseDir = IPluginManager::Get().FindPlugin("ProceduralLevelGraph")->GetBaseDir();
	const FString IconPath = FPaths::Combine(*PluginBaseDir, TEXT("Content/Icons/door.png"));
	PinBrush = MakeShareable(new FSlateImageBrush(IconPath, FVector2D(32.0f, 32.0f)));
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}

const FSlateBrush* SRoomGraphNodePin::GetPinIcon() const
{
	return PinBrush.Get();
}
