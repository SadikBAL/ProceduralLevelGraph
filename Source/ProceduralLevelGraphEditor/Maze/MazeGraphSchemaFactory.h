#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MazeGraphSchemaFactory.generated.h"

UCLASS()
class UMazeGraphSchemaFactory : public UFactory
{
    GENERATED_BODY()

public:
    UMazeGraphSchemaFactory();

    // UFactory interface
    virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
    virtual bool ShouldShowInNewMenu() const override;
    // End of UFactory interface
    
};