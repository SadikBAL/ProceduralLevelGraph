#include "ProceduralLevelGraphRuntimeModule.h"

#define LOCTEXT_NAMESPACE "FProceduralLevelGraphRuntimeModule"

void FProceduralLevelGraphRuntimeModule::StartupModule()
{
	//IModuleInterface::StartupModule();
}

void FProceduralLevelGraphRuntimeModule::ShutdownModule()
{
	//IModuleInterface::ShutdownModule();
}
#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FProceduralLevelGraphRuntimeModule, ProceduralLevelGraphRuntime)