#pragma once

#include "CoreMinimal.h"
#include "MazeNodeBase.h"
#include "UObject/Object.h"
#include "ProceduralLevelGraphRuntime.generated.h"

// Forward declaration
class UEdGraph;

UCLASS(Blueprintable)
class PROCEDURALLEVELGRAPHRUNTIME_API UProceduralLevelGraphRuntime : public UObject
{
    GENERATED_BODY()

public:
    UProceduralLevelGraphRuntime();

#if WITH_EDITORONLY_DATA
    UPROPERTY()
    TObjectPtr<UEdGraph> EdGraph;

    void SaveGraphToRuntimeData();
#endif
    
    UPROPERTY(BlueprintReadOnly, Category = "Procedural Level Graph")
    TArray<TObjectPtr<UMazeNodeBase>> AllNodes;
    
    UPROPERTY(BlueprintReadOnly, Category = "Procedural Level Graph")
    TObjectPtr<UMazeNodeBase> StartNode;
};