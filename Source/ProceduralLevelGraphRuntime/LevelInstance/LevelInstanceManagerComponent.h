#pragma once

#include "CoreMinimal.h"
#include "MazeTileLevelInstance.h"
#include "Components/ChildActorComponent.h"
#include "LevelInstanceManagerComponent.generated.h"

class UPirateExperienceDefinition;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ULevelInstanceManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULevelInstanceManagerComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMazeTileLevelInstance* LevelInstance;
protected:
	virtual void BeginPlay() override;
	virtual void OnRegister() override;

};