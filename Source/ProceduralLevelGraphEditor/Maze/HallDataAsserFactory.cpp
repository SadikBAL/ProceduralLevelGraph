#include "HallDataAsserFactory.h"
#include "ProceduralLevelGraphEditor/ProceduralLevelGraphEditorModule.h"
#include "ProceduralLevelGraphRuntime/Data/HallData.h"

UHallDataAsserFactory::UHallDataAsserFactory()
{
	SupportedClass = UHallData::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UHallDataAsserFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UHallData>(InParent, InClass, InName, Flags);
}
