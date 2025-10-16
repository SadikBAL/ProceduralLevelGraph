#include "MazeGraphSchemaFactory.h"

#include "MazeGraphSchema.h"
#include "EdGraph/EdGraph.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphRuntime.h"

UMazeGraphSchemaFactory::UMazeGraphSchemaFactory()
{
    bCreateNew = true;
    bEditAfterNew = true;
    SupportedClass = UProceduralLevelGraphRuntime::StaticClass();
}

UObject* UMazeGraphSchemaFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UProceduralLevelGraphRuntime* MazeGraph = NewObject<UProceduralLevelGraphRuntime>(InParent, InClass, InName, Flags | RF_Transactional);
    if (MazeGraph)
    {
        MazeGraph->EdGraph = NewObject<UEdGraph>(MazeGraph, FName("UMazeGraphSchema"), RF_Transactional);
        MazeGraph->EdGraph->Schema = UMazeGraphSchema::StaticClass();
    }
    return MazeGraph;
}

bool UMazeGraphSchemaFactory::ShouldShowInNewMenu() const
{
    return true;
}