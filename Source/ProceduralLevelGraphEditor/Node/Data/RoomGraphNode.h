#pragma once

#include "CoreMinimal.h"
#include "MazeGraphNodeBase.h"
#include "RoomGraphNode.generated.h"

UCLASS()
class URoomGraphNode : public UMazeGraphNodeBase
{
    GENERATED_BODY()
    URoomGraphNode();
public:
    
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void AllocateDefaultPins() override;
    void OnTileBlueprintsChanged();
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room")
    int RoomWith;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room")
    int RoomHeight;
    UPROPERTY(EditDefaultsOnly, Category = "Room")
    TArray<TSubclassOf<AMazeTileActor>> RoomBlueprints;
    UPROPERTY(EditDefaultsOnly, Category = "Room")
    TArray<TSubclassOf<AMazeTileActor>> HallBlueprints;
};