#include "PLGRoomFactory.h"
#include "SGraphNode_PLGRoom.h"       // Oluşturacağımız GÖRSEL Sınıf
#include "EdGraphNode_PLGRoom.h"      // Kontrol edeceğimiz VERİ Sınıfı

TSharedPtr<SGraphNode> FPLGRoomFactory::CreateNode(UEdGraphNode* Node) const
{
	// Gelen node'un bizim özel RoomNode'umuz olup olmadığını Cast ile kontrol et
	if (UEdGraphNode_PLGRoom* RoomNode = Cast<UEdGraphNode_PLGRoom>(Node))
	{
		// Eğer doğru tipteyse, onun için yeni bir SGraphNode_PLGRoom widget'ı oluştur ve döndür.
		// Bu SNew çağrısı, SGraphNode_PLGRoom::Construct fonksiyonunu tetikler.
		return SNew(SGraphNode_PLGRoom, RoomNode);
	}

	// Eğer bizim nodumuz değilse, null döndürerek sırayı zincirdeki diğer fabrikalara bırak.
	return nullptr;
}