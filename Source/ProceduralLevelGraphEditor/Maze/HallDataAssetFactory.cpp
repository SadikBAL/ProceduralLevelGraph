#include "HallDataAssetFactory.h"
#include "ProceduralLevelGraphEditor/ProceduralLevelGraphEditorModule.h"
#include "ProceduralLevelGraphRuntime/Data/HallData.h"

UHallDataAssetFactory::UHallDataAssetFactory()
{
	SupportedClass = UHallData::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UHallDataAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UHallData>(InParent, InClass, InName, Flags);
}
