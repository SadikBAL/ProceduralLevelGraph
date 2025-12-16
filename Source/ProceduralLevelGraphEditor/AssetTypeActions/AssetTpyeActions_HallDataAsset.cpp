#include "AssetTpyeActions_HallDataAsset.h"

#include "ProceduralLevelGraphEditor/ProceduralLevelGraphEditor.h"
#include "ProceduralLevelGraphRuntime/ProceduralLevelGraphRuntime.h"
#include "ProceduralLevelGraphRuntime/Data/HallData.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_ProceduralLevelGraph"

FAssetTypeActions_HallDataAsset::FAssetTypeActions_HallDataAsset(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FAssetTypeActions_HallDataAsset::GetName() const
{
	return LOCTEXT("AssetTypeName", "Hall Data Asset");
} 

FColor FAssetTypeActions_HallDataAsset::GetTypeColor() const
{
	return FColor(50, 190, 50);
}

UClass* FAssetTypeActions_HallDataAsset::GetSupportedClass() const
{
	return UHallData::StaticClass();
}


uint32 FAssetTypeActions_HallDataAsset::GetCategories()
{
	return MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE