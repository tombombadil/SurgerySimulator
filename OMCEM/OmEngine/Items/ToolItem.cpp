#include "ToolItem.h"
#include "OMCEM.h"
#include "OmEngine/Controllers/MainCont.h"
#include "OmEngine/Controllers/Observer.h"
#include "FieldItem.h"
#include "OmEngine/Controllers/TaskCont.h"
#include "OmEngine/Components/ProcessComp.h"
#include "../Controllers/ToolsCont.h"
#include "../Components/CheckOutOfMeshComp.h"
#include "../Controllers/PlayerCont.h"
#include "../Components/ContainerComp.h"



AToolItem::AToolItem()
{
	PrimaryActorTick.bCanEverTick = true;
	IsCatchCollision = true;
}


void AToolItem::Init()
{
	AItem::Init();
	isToolActive = false;
	SetActiveTool(false);
	isPawn = false;

	for (UOmComp* omComp : listOmComps)
	{
		omComp->SetEnableGravity(false);
	}

	CompContainer = Cast<UContainerComp>(GetComponentByClass(UContainerComp::StaticClass()));
	
}




void AToolItem::Tick(float DeltaTime)
{
	for (UOmComp* omComp :  listOmComps)
	{
		if (Cast<UCheckOutOfMeshComp>(omComp)) 	Cast<UCheckOutOfMeshComp>(omComp)->Render(DeltaTime);
	}

	if (!IsEnable && AMainCont::INS->IsAppStarted) return;
	AItem::Tick(DeltaTime);

	

}






/* ________________________________ SetProcess  ____________________________________ * */
void AToolItem::SetProcess(bool _status, UProcessComp* _compPawn,  FOmValue _params)
{
	IsOnProcess = _status;
	if (IsOnProcess)
	{
		BpEventStartProcess();
	}
	else
	{
		BpEventEndProcess();
	}
	
}




/* ________________________ TRANSFORMS _________________________ */
void AToolItem::SetTrans(PairTrans _pawnTrans) //@override
{
	Super::SetTrans(_pawnTrans);
}





PairTrans AToolItem::GetTrans()
{
	PairTrans t;
	FTransform t1 = GetActorTransform();
	FTransform t2;


	if (CompRel)
	{
		t2 = CompRel->GetRelativeTransform();
	}
	t.Key = t1;
	t.Value = t2;
	return t;
}



/* ________________________ set selectable _________________________ */
void AToolItem::SetSelectable(bool _selectable)
{
	FString compRayCbCollProfile = _selectable ? "Selected" : "NoCollision";
	UPrimitiveComponent* compRayCb = Cast< UPrimitiveComponent>(GetComponent("RayCallback"));
	if (compRayCb)
	{
		compRayCb->SetCollisionProfileName(FName(*compRayCbCollProfile)); //to-do : calismiyor!
		//compRayCb->IsEnable = _selectable;
		SetEnable(_selectable);
	}
		

}

/* ________________________ set active _________________________ */
void AToolItem::SetActiveTool(bool _IsActive, bool _putToToolset)
{

	SetSelectable(!_IsActive);
	isToolActive = _IsActive;
}

bool AToolItem::GetIsToolActive()
{
	return isToolActive;
}





FVector AToolItem::GetTipPoint(bool _isWorld)
{
	FVector p = FVector::ZeroVector;
	if (CompTip)
	{
		
		p = CompTip->GetComponentLocation() - GetActorLocation();
	}
		
	
	if (_isWorld)
	{
		p += GetActorLocation();
	}
	return p;
}






/* ______________________ # Cuurent Part  _______________________ */

void AToolItem::SetCurrPart(AToolItem* _toolPart)
{
	CurrPart = _toolPart;
	FString id = CurrPart ? CurrPart->Id : "0";

	UpdateParams("CurrPart:("+ id +")");
	if (!CurrPart)
		Debug("currpart null");

}
AToolItem*  AToolItem::GetCurrPart()
{
	return CurrPart;
}

/* ______________________ # Take / Put _______________________ */
void AToolItem::TakePartInField(AFieldItem* _field)
{
	
	
	AItem* part = _field->TakePart();
	if (part)
	{
		HoldItem(part );

		UpdateParams(FOmValue("isHold:(1)"));
	}
}


void AToolItem::PutPartInField(AFieldItem* _field)
{
	//AItem* _part = childItem;
	if (CurrPart && _field)
	{
		ReleaseItem(false);
		_field->PutPart(CurrPart);
		UpdateParams(FOmValue("isHold:(0)"));
	}
}

/* ______________________ # hold /release _______________________ */
void AToolItem::HoldItem(AItem* _item)
{

	/*if (AMainCont::INS->IsOperationStarted)
	{
		if (AUICont::INS->GetCurrSelectedEqOnDesk())
		{
			_item = AUICont::INS->GetCurrSelectedEqOnDesk();
		}
	}*/

		
	//if (UToolsCont::INS->ConnectItems(true, this, _item, FOmValue("ConnectPoint:(ConnectPoint),IsSnapTarget:(1)")))
	//if (UToolsCont::INS->ConnectItems( _item->Id, this->Id, FOmValue("IsSnapTarget:(1),ConnectPoint:(ConnectPoint)")))
	if(Cast<AToolItem>(_item))
	{
		APlayerCont::INS->TakeTool("MoveR", Cast<AToolItem>(_item));
		childItem = _item;
		RunBpCommand("hold");

	}

}

AItem* AToolItem::ReleaseItem(bool _disableChild)
{
	AItem* child = 0;
	
	return child;
}


PairTrans AToolItem::fromRelToWorldOmTrans(UOmComp* _comp, PairTrans _relTrans)
{
	FTransform transRel = _relTrans.Key;
	FTransform transChildRel = _relTrans.Value;
	FTransform t1;


	TArray<USceneComponent*> allParents;
	_comp->GetParentComponents(allParents);
	FVector worldLoc = transRel.GetLocation();
	FRotator worldRot = transRel.GetRotation().Rotator();
	for (USceneComponent* parent : allParents)
	{
		if (parent->GetOwner() == _comp->GetOwner())
		{
			worldLoc = parent->GetRelativeTransform().GetLocation() + worldLoc;
			worldRot = parent->GetRelativeTransform().GetRotation().Rotator() + worldRot;

		}
	}


	t1.SetLocation(worldLoc);
	t1.SetRotation(worldRot.Quaternion());

	PairTrans result;
	result.Key = t1;
	result.Value = _relTrans.Value;
	return result;
}


