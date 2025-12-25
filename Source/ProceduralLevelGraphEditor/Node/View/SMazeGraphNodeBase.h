#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "SRoomGraphNodePin.h"

class SMazeGraphNodeBase : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SMazeGraphNodeBase) {}
	SLATE_END_ARGS()
	
	virtual void GetAllPinWidgets(TArray<TSharedPtr<SGraphPin>>& OutPinWidgets) const;
	virtual void MoveTo(const FVector2f& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty = true) override;
	FVector2D GetPinPositionInGraphSpace(const TSharedPtr<SGraphPanel>& GraphPanel, const UEdGraphPin* Pin);
	virtual FOptionalSize GetNodeHeight() const;
	virtual FOptionalSize GetNodeWidth() const;
	virtual FText GetNodeName() const;
	virtual FSlateColor GetNodeBackgroundColor() const;
	
protected:
	
	int PinPadding = -4;
	bool bOnDrag = false;
	TArray<TSharedPtr<SRoomGraphNodePin>> Pins;
};