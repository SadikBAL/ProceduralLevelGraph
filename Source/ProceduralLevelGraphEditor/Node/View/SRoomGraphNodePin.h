#pragma once

#include "CoreMinimal.h"
#include "SGraphPin.h"
#include "SMazeGraphNodeBase.h"
#include "ProceduralLevelGraphEditor/Node/Data/MazeGraphNodeBase.h"
#include "ProceduralLevelGraphRuntime/Node/MazeNodeBase.h"

enum class EMazePinType : uint8;

class SRoomGraphNodePin : public SGraphPin
{
public:
	
	SLATE_BEGIN_ARGS(SRoomGraphNodePin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	EMazeOrientation PinDirection;
	EMazePinType PinType;
	EMazeDirection PinLocation;
	FVector2D PinOffset;
	UMazeGraphNodeBase* PinBase = nullptr;
	
protected:
	
	virtual const FSlateBrush* GetPinIcon() const override;
	virtual FSlateColor GetPinColor() const override;
	
	TSharedPtr<FSlateBrush> PinBrushVertical;
	TSharedPtr<FSlateBrush> PinBrushHorizontal;

};
