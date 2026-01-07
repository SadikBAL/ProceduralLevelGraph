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

#if WITH_EDITOR
	TObjectPtr<ALevelBound> LevelBound = nullptr;
	ALevelBound* FindMyLevelBoundActor();
#endif
	
	UPROPERTY(EditAnywhere, Category = "Passage Point Data", meta=(DisplayPriority="-1"))
	EMazeDirection DoorLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passage Point Data", meta = (UIMin = "-50", UIMax = "50", ClampMin = "-50", ClampMax = "50", MultipleOf = "1", DisplayPriority="-1"))
	int Offset = 0;
	UPROPERTY(EditAnywhere, Category = "Passage Point Data", meta=(DisplayPriority="-1"))
	EMazeFloor DoorFloor;
	UPROPERTY(EditAnywhere, Category = "Passage Point Data", meta=(DisplayPriority="-1"))
	EPassageType CurrentPassageStatus;
	UPROPERTY(EditAnywhere, Category = "Passage Point Data", meta=(DisplayPriority="-1"))
	TMap<EPassageType, AActor*> PassageActorMap;
	UFUNCTION(BlueprintCallable, Category = "Passage Point Data", meta=(DisplayPriority="-1"))
	void UpdatePassageStatus(EPassageType Status) const;
	UFUNCTION(BlueprintCallable, Category = "Passage Point Data", meta=(DisplayPriority="-1"))
	bool IsPassageDataMatchDoorData(FDoorData Data);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

	virtual void BeginPlay() override;
	
};