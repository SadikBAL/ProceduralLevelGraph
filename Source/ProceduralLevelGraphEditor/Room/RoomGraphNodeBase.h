#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RoomGraphNodeBase.generated.h"

UCLASS()
class URoomGraphNodeBase : public UEdGraphNode
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	FGameplayTagContainer GameplayTags;
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
};
