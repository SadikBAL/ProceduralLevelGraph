#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "SMazeGraphNodeBase.h"

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

protected:
	TSharedPtr<SGraphPin> UpPin;
	TSharedPtr<SGraphPin> DownPin;
	TSharedPtr<SGraphPin> LeftPin;
	TSharedPtr<SGraphPin> RightPin;
};