#pragma once

#include "CoreMinimal.h"
#include "LevelBound.generated.h"

UCLASS()
class ALevelBound : public AActor
{
	GENERATED_BODY()

public:
	
	ALevelBound();

protected:

	virtual void BeginPlay() override;
	
};