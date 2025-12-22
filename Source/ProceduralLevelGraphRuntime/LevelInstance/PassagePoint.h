#pragma once

#include "CoreMinimal.h"
#include "PassagePoint.generated.h"

UCLASS()
class APassagePoint : public AActor
{
	GENERATED_BODY()

public:
	
	APassagePoint();

protected:

	virtual void BeginPlay() override;
	
};