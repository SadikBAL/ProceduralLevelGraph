#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"

/**
 * Bu fabrika sınıfı, UEdGraphNode_PLGRoom veri objelerini
 * SGraphNode_PLGRoom görsel widget'larına dönüştürmekle görevlidir.
 */
class FPLGRoomFactory : public FGraphPanelNodeFactory
{
public:
	// FGraphPanelNodeFactory arayüzünden override edilen fonksiyon.
	// UE 5.4'te bu fonksiyonun adı CreateNodeWidget'tir.
	virtual TSharedPtr<class SGraphNode> CreateNode(class UEdGraphNode* Node) const override;
};