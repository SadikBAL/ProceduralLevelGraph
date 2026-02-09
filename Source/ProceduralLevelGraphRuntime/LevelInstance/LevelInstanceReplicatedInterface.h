#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "GameFramework/Actor.h"
#include "LevelInstanceReplicatedInterface.generated.h"
UINTERFACE(MinimalAPI)
class ULevelInstanceReplicatedInterface : public UInterface
{
	GENERATED_BODY()
};
class ILevelInstanceReplicatedInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void ReplaceActor(AActor* Source);
};
