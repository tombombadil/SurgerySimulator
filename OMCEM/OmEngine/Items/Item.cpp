#include "Item.h"
#include "OMCEM.h"
#include "OmEngine/Components/OmComp.h"
#include "OmEngine/Utils/OmUtils.h"
#include "OmEngine/Controllers/Observer.h"
#include "OmEngine/Controllers/Process.h"
#include "OmEngine/Components/ProcessComp.h"
#include "../Utils/OmValue.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	IsCLone = false;
}

FString AItem::Create(int _index)
{
	Index = _index;
	if (ItemName.IsValidIndex(0))
		Id = ItemName;
	else
		Id = FString::FromInt(Index);

	if(!IdCat.IsValidIndex(0)) IdCat = Id;
	return Id;
}


void AItem::Init()
{
	//if (!IsEnable || IsInited) return;
	if (IsInited) return;
	if (ItemName.Len() < 1) ItemName = GetName();
	GetComponents(listOmComps,true);
	
	if (listOmComps.Num() > 0)
	{
		for (UOmComp* omComp : listOmComps)
		{
		
			if (omComp->GetOwner() != this) break; //omComp is should not be other actor (childActor) component 
			if (omComp->GetName() == "Rel") CompRel = omComp;
			if (omComp->GetName() == "Tip") CompTip = omComp;
			if (omComp->GetName() == "Volume") 
			{
				CompVol = omComp;
			}
			
			if(!IsEnable) omComp->SetEnable(false);
			omComp->Init();
		}
	}

	TArray< UOmWidgetComp*> listOmWidgetComps;;
	GetComponents(listOmWidgetComps, true);

		for (UOmWidgetComp* widgetComp : listOmWidgetComps)
		{
		
			widgetComp->Init();
		}

	

	InitTrans = GetTrans();
	if(RootComponent->GetAttachParent()) initParentActor = RootComponent->GetAttachParent()->GetOwner();
	SaveRelTrans("initRel");
	SaveTrans("init");
	Reset();

	if (IsHidden())
	{
		SetVisible(false, true);
	}

	Debug(Id + ".Init()");
	createListStaticMeshMaterials();

	IsInited = true;
	//SetMaterial(UResource::INS->MAT_HIGHLIGHTTOOL);
}


void AItem::Start()
{
	BpEventStart();

	for (UOmComp* c : listOmComps)
		c->BpEventStart();
}


void AItem::StartOperation()
{
	BpEventStartOperation();

	for (UOmComp* c : listOmComps)
		c->BpEventStartOperation();
}

void AItem::Tick(float DeltaTime)
{
	if (!IsEnable) return;
	for (UOmComp* omComp : listOmComps)
	{
		if(omComp) 	omComp->Render(DeltaTime);
	}

	if (IsCatchCollision)
	{
		checkCollision();
	}
	
	//Debug(Id + ".compPivot tick  relative :" + RootComponent->GetRelativeTransform().ToString());
	Super::Tick(DeltaTime);
}




void AItem::Run(FOmValue cmd)
{
	Debug("		"+Id + " Run --> : " + cmd.ToString());
	FString strCmd = cmd.ToString();

	BpEventRun(strCmd);

	FString func = cmd.GetValue("func").GetString(0);

	if (func == "SetEnable")
	{
		SetEnable(true);
	}
		
	else if (func == "SetDisable")
	{
		SetEnable(false);

	}
	if (func == "Show")
	{
		SetVisible(true, true);
	}

	else if (func == "Hide")
	{
		SetVisible(false, true);
	}
		
	
		
	//UMoveAgent::INS->Run(cmd);
}



void AItem::UpdateParams(FOmValue _value, bool sendToObserver)
{
	if (_value.IsValid)
	{
		params.Add(_value,true);
		Debug(Id + " UpdateParams keyparam :" + _value.ToString());
	}
	if (sendToObserver)
		SendParams(_value);
}

void AItem::SendParams(FOmValue _value)
{
	/*_value.Add(
		"Btn1Press:(" + FString::FromInt((int)APlayerCont::INS->IsHapticButton1Press) +
		"),Btn2Press:(" + FString::FromInt((int)APlayerCont::INS->IsHapticButton2Press) +
		"),BtnTrigger:(" + FString::FromInt((int)APlayerCont::INS->BtnTrigger) + ")",
		true);*/
	UObserver::INS->Send(_value);
}

FString AItem::GetParam(FString _key)
{
	FString r = "";
	if( _key.IsValidIndex(0))
	r = params.GetValue(_key).GetString(0);
	return r;
}




