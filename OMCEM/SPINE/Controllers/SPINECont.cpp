#include "SPINECont.h"
#include "SPINETaskCont.h"
#include "OmEngine/Controllers/ToolsCont.h"
#include "OmEngine/Components/OmComp.h"
#include "../Components/HoleInfoComp.h"
#include "OmEngine/Utils/Server.h"
#include "OmEngine/Components/TransparentContComp.h"
#include "../Items/LaminectomyContainer.h"
#include "OmEngine/Controllers/XRayRender.h"
#include "../../OmEngine/Controllers/PlayerCont.h"


ASPINECont * ASPINECont::INS = NULL;


ASPINECont::ASPINECont():AMainCont()
{
	PrimaryActorTick.bCanEverTick = true;
	ASPINECont::INS = this;
}
//
//void ASPINECont::createControllers()
//{
//	Operation = FOperation("tlif");
//	UiCont = GetWorld()->SpawnActor<AUICont>(ClassUIController, FVector::ZeroVector, FRotator::ZeroRotator);
//	taskCont = NewObject<UTaskCont>(this, ClassTaskController);
//	resource = NewObject<UResource>(UResource::StaticClass());
//	bleedingCont = NewObject<UBleedingCont>();
//}

void ASPINECont::delayedBeginPlay()
{

	resource->EventOnStatus.AddDynamic(this, &AMainCont::onResourcesLoaded);
	resource->Load();

	spineConnector = NewObject<USpineConnector>();
	spineConnector->JsonBase = resource->GetOpJsonObj("Operation");

	server = NewObject<UServer>();
	//server->OnAllMetricResponse.AddDynamic(this, &AMainCont::onServerAllMetricResponse);
	server->INS = server;
	server->Init();

	//taskCont->Init();
}



void ASPINECont::Start()
{
	AXRayRender::INS->RefreshCaptures();
	SpineData = spineConnector->LoadSpine();
	Debug("spinedata  Operation : " + SpineData.IdOperation + " / " + SpineData.NameOperation);
	Debug("spinedata  Section : " + SpineData.IdSection + " / " + SpineData.NameSection);
	/*if(false)
	{
		d.IdOperation = "InserScrew";
		d.IdSection = "Lumbar";
		TArray<FSpineHoleData> holes;
		{
			FSpineHoleData hd;
			hd.IdBone = "L5";
			hd.IdDirection = "R";
			hd.IdScrew = "Screw4545";
			holes.Add(hd);
		}
		{
			FSpineHoleData hd;
			hd.IdBone = "L3";
			hd.IdDirection = "R";
			hd.IdScrew = "Screw4550";
			holes.Add(hd);
		}

		d.ListHoles = holes;
	}
*/
	

	if (SpineData.IdOperation.ToLower() == "insertscrew")
		initHoles(SpineData);
	else if (SpineData.IdOperation.ToLower() == "laminectomy")
	{
		ALaminectomyContainer* lamContainer = Cast< ALaminectomyContainer >( UResource::INS->GetItem("LaminectomyContainer"));
		if (lamContainer)		lamContainer->SpawnObjects(SpineData);
	}

	{//ALaminectomyContainer bazi spine bonelari gizledigi icin xray listesini belirlemek ve tekrar refreshlemek gerek
		AXRayRender::INS->DetermineHiddenItems();
		AXRayRender::INS->RefreshCaptures();
	}
	

	Super::Start();


	if (InputMode == 3) //vr
	{
		AVrMoveCont* vrRight = Cast< AVrMoveCont>(APlayerCont::INS->MoveR);
		AVrMoveCont* vrLeft = Cast< AVrMoveCont>(APlayerCont::INS->MoveL);
		if(vrRight->VrMenu && vrLeft->VrMenu)
		BpEventSetVrMenus(vrRight->VrMenu, vrLeft->VrMenu);
	}

	for (AItem* item : UResource::INS->GetItems())
	{
		if (item)
		{
			item->SaveTrans("Init");
			for (UOmComp* comp :item->GetComps())
			{
				comp->SaveTrans("init");
			}

		}
	}
	taskCont->Init();

	
	

}



void ASPINECont::initHoles(FSpineData _data)
{
	if (_data.ListBones.IsValidIndex(0))
	{
		TArray< FSpineHoleData> listHoleData = _data.ListBones;
		int8 index = 0;
		for (FSpineHoleData hData : listHoleData)
		{
			
			FSpineHoleData newData = spawnHole(hData, index);
			index++;
			if (newData.Hole)
			{
				listHoleDatas.Add(newData);
			}
		}
	}
		
}



/* spawn with fill data */
FSpineHoleData ASPINECont::spawnHole(FSpineHoleData _data, int8 _index)
{
	ASpineHoleField* hole = 0;
	FSpineHoleData newData = _data;
	if (!_data.IdBone.IsValidIndex(0) || !_data.IdDirection.IsValidIndex(0))
	{
		Debug::Warning("spinehole idBone veya idDirection hatali  hole:" + _data.IdHole);
		return newData;
	}

	FString idContainer = _data.IdBone + "/HoleInfo" + _data.IdDirection;
	UHoleInfoComp* holeInfoComp = Cast<UHoleInfoComp>(  UResource::INS->GetComp(idContainer));

	if (holeInfoComp)
	{
		FActorSpawnParameters SpawnInfo;
		hole = GetWorld()->SpawnActor<ASpineHoleField>(ClassSpineHole, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		if (hole)
		{
			hole->ItemName = "hole" + newData.IdBone + newData.IdDirection;
			UResource::INS->RegisterItem(hole);
			UToolsCont::INS->ConnectItems(hole->Id, holeInfoComp->Id, FOmValue("IsSnapTarget:(1)"));
			hole->Init();

			{//fill struct
				newData.IdHole = hole->Id;
				newData.Hole = hole;
				newData.MinAngles = holeInfoComp->Data.MinAngles;
				newData.MaxAngles = holeInfoComp->Data.MaxAngles;
				newData.MinDepth = holeInfoComp->Data.MinDepth;
				newData.MaxDepth = holeInfoComp->Data.MaxDepth;
				newData.IdScrew = holeInfoComp->Data.IdScrew;
			}

		}
	}
	else
	{
		Debug::Warning("hole container not found : " + idContainer);
	}

		


	return newData;
}





TArray< FSpineHoleData> ASPINECont::GetHoleFields()
{
	return listHoleDatas;
}



void ASPINECont::SetTransparentMode(bool _status)
{
	TArray<FString> arrItems;
	arrItems.Add("AllPatient");
	AMainCont::SetTransparentMode(_status, arrItems, "CAN_TRANSPARENT");
	

	
}