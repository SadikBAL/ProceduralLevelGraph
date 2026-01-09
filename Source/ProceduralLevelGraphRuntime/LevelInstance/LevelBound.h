#pragma once

#include "CoreMinimal.h"
#include "LevelBound.generated.h"

UCLASS(PrioritizeCategories = "Level Bound Data")
class ALevelBound : public AActor
{
	GENERATED_BODY()

public:
	
	ALevelBound();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level Bound Data", meta = (UIMin = "1", UIMax = "1000", ClampMin = "1", ClampMax = "1000", MultipleOf = "1", DisplayPriority="0"))
	float Width = 6.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level Bound Data", meta = (UIMin = "1", UIMax = "1000", ClampMin = "1", ClampMax = "1000", MultipleOf = "1", DisplayPriority="0"))
	float Height = 6.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Level Bound Data", meta = (UIMin = "1", UIMax = "1000", ClampMin = "1", ClampMax = "1000", MultipleOf = "1", DisplayPriority="0"))
	float Depth = 6.0f;


protected:

	virtual void OnConstruction(const FTransform& Transform) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* DebugBox;
	
};