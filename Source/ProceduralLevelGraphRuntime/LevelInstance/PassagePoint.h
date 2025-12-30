#pragma once

#include "CoreMinimal.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphTypes.h"
#include "PassagePoint.generated.h"

UCLASS()
class APassagePoint : public AActor
{
	GENERATED_BODY()

public:
	
	APassagePoint();
	
	UPROPERTY(EditAnywhere)
	EMazeFloor Floor;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

	virtual void BeginPlay() override;
	
};