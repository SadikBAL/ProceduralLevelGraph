#include "HallData.h"

UHallData::UHallData(const FObjectInitializer& ObjectInitializer)
{
}
#if WITH_EDITOR
void UHallData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	OnHallDataChanged.Broadcast();
}
#endif