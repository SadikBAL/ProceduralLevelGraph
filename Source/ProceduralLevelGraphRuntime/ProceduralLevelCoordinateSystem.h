// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Node/MazeNodeBase.h"
#include "Runtime/CoreUObject/Public/UObject/Object.h"
#include "ProceduralLevelCoordinateSystem.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALLEVELGRAPHRUNTIME_API UProceduralLevelCoordinateSystem : public UObject
{
	GENERATED_BODY()
	
public:
	UProceduralLevelCoordinateSystem();
	
    UFUNCTION()
	void SaveBounds(UMazeNodeBase* Node);
	
	UFUNCTION(BlueprintCallable)
	FVector2D GetNormalizedCoordinate(FVector WorldLocation);
	
private:
	float MinX;
	float MaxX;
	float MinY;
	float MaxY;
	float Width;
	float Height;
};
