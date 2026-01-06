#pragma once

#include "CoreMinimal.h"
#include "SGraphPin.h"
#include "ProceduralLevelGraphEditor/Node/Data/MazeGraphNodeBase.h"
#include "ProceduralLevelGraphRuntime/Node/MazeNodeBase.h"

enum class EMazePinType : uint8;

class SRoomGraphNodePin : public SGraphPin
{
public:
	
	SLATE_BEGIN_ARGS(SRoomGraphNodePin) {}
	SLATE_END_ARGS()

	FText GetCustomToolTipText();
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	FDoorData PinData;
	UMazeGraphNodeBase* PinBase = nullptr;
	
protected:
	
	virtual const FSlateBrush* GetPinIcon() const override;
	virtual FSlateColor GetPinColor() const override;
	virtual FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	
	TSharedPtr<FSlateBrush> PinBrushVertical;
	TSharedPtr<FSlateBrush> PinBrushHorizontal;

};
