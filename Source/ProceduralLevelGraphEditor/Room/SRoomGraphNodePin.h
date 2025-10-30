#pragma once

#include "CoreMinimal.h"
#include "SGraphPin.h"
#include "ProceduralLevelGraphRuntime/MazeNodeBase.h"

class SRoomGraphNodePin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SRoomGraphNodePin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);
	EMazeDirection PinLocation;
	
protected:
	virtual const FSlateBrush* GetPinIcon() const override;
	virtual FSlateColor GetPinColor() const override;
	TSharedPtr<FSlateBrush> PinBrushVertical;
	TSharedPtr<FSlateBrush> PinBrushHorizontal;

};
