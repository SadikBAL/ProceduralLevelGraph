#pragma once

#include "CoreMinimal.h"
#include "SMazeGraphNodeBase.h"
#include "SRoomGraphNodePin.h"

class URoomGraphNode;

class SRoomGraphNode : public SMazeGraphNodeBase
{
public:
	SLATE_BEGIN_ARGS(SRoomGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, URoomGraphNode* InNode);
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;
	virtual void GetAllPinWidgets(TArray<TSharedPtr<SGraphPin>>& OutPinWidgets) const override;
	virtual FOptionalSize GetNodeHeight() const override;
	virtual FOptionalSize GetNodeWidth() const override;
};