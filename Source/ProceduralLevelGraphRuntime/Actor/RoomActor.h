#pragma once

#include "MazeTileActor.h"
#include "RoomActor.generated.h"

UCLASS(Abstract, HideDropdown)
class ARoomActor : public AMazeTileActor
{
	GENERATED_BODY()
public:
	ARoomActor();

};
