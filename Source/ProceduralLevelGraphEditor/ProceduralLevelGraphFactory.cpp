#include "ProceduralLevelGraphFactory.h"

#include "EdGraphSchema_ProceduralLevelGraph.h"
#include "EdGraph_ProceduralLevelGraph.h" // Henüz oluşturmadık
#include "SGraphNode_PLGRoom.h"
#include "EdGraph/EdGraph.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraph.h"

UProceduralLevelGraphFactory::UProceduralLevelGraphFactory()
{
    bCreateNew = true;
    bEditAfterNew = true;
    SupportedClass = UProceduralLevelGraph::StaticClass();
}

UObject* UProceduralLevelGraphFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UProceduralLevelGraph* NewGraphAsset = NewObject<UProceduralLevelGraph>(InParent, InClass, InName, Flags | RF_Transactional);
    if (NewGraphAsset)
    {
        // Asset'in içinde görsel grafiği oluştur
        NewGraphAsset->EdGraph = NewObject<UEdGraph_ProceduralLevelGraph>(NewGraphAsset, FName("ProceduralLevelGraph_EdGraph"), RF_Transactional);
        NewGraphAsset->EdGraph->Schema = UEdGraphSchema_ProceduralLevelGraph::StaticClass();
    }
    return NewGraphAsset;
}

bool UProceduralLevelGraphFactory::ShouldShowInNewMenu() const
{
    return true;
}