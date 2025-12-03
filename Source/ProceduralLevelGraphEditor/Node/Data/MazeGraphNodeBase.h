#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#include "MazeGraphNodeBase.generated.h"

UCLASS()
class UMazeGraphNodeBase : public UEdGraphNode
{
	GENERATED_BODY()

public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool CanUserDeleteNode() const override;
	virtual bool CanUserCopyNode() const;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual EMazePinType GetPinType();

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
	bool bIsOnHighlightedPath;

};