void AItem::UpdateParams(FString _strParams, bool sendToObserver)
{
	UpdateParams(FOmValue(_strParams), sendToObserver);
}


AItem* AItem::GetChildItem(int8 _index/*=0*/)
{
	AItem* i = 0;
	if (listChildItems.Num() > _index)
	{
		i = Cast<AItem>( Children[_index]);
	}
	return i;
}



void AItem::SetCollisionEnabled(bool _status)
{
}


void AItem::SetVisible(bool isVisible, bool applyallChildActors)
{
	
	
	RootComponent->SetVisibility(isVisible, true);

	if (applyallChildActors)
	{
		TArray<AActor*> listAllChilds;
		GetAttachedActors(listAllChilds);
		for (AActor* a : listAllChilds)
		{
			AItem* item = Cast<AItem>(a);
			if(item) item->SetVisible(isVisible, true);
			else { //for nonitem
				a->GetRootComponent()->SetVisibility(isVisible, true);
				TArray<AActor*> listAllChildsInOtherActor;
				a->GetAttachedActors(listAllChildsInOtherActor);
				for (AActor* ac : listAllChildsInOtherActor)
				{
					ac->GetRootComponent()->SetVisibility(isVisible, true);

				}
			}
		}
	}

}

bool AItem::GetVisible()
{

	return RootComponent->IsVisible();
	

}


void AItem::SetVisibleAllCompsByTag(bool _visiblity, FString _tag, bool _dontVisibleHideComps, bool _includeAllChilds)
{

	TArray<USceneComponent*>  list = getVisiblityCompsList(_visiblity, RootComponent, _dontVisibleHideComps, _includeAllChilds);
	for (USceneComponent* comp : list)
	{
		if (comp->ComponentHasTag(*_tag))
		{
			comp->SetVisibility(_visiblity);
		}
			
	}
}
void AItem::SetVisibleAllCompsByParentComp(bool _visiblity, USceneComponent* _root, bool _dontVisibleHideComps, bool _includeAllChilds)
{

	TArray<USceneComponent*>  list = getVisiblityCompsList(_visiblity, _root, _dontVisibleHideComps, _includeAllChilds);
	for (USceneComponent* comp : list)
	{
		comp->SetVisibility(_visiblity);
	}

}
TArray<USceneComponent*> AItem::getVisiblityCompsList(bool _visiblity, USceneComponent* _root, bool _dontVisibleHideComps, bool _includeAllChilds)
{
	TArray<USceneComponent*> list;
	_root->GetChildrenComponents(_includeAllChilds, list);

	if (_dontVisibleHideComps)
	{
		if(storeWillNotVisibleComps.IsValidIndex(0))
		{
			list.Empty(0);
			list = storeWillNotVisibleComps;
		}


		
	}



		
		//GetComponents(allComps, _includeAllChilds);
		for(USceneComponent* comp : list)
		{
			if (!_visiblity && _dontVisibleHideComps)
			{
				if (!storeWillNotVisibleComps.Contains(comp) && !comp->IsVisible())
				{
					storeWillNotVisibleComps.Add(comp);
				}
					
			}

		
				
		}

		return list;
	
}
void AItem::checkCollision()
{
	/*FContactResult* contactResult = 0;
	for (UOmComp* omComp : listOmComps)
	{
		if (omComp->CollisionEnter)
		{
			OnCollision(true, omComp->CollisionEnter);
			omComp->CollisionEnter = 0;
		}
		if (omComp->CollisionExit)
		{
			OnCollision(false, omComp->CollisionExit);
			omComp->CollisionExit = 0;
		}

		if (omComp->CollList)
		if (omComp->CollList->GetIsValid())
		{
			for (FContactResult* contact : omComp->CollList->List)
				OnCollisionContinues(contact);
		}
	}*/
}
//todo ray
//void AItem::OnCollision(bool _status, FContactResult* _result)
//{
//	UOmComp* fromComp = Cast<UOmComp>(_result->fromComp);
//	UOmComp* toComp = Cast<UOmComp>(_result->toComp);
//	if (_status)
//	{
//	
//		//ebug(Id+"	.OnCollision : " + _result->toString());
//		if (fromComp && toComp)
//		BpEventOnColl(fromComp, toComp, _result->toItem, FVector(0, 0, 0));
//	}
//	else
//	{
//		//ebug(Id+"	.OnCollisionExit : " + _result->toString());
//		if (fromComp && toComp)
//		BpEventOnCollRemove(fromComp, toComp, _result->toItem);
//	}
//
//	
//}
//
//
//void AItem::OnCollisionContinues(FContactResult* _result)
//{
//	
//
//}

