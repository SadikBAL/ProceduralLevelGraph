#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "ProceduralLevelGraphFactory.generated.h"

UCLASS()
class UProceduralLevelGraphFactory : public UFactory
{
    GENERATED_BODY()

public:
    UProceduralLevelGraphFactory();

    // UFactory interface
    virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
    virtual bool ShouldShowInNewMenu() const override;
    
    // End of UFactory interface
};