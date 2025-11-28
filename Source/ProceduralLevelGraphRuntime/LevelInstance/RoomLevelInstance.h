#pragma once

#include "MazeTileLevelInstance.h"
#include "RoomLevelInstance.generated.h"

UCLASS(Abstract, HideDropdown)
class ARoomLevelInstance : public AMazeTileLevelInstance
{
	GENERATED_BODY()
public:
	ARoomLevelInstance();

};
