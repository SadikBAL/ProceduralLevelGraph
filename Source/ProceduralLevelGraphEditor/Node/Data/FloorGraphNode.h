#pragma once

#include "CoreMinimal.h"
#include "MazeGraphNodeBase.h"
#include "Styling/SlateBrush.h" // FSlateBrush için
#include "FloorGraphNode.generated.h"

UCLASS()
class UFloorGraphNode : public UMazeGraphNodeBase
{
	GENERATED_BODY()

public:
	UFloorGraphNode();

	virtual void AllocateDefaultPins() override;

	virtual bool CanUserDeleteNode() const override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
	//float NodeWidth;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
	//float NodeHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
	FSlateBrush BackgroundBrush;

	virtual FLinearColor GetNodeTitleColor() const override;
};