#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MazeGraphNodeBase.generated.h"

UCLASS()
class UMazeGraphNodeBase : public UEdGraphNode
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	FGameplayTagContainer GameplayTags;
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	UFUNCTION(CallInEditor, Category = "Node Process",DisplayName="Rotate Right")
	void RotateRoomToRight();
	UFUNCTION(CallInEditor, Category = "Node Process",DisplayName="Rotate Left")
	void RotateRoomToLeft();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Node Process")
	int RoomRotation = 0;
protected:
	UPROPERTY()
	FText NodeTitle = FText::FromString("ROOM");
	UPROPERTY()
	FText DoorA = FText::FromString("A");
	UPROPERTY()
	FText DoorB = FText::FromString("B");
	UPROPERTY()
	FText DoorC = FText::FromString("C");
	UPROPERTY()
	FText DoorD = FText::FromString("D");
};
