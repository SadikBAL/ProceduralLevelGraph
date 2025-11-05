#pragma once
#include "SGraphPin.h"
#include "SRoomGraphNode.h"
#include "ProceduralLevelGraphEditor/Node/Data/EntranceGraphNode.h"

class SEntranceGraphNode : public SRoomGraphNode
{
public:
public:
	SLATE_BEGIN_ARGS(SEntranceGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEntranceGraphNode* InNode);
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;
	virtual void GetAllPinWidgets(TArray<TSharedPtr<SGraphPin>>& OutPinWidgets) const override;
	virtual FOptionalSize GetNodeHeight() const override;
	virtual FOptionalSize GetNodeWidth() const override;
};
