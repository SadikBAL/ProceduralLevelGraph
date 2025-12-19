#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"

#include "HallDataAssetFactory.generated.h"

UCLASS()
class UHallDataAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UHallDataAssetFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};