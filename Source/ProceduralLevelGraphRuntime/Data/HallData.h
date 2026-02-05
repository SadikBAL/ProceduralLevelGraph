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
	
	//UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "Hall" )
	//TSubclassOf<AHallLevelInstance> HallCapTile;
	
	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "Hall" )
	TSubclassOf<AHallLevelInstance> HallUpCap;
	
	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "Hall" )
	TSubclassOf<AHallLevelInstance> HallDownCap;
	
#if WITH_EDITOR
	DECLARE_MULTICAST_DELEGATE(FOnHallDataChanged);
	FOnHallDataChanged OnHallDataChanged;
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
