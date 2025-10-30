#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "SMazeGraphNodeBase.h"
#include "SRoomGraphNodePin.h"

class UHallGraphNode;

class SHallGraphNode : public SMazeGraphNodeBase
{
public:
	SLATE_BEGIN_ARGS(SHallGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UHallGraphNode* InNode);
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;
	virtual void GetAllPinWidgets(TArray<TSharedPtr<SGraphPin>>& OutPinWidgets) const override;
	virtual FOptionalSize GetNodeHeight() const override;
	virtual FOptionalSize GetNodeWidth() const override;

protected:
	TSharedPtr<SRoomGraphNodePin> UpPin;
	TSharedPtr<SRoomGraphNodePin> DownPin;
	UHallGraphNode* HallGraphNodeRef;
};