void AItem::SetTrans(PairTrans _pawnTrans)
{
	if (RootComponent->Mobility != EComponentMobility::Static)
	{
		FTransform t1 = _pawnTrans.Key;
		FTransform t2 = _pawnTrans.Value;
		SetActorLocation(t1.GetLocation(), false);
		SetActorRotation(t1.GetRotation().Rotator());

		if (CompRel)
		{
			CompRel->SetRelativeLocation(t2.GetLocation(), true);
			CompRel->SetRelativeRotation(t2.GetRotation().Rotator());
		}

		
	}

}

void AItem::SetTransWorld(FTransform _trans)
{
	PairTrans t = GetTrans();
	t.Key = _trans;
	SetTrans(t);
}



PairTrans AItem::GetTrans()
{
	PairTrans t;
	FTransform t1;
	FTransform t2;
	t1 = GetActorTransform();

	if (CompRel)
	{
		t2 = CompRel->GetRelativeTransform();
	}
	t.Key = t1;
	t.Value = t2;
	return t;
}

FTransform AItem::GetTransWorld()
{
	PairTrans p = GetTrans();
	return p.Key;
}

void AItem::SetItemLoc(FVector _location, bool _isWorld, FString _useCompAsPivotPoint)
{
	FVector addLocFromPivot = FVector::ZeroVector;
	if (_useCompAsPivotPoint.IsValidIndex(0))
	{
		
			UOmComp* compPivot = Cast<UOmComp>(GetComponent(_useCompAsPivotPoint));
			if (compPivot)
			{
				addLocFromPivot = compPivot->GetRelativeTransform().GetLocation() * compPivot->GetComponentTransform().GetScale3D();
				Debug(Id +".compPivot " + compPivot->Id + "  compPivot->GetRelativeTransform().GetLocation() "+ compPivot->GetRelativeTransform().GetLocation().ToString() + " relative :" + RootComponent->GetRelativeTransform().ToString());
			}
		
	}

	if(_isWorld)
		SetActorLocation(_location - addLocFromPivot);
	else
		SetActorRelativeLocation(_location - addLocFromPivot);
	
	Debug(Id + ".compPivot2  relative :" + RootComponent->GetRelativeTransform().ToString());


}

void AItem::SetRot(FRotator _rotation, bool _isWorld)
{
	PairTrans p = GetTrans();
	FTransform t;
	if (_isWorld)
	{
		t = p.Key;
		t.SetRotation(_rotation.Quaternion());
		p.Key = t;
	}
	else 
	{
		t = p.Value;
		t.SetRotation(_rotation.Quaternion());
		p.Value = t;
	}
	SetTrans(p);

}





void AItem::SetCollisionPreset(FString _presetName)
{
	for (UOmComp* omComp : listOmComps)
	{
		omComp->SetCollisionProfileName(FName(*_presetName));
	}
	

}
void AItem::ResetCollisionPreset()
{
	AItem();
	for (UOmComp* omComp : listOmComps)
	{
		omComp->ResetCollisionPreset();
	}

}


UOmComp* AItem::GetCompByName(FString _id)
{
	UOmComp* r = 0;
	for (UOmComp* omComp : listOmComps)
	{

		if (omComp->GetOwner() == this)
		{
			if (omComp->GetName() == _id) r = omComp;
		}
		
	}
	return r;
}


TArray<UOmComp*> AItem::GetComps()
{
	
	return listOmComps;
}


AItem* AItem::GetChildItemsByCatId(FString _idCat)
{
	AItem* r = 0;
	TArray<AActor*> listAllActors;
	
	GetAttachedActors(listAllActors);
	for (AActor* a : listAllActors)
	{
		AItem* item = Cast<AItem>(a);
		if (item)
		{
			if (item->IdCat == _idCat)
			{
				r = item;
			}
		}

	}

	//UChildActorComponent* caComp = Cast<UChildActorComponent>(GetComponent(_compName));
	//if (caComp) actor = Cast<AOmActor>(caComp->GetChildActor());
	return r;
}



/* materials */
void AItem::createListStaticMeshMaterials()
{
	TArray<UMeshComponent*> allMeshComp;
	GetComponents(allMeshComp, true);
	for (UMeshComponent* meshComp : allMeshComp)
	{
		if(Id == "LamL3")
			Debug("");
		meshComp->SetAbsolute(false, false, false);
		TArray<UMaterial*> listMats;
		for (int8 m = 0; m < meshComp->GetNumMaterials(); m++)
		{
			if (meshComp->GetMaterial(m)) listMats.Add(meshComp->GetMaterial(m)->GetMaterial());
		}
		listStaticMeshComps.Add(meshComp, listMats);
	}
}


