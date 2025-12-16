#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"

#include "HallDataAsserFactory.generated.h"

UCLASS()
class UHallDataAsserFactory : public UFactory
{
	GENERATED_BODY()

public:
	UHallDataAsserFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};