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
	//const FString PluginBaseDir = IPluginManager::Get().FindPlugin("ProceduralLevelGraph")->GetBaseDir();
	//const FString IconPath = FPaths::Combine(*PluginBaseDir, TEXT("Content/Icons/door.png"));
	//PinBrush = MakeShareable(new FSlateImageBrush(IconPath, FVector2D(32.0f, 32.0f)));

	BlackBrush = MakeShareable(new FSlateBrush());
	BlackBrush->DrawAs = ESlateBrushDrawType::Box;
	BlackBrush->TintColor = FLinearColor::Black;
	BlackBrush->ImageSize = FVector2D(32.0f, 32.0f);
	BlackBrush->Margin = FMargin(0.0f);
	GreenBrush = MakeShareable(new FSlateBrush());
	GreenBrush->DrawAs = ESlateBrushDrawType::Box;
	GreenBrush->TintColor = FLinearColor::Green;
	GreenBrush->Margin = FMargin(0.0f);
	GreenBrush->ImageSize = FVector2D(32.0f, 32.0f);
	
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}

const FSlateBrush* SRoomGraphNodePin::GetPinIcon() const
{
	return GreenBrush.Get();
}