void AItem::SetMaterial(UMaterial* _newMat)
{
	if (!_newMat) return;
	UMaterialInterface* newMat = (UMaterialInterface*)_newMat;
	for (auto& pair : listStaticMeshComps)
	{
		UMeshComponent* meshComp = pair.Key;
		for (int8 m = 0; m < meshComp->GetNumMaterials(); m++)
		{
			meshComp->SetMaterial(m, newMat);
			
		}
		
	}
}

void AItem::SetComponentMat(FString _MaterialId, FString _ComponentName)
{
	for (FComponentMaterial compMat : ComponentMaterials)
	{
		if (compMat.ComponentName == _ComponentName)
		{
			if (compMat.Materials.Contains(_MaterialId))
			{
				UPrimitiveComponent* meshComp = Cast< UPrimitiveComponent>( GetComponent(_ComponentName));
				if (meshComp)
				{
					UMaterialInterface* mat = (UMaterialInterface*)compMat.Materials[_MaterialId];
					for (int8 m = 0; m < meshComp->GetNumMaterials(); m++)
					{
						meshComp->SetMaterial(m, mat);

					}
				}
			}
		}
	}
}

TArray<UPrimitiveComponent*> AItem::FindComponentsByMatId(FString _MaterialId)
{
	TArray<UPrimitiveComponent*> listResult;
	for (FComponentMaterial compMat : ComponentMaterials)
	{
		for (auto It = compMat.Materials.CreateConstIterator(); It; ++It)
		{
			FString matId = It.Key();
			UMaterialInterface* mat = It.Value();
			if (matId == _MaterialId)
			{
				UPrimitiveComponent* meshComp = Cast<UPrimitiveComponent>(GetComponent(compMat.ComponentName));
				if (meshComp)
					listResult.Add(meshComp);
			}

				
		}
	}
	return listResult;
}
void AItem::ResetCompMat(FString _ComponentName)
{
	UMeshComponent* meshComp = Cast<UMeshComponent>(GetComponent(_ComponentName));
	if (listStaticMeshComps.Contains(meshComp))
	{
		TArray<UMaterial*> listMats = listStaticMeshComps[meshComp];
		for (int8 m = 0; m < listMats.Num(); m++)
		{

			UMaterialInterface* mat = (UMaterialInterface*)listMats[m];
			meshComp->SetMaterial(m, mat);
		}
	}
	
}



void AItem::ResetMaterial()
{
	for (auto& pair : listStaticMeshComps)
	{
		UMeshComponent* meshComp = pair.Key;
		TArray<UMaterial*> listMats = pair.Value;
		for (int8 m = 0; m < listMats.Num(); m++)
		{

			UMaterialInterface* mat = (UMaterialInterface*)listMats[m];
			meshComp->SetMaterial(m, mat);
		}

	}
}




/* save & restore */
void AItem::SaveTrans(FString _id)
{
	FString id = _id.ToLower();
	if (!listSavedTrans.Contains(id))
		listSavedTrans.Add(id, GetActorTransform());
	else
		listSavedTrans[id] = GetActorTransform();
}

FTransform AItem::GetSavedTrans(FString _id)
{
	FTransform _trans;
	FString id = _id.ToLower();
	if (listSavedTrans.Contains(id))
		_trans = listSavedTrans[id];
	return _trans;
}

void AItem::SaveRelTrans(FString _id)
{
	if (!listSavedTrans.Contains(_id))
		listSavedTrans.Add(_id, RootComponent->GetRelativeTransform());
}

FTransform AItem::RestoreTrans(FString _id)
{
	FTransform t;
	FString id = _id.ToLower();
	if (listSavedTrans.Contains(id))
	{
		t = listSavedTrans[id];
		SetActorTransform(t);
	}
	return t;
}

FTransform AItem::RestoreRelTrans(FString _id)
{
	FTransform t;
	FString id = _id.ToLower();
	if (listSavedTrans.Contains(id))
	{
		t = listSavedTrans[id];
		SetActorRelativeTransform(t);
	}
	return t;
}



AItem* AItem::CloneIt()
{
	FActorSpawnParameters Parameters;

	Parameters.Template = this;

	class AItem* clone = GetWorld()->SpawnActor<class AItem>(GetClass(), Parameters);
	clone->IsCLone = true;
	clone->SetEnable(false);

	return clone;
}



void AItem::Reset()
{
	BpEventRun("func:(Reset)");

	//for (AItem* item : listConnectedItems)
	//{
		//item->RestoreRelTrans("initRel");
	//}

}


void AItem::PreDestroy()
{

}
