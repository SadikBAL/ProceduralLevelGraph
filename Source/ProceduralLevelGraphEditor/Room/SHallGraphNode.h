#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class UHallGraphNode;

class SHallGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SHallGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UHallGraphNode* InNode);
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;

protected:
	TSharedPtr<SGraphPin> PinA;
	TSharedPtr<SGraphPin> PinB;
};