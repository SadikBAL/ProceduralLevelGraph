#pragma once

#include "CoreMinimal.h"
#include "ConnectionDrawingPolicy.h"
#include "EdGraph/EdGraphSchema.h"
#include "ScopedTransaction.h"
#include "ProceduralLevelGraphEditor/Node/Data/MazeGraphNodeBase.h"
#include "MazeGraphSchema.generated.h"

class UEdGraphNode_PLGBase;

UENUM()
enum FEnumPinType : int
{
    Up,
    Down,
    Left,
    Right,
    None,
};

struct FPinPair
{
    UEdGraphPin* Pin1;
    UEdGraphPin* Pin2;

    FPinPair(UEdGraphPin* InPin1, UEdGraphPin* InPin2)
    {
        if (InPin1 < InPin2)
        {
            Pin1 = InPin1;
            Pin2 = InPin2;
        }
        else
        {
            Pin1 = InPin2;
            Pin2 = InPin1;
        }
    }

    bool operator==(const FPinPair& Other) const
    {
        return Pin1 == Other.Pin1 && Pin2 == Other.Pin2;
    }

    friend uint32 GetTypeHash(const FPinPair& InPair)
    {
        return HashCombine(GetTypeHash(InPair.Pin1), GetTypeHash(InPair.Pin2));
    }
};

USTRUCT()
struct FPLGGraphSchemaAction_NewNode : public FEdGraphSchemaAction
{
    GENERATED_BODY();

    FPLGGraphSchemaAction_NewNode() : NodeTemplate(nullptr) {}

    FPLGGraphSchemaAction_NewNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
        : FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr) {}
    
    //virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
    virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override;
    UPROPERTY()
    TObjectPtr<UEdGraphNode> NodeTemplate;
};

USTRUCT()
struct FPLGGraphSchemaAction_PasteMirror : public FEdGraphSchemaAction
{
    GENERATED_BODY();

    FPLGGraphSchemaAction_PasteMirror() : MirrorDirection(EMazeOrientation::Horizontal) {}

    FPLGGraphSchemaAction_PasteMirror(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, EMazeOrientation InDirection)
        : FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), MirrorDirection(InDirection) {}
    
    //virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
    virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override;
    UPROPERTY()
    EMazeOrientation MirrorDirection;;
};

USTRUCT()
struct FPLGGraphSchemaAction_Paste : public FEdGraphSchemaAction
{
    GENERATED_BODY();

    FPLGGraphSchemaAction_Paste() {}

    FPLGGraphSchemaAction_Paste(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
        : FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}

    virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override;
};

class FPLGConnectionDrawingPolicy : public FConnectionDrawingPolicy
{
public:
    FPLGConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj);
    
    virtual void DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, /*inout*/ FConnectionParams& Params) override;
    virtual void DrawConnection(int32 LayerId, const FVector2f& Start, const FVector2f& End, const FConnectionParams& Params) override;
    virtual void DrawSplineWithArrow(const FVector2f& StartPoint, const FVector2f& EndPoint, const FConnectionParams& Params) override;
    virtual void DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom, const FConnectionParams& Params) override;
private:
    TSet<FPinPair> DrawnConnections;
    
};

UCLASS()
class UMazeGraphSchema : public UEdGraphSchema
{
    GENERATED_BODY()
public:
    virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
    virtual class FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
};