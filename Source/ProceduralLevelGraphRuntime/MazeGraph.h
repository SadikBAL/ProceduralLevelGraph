#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MazeGraph.generated.h"

// Forward declaration
class UEdGraph;

UCLASS(Blueprintable)
class PROCEDURALLEVELGRAPHRUNTIME_API UMazeGraph : public UObject
{
    GENERATED_BODY()

public:
    UMazeGraph();

#if WITH_EDITORONLY_DATA
    // Editörde grafiği görselleştirmek için kullanılacak olan EdGraph referansı.
    // Bu, paketlenmiş oyuna dahil edilmez.
    UPROPERTY()
    TObjectPtr<UEdGraph> EdGraph;
#endif

    // Buraya runtime'da grafiği çalıştırmak için gerekli
    // başlangıç düğümü, değişkenler vb. property'leri ekleyebilirsiniz.
};