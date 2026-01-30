#pragma once
#include "MazeTileLevelInstance.h"

#include "HallLevelInstance.generated.h"

UCLASS(Abstract, HideDropdown)
class AHallLevelInstance : public AMazeTileLevelInstance
{
	GENERATED_BODY()
public:
	AHallLevelInstance();
#if WITH_EDITOR
	DECLARE_MULTICAST_DELEGATE(FOnHallLevelInstanceChanged);
	FOnHallLevelInstanceChanged OnHallDataChanged;
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
