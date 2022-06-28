// this class manage the spine bones which will hide and spawn the laminectomy bones and ligamenst by opetation json data


#include "LaminectomyContainer.h"
#include "OmEngine/Controllers/ToolsCont.h"
#include "OmEngine/Components/OmComp.h"

void ALaminectomyContainer::Init()
{
	Super::Init();
	containerBones = GetCompByName("Bones");
	containerLigaments = GetCompByName("Ligaments");
}

void ALaminectomyContainer::SpawnObjects(FSpineData _data)
{
	
	if (_data.ListBones.IsValidIndex(0))
	{
		for (FSpineHoleData boneData : _data.ListBones)
		{
			spawnBone(boneData.IdBone);
			AItem* boneInSpine = UResource::INS->GetItem(boneData.IdBone);
			if (boneInSpine)
				//boneInSpine->SetVisible(false, true);
				boneInSpine->SetActorHiddenInGame(true);
		}
	}


}



void ALaminectomyContainer::spawnBone(FString _lamBoneId)
{

	FLaminectomyObjects objectsStruct;

	for (FLaminectomyObjects strct : ListData)
	{
		if (strct.Id == _lamBoneId)
			objectsStruct = strct;
	}


	spawnObject(containerBones, objectsStruct.BoneClass);
	spawnObject(containerLigaments, objectsStruct.Ligament1Class);
	spawnObject(containerLigaments, objectsStruct.Ligament2Class);





}


void ALaminectomyContainer::spawnObject(UOmComp* _parent, TSubclassOf<AItem> _Class )
{
	if (_Class)
	{
		FActorSpawnParameters SpawnInfo;
		AItem* item = GetWorld()->SpawnActor<AItem>(_Class, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		if (UResource::INS->GetItem(item->ItemName, false))
		{
			item->Destroy();
			return;
		}
		

		

		UResource::INS->RegisterItem(item,true);
		UToolsCont::INS->ConnectItems(item->Id, _parent->Id, FOmValue("IsSnapTarget:(1)"));
		//item->Init();

	}

}