#pragma once

#include "CoreMinimal.h"
#include "SGraphPin.h"

class SRoomGraphNodePin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SRoomGraphNodePin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected:
	virtual const FSlateBrush* GetPinIcon() const override;
	virtual FSlateColor GetPinColor() const override;
	TSharedPtr<FSlateBrush> PinBrush;
};