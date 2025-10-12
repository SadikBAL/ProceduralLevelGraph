#include "SRoomGraphNodePin.h"

#include "Interfaces/IPluginManager.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Styling/SlateStyleRegistry.h" // Brush oluşturmak için
#include "Styling/SlateTypes.h"         // FSlateImageBrush için

void SRoomGraphNodePin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	GraphPinObj = InPin;
	check(GraphPinObj != nullptr);
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	const FString PluginBaseDir = IPluginManager::Get().FindPlugin("ProceduralLevelGraph")->GetBaseDir();
	const FString IconPath = FPaths::Combine(*PluginBaseDir, TEXT("Content/Icons/door.png"));
	PinBrush = MakeShareable(new FSlateImageBrush(IconPath, FVector2D(16.0f, 16.0f)));
}

const FSlateBrush* SRoomGraphNodePin::GetPinIcon() const
{
	// Bu fonksiyon pinin rengini ve şeklini (dolu, boş) belirler.
	// Base class'taki bu mantığı yeniden kullanmak en iyisidir.
	return PinBrush.Get();
}