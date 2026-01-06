#pragma once

#include "CoreMinimal.h"
#include "LevelBound.generated.h"

UCLASS()
class ALevelBound : public AActor
{
	GENERATED_BODY()

public:
	
	ALevelBound();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level", meta = (UIMin = "1", UIMax = "100", ClampMin = "1", ClampMax = "100", MultipleOf = "1"))
	float Width = 6.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level", meta = (UIMin = "1", UIMax = "100", ClampMin = "1", ClampMax = "100", MultipleOf = "1"))
	float Height = 6.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Level", meta = (UIMin = "1", UIMax = "100", ClampMin = "1", ClampMax = "100", MultipleOf = "1"))
	float Depth = 6.0f;


protected:

	virtual void OnConstruction(const FTransform& Transform) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* DebugBox;
	
};