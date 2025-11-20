#pragma once

#include "CoreMinimal.h"
#include "EdGraphNode_Comment.h"
#include "LayoutGraphNode.generated.h"

UCLASS()
class ULayoutGraphNode : public UEdGraphNode_Comment
{
	GENERATED_BODY()
	ULayoutGraphNode();
public:
	UPROPERTY(EditAnywhere, Category = "Background Display")
	TObjectPtr<UTexture2D> BackgroundImage;
	
	UPROPERTY(EditAnywhere, Category = "Background Display", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ImageOpacity = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Background Display")
	FText TitleText;
	virtual void AllocateDefaultPins() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void ResizeNode(const FVector2f& NewSize) override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
};
