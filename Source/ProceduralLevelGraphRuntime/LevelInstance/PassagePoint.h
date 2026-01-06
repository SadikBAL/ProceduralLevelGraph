#pragma once

#include "CoreMinimal.h"
#include "LevelBound.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#include "PassagePoint.generated.h"

UCLASS()
class APassagePoint : public AActor
{
	GENERATED_BODY()

public:
	
	APassagePoint();

#if WITH_EDITOR
	TObjectPtr<ALevelBound> LevelBound = nullptr;
	ALevelBound* FindMyLevelBoundActor();
#endif
	
	UPROPERTY(EditAnywhere, Category = "Door Data")
	EMazeDirection DoorLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door Data", meta = (UIMin = "-50", UIMax = "50", ClampMin = "-50", ClampMax = "50", MultipleOf = "1"))
	int Offset = 0;
	UPROPERTY(EditAnywhere, Category = "Door Data")
	EMazeFloor DoorFloor;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

	virtual void BeginPlay() override;
	
};