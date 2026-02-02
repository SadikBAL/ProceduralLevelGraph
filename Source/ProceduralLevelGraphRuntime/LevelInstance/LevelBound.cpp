#include "LevelBound.h"

#include "Components/BoxComponent.h"

ALevelBound::ALevelBound()
{
	PrimaryActorTick.bCanEverTick = false;
	DebugBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DebugBox"));
	DebugBox->ShapeColor = FColor::Red;
	DebugBox->SetHiddenInGame(true);
	DebugBox->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = DebugBox;
}

void ALevelBound::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (DebugBox)
	{
		DebugBox->SetBoxExtent(FVector((Width * 100) / 2.f, (Height * 100) / 2.f, (Depth * 100) / 2.f));
		DebugBox->SetRelativeLocation(FVector(GetActorLocation().X, GetActorLocation().Y, (Depth * 100) / 2.f));
	}
}
