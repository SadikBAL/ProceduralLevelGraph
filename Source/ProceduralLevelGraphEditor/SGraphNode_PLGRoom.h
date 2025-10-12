#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class UEdGraphNode_PLGRoom;

/**
 * UEdGraphNode_PLGRoom'un görsel temsilini (visual representation) yöneten Slate widget'ı.
 * Bu sınıf, pinleri varsayılan sol/sağ düzeni yerine nodun dört bir yanına yerleştirmek için
 * özel bir layout oluşturur.
 */
class SGraphNode_PLGRoom : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNode_PLGRoom) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode_PLGRoom* InNode);

protected:
	virtual void CreatePinWidgets() override;
	TSharedPtr<SNodeTitle> NodeTitle;
	// Pinlerin yerleştirileceği konteyner widget'ları için pointer'lar
	TSharedPtr<SHorizontalBox> TopPinBox;
	TSharedPtr<SHorizontalBox> BottomPinBox;
	TSharedPtr<SVerticalBox> LeftPinBox;
	TSharedPtr<SVerticalBox> RightPinBox;
};