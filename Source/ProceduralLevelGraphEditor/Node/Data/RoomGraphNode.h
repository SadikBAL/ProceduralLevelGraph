#pragma once

#include "CoreMinimal.h"
#include "MazeGraphNodeBase.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/RoomLevelInstance.h"
#include "RoomGraphNode.generated.h"

class AHallLevelInstance;

UCLASS()
class URoomGraphNode : public UMazeGraphNodeBase
{
    GENERATED_BODY()
    
public:
    URoomGraphNode();
    
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void AllocateDefaultPins() override;
    void OnTileBlueprintsChanged();
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    virtual void PostLoad() override;
    virtual EMazeDirection GetMazePinDirection(const UEdGraphPin* Pin) override;
    int32 GetIndexFromPinName(const FName& PinName);
    virtual FText GetNodeName() const override;
    virtual FSlateColor GetNodeBackgroundColor() const override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room")
    int RoomWidth;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room")
    int RoomHeight;
    UPROPERTY(EditDefaultsOnly, Category = "Room")
    TArray<TSubclassOf<ARoomLevelInstance>> RoomLevelInstanceRef;
    UPROPERTY(EditDefaultsOnly, Category = "Room")
    TArray<TSubclassOf<AHallLevelInstance>> HallLevelInstanceRef;
};