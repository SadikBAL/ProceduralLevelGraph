#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProceduralLevelGraphRuntime/LevelInstance/HallLevelInstance.h"
#include "HallData.generated.h"


UCLASS ( BlueprintType , Const , Meta = ( DisplayName = "Hall Data" , ShortTooltip = "Data asset used to define a Hall." ) )
class UHallData : public UPrimaryDataAsset
{

	GENERATED_BODY ( )
public:
	UHallData ( const FObjectInitializer& ObjectInitializer );
	
	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "Hall" )
	TArray<TSubclassOf<AHallLevelInstance>> HallTiles;
	
	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "Hall" )
	TSubclassOf<AHallLevelInstance> HallStartTile;
	
	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "Hall" )
	TSubclassOf<AHallLevelInstance> HallEndTile;
	
};
