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
	// Bu fonksiyon, pinin ikonunu (yuvarlak şeklini) döndürür.
	// Base class'ın implementasyonunu direkt kullanacağız.
	virtual const FSlateBrush* GetPinIcon() const override;
	TSharedPtr<FSlateBrush> PinBrush;
};