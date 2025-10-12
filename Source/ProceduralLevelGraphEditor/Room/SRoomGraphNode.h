#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class URoomGraphNode;

class SRoomGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SRoomGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, URoomGraphNode* InNode);
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;

protected:
	TSharedPtr<SGraphPin> UpPin;
	TSharedPtr<SGraphPin> DownPin;
	TSharedPtr<SGraphPin> LeftPin;
	TSharedPtr<SGraphPin> RightPin;
};