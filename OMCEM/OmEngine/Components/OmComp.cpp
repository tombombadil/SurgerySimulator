#include "OmComp.h"
#include "OMCEM.h"
#include "OmEngine/Utils/OmUtils.h"
#include "OmEngine/Items/Item.h"
#include "OmEngine/Items/ToolItem.h"
#include "OmEngine/Utils/Config.h"
#include "OmEngine/Utils/OmValue.h"
#include "OmEngine/Controllers/MainCont.h"
#include "../Controllers/Observer.h"




UOmComp::UOmComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}


FString UOmComp::Create(int8 _index, FString _id)
{
	FullName = _id;
	Index = _index;
	Id = FullName;
	return Id;
}



void UOmComp::Init()
{
	CollList = new FCollisionList();
	CollisionEnter = 0;
	CollisionExit = 0;
	

	if(!Collision)
		Collision = (UPrimitiveComponent*) this;
	
	
	/*if (Collision)
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &UOmComp::onOverlapBegin);
		Collision->OnComponentEndOverlap.AddDynamic(this, &UOmComp::onOverlapEnd);
	}*/
	collProfileName = GetCollisionProfileName().ToString();

	SaveTrans("init");
}

void UOmComp::Render(float DeltaTime)
{
	if (AMainCont::INS->DebugMode > 1)
	{
		if (!bHiddenInGame)
				SetHiddenInGame(false);
	}
	else {
		if (!bHiddenInGame)
			SetHiddenInGame(true);
	}
	
}


void UOmComp::SetStatus(EEventType _status)
{
	status = _status;
	BpEventStatus(_status);
}
EEventType UOmComp::GetStatus()
{
	return status;
}

void UOmComp::SetLoc(FVector _loc)
{
	FTransform t = GetTrans();
	t.SetLocation(_loc);
	SetTrans(t);
}

void UOmComp::SetRot(FRotator _rot)
{
	FTransform t = GetTrans();
	t.SetRotation(_rot.Quaternion());
	SetTrans(t);
}


void UOmComp::SetTrans(FTransform _trans)
{

	FHitResult hitResult;
	SetWorldTransform(_trans,false, &hitResult);
}
FTransform UOmComp::GetTrans()
{
	return GetComponentTransform();
}


void UOmComp::SetRelTrans(FTransform _trans)
{
	SetRelativeTransform(_trans);
}
FTransform UOmComp::GetRelTrans()
{
	return GetRelativeTransform();
}


void UOmComp::SetInitTrans(FTransform _trans)
{
	initTrans = _trans;
}

FTransform UOmComp::GetInitTrans()
{
	return initTrans;
}



void UOmComp::Run(FOmValue cmd)
{
	FString strCmd = cmd.ToString();
	BpEventRun(strCmd);
}



void UOmComp::SetEnable(bool _isEnable)
{
		IsEnable = _isEnable;
}

bool UOmComp::GetEnable()
{
	return IsEnable;
}



void UOmComp::SetCollisionPreset(FString _presetName)
{
	SetCollisionProfileName(FName(*_presetName));

}
void UOmComp::ResetCollisionPreset()
{
	SetCollisionProfileName(FName(*collProfileName));

}

FVector UOmComp::GetTipPoint(bool _isWorld)
{
	FVector p = FVector::ZeroVector + (GetUpVector() * GetScaledBoxExtent().Z * 1.5);
	if (_isWorld)
	{
		p += GetComponentLocation();
	}
	return p;
}

/* save & restore */
void UOmComp::SaveTrans(FString _id)
{
	FString id = _id.ToLower();
	if (!listSavedTrans.Contains(id))
		listSavedTrans.Add(id, GetTrans());
	else
		listSavedTrans[id] = GetTrans();
}


FTransform UOmComp::GetSavedTrans(FString _id)
{
	FTransform _trans;
	FString id = _id.ToLower();
	if (listSavedTrans.Contains(id))
		_trans = listSavedTrans[id];
	return _trans;
}


FTransform UOmComp::RestoreTrans(FString _id)
{
	FTransform _trans = GetSavedTrans(_id);
	SetTrans(_trans);
	return _trans;
}



FVector UOmComp::RestoreLoc(FString _id)
{
	FVector _loc;
	if (listSavedTrans.Contains(_id))
	{
		_loc = listSavedTrans[_id].GetLocation();
		SetLoc(_loc);
	}
	return _loc;
}


FVector UOmComp::GetRandomPositionInBounds()
{
	FVector _loc = GetComponentLocation();
	FVector boxExtends = GetScaledBoxExtent();
	_loc.X += FMath::RandRange(-boxExtends.X, boxExtends.X);
	_loc.Y += FMath::RandRange(-boxExtends.Y, boxExtends.Y);
	_loc.Y += FMath::RandRange(-boxExtends.Z, boxExtends.Z);
	return _loc;
}


/* private */
//void UOmComp::onOverlapBegin( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	//ebug(GetName() + ".onOverlapBegin->  OtherComp,Item :" + OtherComp->GetName() + " , loc:"+ SweepResult.ImpactPoint.ToString());
//	if (!OtherComp) return;
//	FContactResult* coll = CollList->GetCollision(OtherComp);
//	if (!coll)
//		OnCollision(true,new FContactResult(this, OtherComp));
//
//}
//
//void UOmComp::onOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (!OtherComp) return;
//	FContactResult* coll = CollList->GetCollision(OtherComp);
//	if (coll)
//		OnCollision(false, coll);
//}





void UOmComp::UpdateParams(FString _strParams, bool sendToObserver)
{
	UpdateParams(FOmValue(_strParams), sendToObserver);
}
void UOmComp::UpdateParams(FOmValue _value, bool sendToObserver)
{
	if (_value.IsValid)
	{
		params.Add(_value, true);
	}
	if (sendToObserver)
		SendParams(_value);
}

void UOmComp::SendParams(FOmValue _value)
{

	UObserver::INS->Send(_value);
}

FString UOmComp::GetParam(FString _key)
{
	FString r = "";
	if (_key.IsValidIndex(0))
		r = params.GetValue(_key).GetString(0);
	return r;
}

void UOmComp::PreDestroy()
{

}



