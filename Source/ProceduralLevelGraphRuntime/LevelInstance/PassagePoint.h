#pragma once

#include "CoreMinimal.h"
#include "LevelBound.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#include "PassagePoint.generated.h"

UCLASS(PrioritizeCategories = "Passage Point Data")
class APassagePoint : public AActor
{
	GENERATED_BODY()

public:
	
	APassagePoint();
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<ALevelBound> LevelBound = nullptr;
#endif

#if WITH_EDITOR
	ALevelBound* FindMyLevelBoundActor();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	UPROPERTY(EditAnywhere, Category = "Passage Point Data", meta=(DisplayPriority="-1"))
	EPassageSize PassageSize = EPassageSize::Double;
	UPROPERTY(EditAnywhere, Category = "Passage Point Data", meta=(DisplayPriority="-1"))
	EMazeDirection DoorLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passage Point Data", meta = (UIMin = "-200", UIMax = "200", ClampMin = "-200", ClampMax = "200", MultipleOf = "1", DisplayPriority="-1"))
	int Offset = 0;
	UPROPERTY(EditAnywhere, Category = "Passage Point Data", meta=(DisplayPriority="-1"))
	EMazeFloor DoorFloor;
	UPROPERTY(EditAnywhere, Category = "Passage Point Data", meta=(DisplayPriority="-1"))
	EPassageType CurrentPassageStatus;
	UPROPERTY(EditAnywhere, Category = "Passage Point Data", meta=(DisplayPriority="-1"))
	TMap<EPassageType, FPassageActorList> PassageActorMap;
	UFUNCTION(BlueprintCallable, Category = "Passage Point Data", meta=(DisplayPriority="-1"))
	void UpdatePassageStatus(EPassageType Status) const;
	UFUNCTION(BlueprintCallable, Category = "Passage Point Data", meta=(DisplayPriority="-1"))
	bool IsPassageDataMatchDoorData(FDoorData Data, bool bHallDoor = false);

protected:
	
	virtual void BeginPlay() override;
	
};