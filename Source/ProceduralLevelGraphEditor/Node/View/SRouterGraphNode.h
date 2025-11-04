#pragma once

#include "CoreMinimal.h"
#include "SMazeGraphNodeBase.h"
#include "SRoomGraphNodePin.h"

class URouterGraphNode;

class SRouterGraphNode : public SMazeGraphNodeBase
{
public:
	SLATE_BEGIN_ARGS(SRouterGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, URouterGraphNode* InNode);
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;
	virtual void GetAllPinWidgets(TArray<TSharedPtr<SGraphPin>>& OutPinWidgets) const override;
	virtual FOptionalSize GetNodeHeight() const override;
	virtual FOptionalSize GetNodeWidth() const override;
};