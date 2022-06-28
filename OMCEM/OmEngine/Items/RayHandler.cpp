#include "RayHandler.h"
#include "OMCEM.h"
#include "OmEngine/Controllers/Observer.h"
#include "OmEngine/Controllers/MainCont.h"
#include "OmEngine/Controllers/Process.h"
#include "OmActor.h"
#include "OmEngine/Utils/OmUtils.h"
#include "../Controllers/MoveCont.h"
#include "../Components/OmComp.h"



void ARayHandler::BeginPlay()
{
	Super::BeginPlay();

	TArray<USceneComponent*> listChilds;
	RootComponent->GetChildrenComponents(false, listChilds);
	for (USceneComponent* c : listChilds) {
		if (c->GetName() == "Line") compLine = c;
		if (c->GetName() == "HitMark") compHitMark = c;
	}

	if (compLine)
	{
		compLine->SetWorldScale3D(FVector(Thickness, Thickness, RayLen));
		compLine->SetRelativeLocation(FVector(RayLen/2,0,0));
	}
	if (compHitMark) compHitMark->SetVisibility(false);
	compLine->SetVisibility(false);
	updateShowLinesStatus();
}








void ARayHandler::SetEnable(bool _isEnable)
{
	Debug("ARayHandler(" + RootComponent->GetAttachParent()->GetName()+ ") " + RootComponent->GetAttachParent()->GetOwner()->GetName() + "SetEnable: ", _isEnable);
	isEnableRayHandler = _isEnable;

	updateShowLinesStatus();


	if (!isEnableRayHandler)
	{
		ShowLine(false);
		if (compHitMark) compHitMark->SetVisibility(false);
	}
	
}

void ARayHandler::Tick(float DeltaSeconds)
{
	if (!isEnableRayHandler) return;
	Super::Tick(DeltaSeconds);


	TArray <FContactResult> _listSelected;

	if (ShowLineRule == EShowRayLineRule::SHOW_WHEN_RAY_HIT)
	{
		if (compHitMark) compHitMark->SetVisibility(false);
		ShowLine(false);
	}


	FVector rayStart = this->GetActorLocation();
	FVector rayEnd = rayStart + (this->GetActorForwardVector() * RayLen);
	//FVector normal = (rayEnd - rayStart).GetSafeNormal();
	FVector normal = this->GetActorForwardVector().GetSafeNormal();
	TArray<FHitResult> lisRayResult = UOmUtils::RayCast(IsMultiCast, RayChannel, rayStart, rayEnd, 0, FColor::Black);
	//if(ShowLineRule == EShowRayLineRule::ALWAYS_SHOW) UOmUtils::DrawLine(rayStart, rayEnd, FColor::Black, false);
	
	int8 i = 0;
	for (FHitResult hitResult : lisRayResult)
	{
			
		FVector _loc = hitResult.Location;
		compHitMark->SetWorldLocation(_loc);

		
		//if(hitResult.GetComponent())
		if (checkComp( hitResult.GetComponent()))
		{
			FString idTo =  hitResult.GetComponent()->GetName();
				if (UObserver::INS && AMainCont::INS->IsAppStarted) UObserver::INS->Send(FOmValue("RayFrom:(Cam),RayTo:(" + idTo + ")"), false);
			FContactResult contact = FContactResult::Create(hitResult.GetComponent());
			contact.HitLoc = _loc;
			float _totalSize = (rayEnd - rayStart).Size();
			float _hitPointSize = (hitResult.Location - rayStart).Size();
			contact.RayHitRatio = 1 - (_hitPointSize / _totalSize);
			contact.HitLoc = hitResult.Location;
			contact.RayHitNormal = normal.GetSafeNormal();
			_listSelected.Add(contact);
			Debug("rayhit[" + FString::FromInt(i) + "](" + RootComponent->GetAttachParent()->GetName() + ") " + RootComponent->GetAttachParent()->GetOwner()->GetName() + "#hitActor: " + hitResult.Actor->GetName() + ", hitcomp : " + idTo, (int8) 2);

			

			if (ShowLineRule == EShowRayLineRule::SHOW_WHEN_RAY_HIT)
			{
				//if (compHitMark) compHitMark->SetVisibility(true);
				ShowLine(true);
			}
		}
		i++;

	}

	{//listede olup, gecici listede olmayanlari siliyoruz
		for (int8 i = 0; i < listHitResult.Num(); i++)
		{
			FContactResult result = listHitResult[i];
			
			if (!getContainsInList(_listSelected, result.toComp))
				removeSelected(result);
		}
	}
	{//gecici listede olup, listede de olanlari guncellle, olmayanlar icin yeni ekliyoruz
		for (int8 i = 0; i < _listSelected.Num(); i++)
		{
			FContactResult result = _listSelected[i];
			if (!getContainsInList(listHitResult, result.toComp))
				addSelected(result);
			else
				updateSelected(result);
		}
	}

}


bool ARayHandler::getContainsInList(TArray<FContactResult> _list, USceneComponent* _comp)
{
	bool result = false;
	for (int8 i = 0; i < _list.Num(); i++)
	{
		FContactResult r = _list[i];
		if (r.toComp == _comp)
			result = true;
	}
	return result;
}

TArray<UOmComp*> ARayHandler::GetHitComps()
{
	TArray < UOmComp* > _list;
	for (FContactResult r : listHitResult)
	{
		_list.Add(Cast<UOmComp>(r.toComp));
	}
	return _list;
}

TArray<FContactResult> ARayHandler::GetHitResult()
{
	return listHitResult;
}

void ARayHandler::ShowLine(bool _status, bool _force)
{
	if (!IsEnable) return;
	bool isShow = false;;
	if (_status)
	{
		if ((ShowLineRule != EShowRayLineRule::ALWAYS_HIDE || _force) )
			isShow = true;
	}
	
	if (compLine)
	{

		compLine->SetVisibility(isShow);

	}
	
}
void ARayHandler::ShowMark(bool _status)
{
	if (!IsEnable) return;
	compHitMark->SetVisibility(_status);
}

void ARayHandler::updateShowLinesStatus()
{
	

		if(ShowLineRule == EShowRayLineRule::ALWAYS_SHOW)
			ShowLine(true);
		
	

}



void ARayHandler::addSelected(FContactResult _result)
{
	if (!getContainsInList(listHitResult, _result.toComp))
	{
		listHitResult.Add(_result);
		

		////UOmComp* omComp = Cast<UOmComp>(_result.toComp);
		UPrimitiveComponent* primComp = Cast<UPrimitiveComponent>(_result.toComp);
		if (primComp)
		{
			AOmActor* owner = Cast<AOmActor>(primComp->GetOwner());
			AMoveCont* move = Cast<AMoveCont>( RootComponent->GetAttachParent()->GetOwner());
			//if (owner) 	owner->BpEventRun("cmd:(RollOver),comp:(" + primComp->GetName() + ")");
			if (owner) 	 owner->OnSelect(FSelectHandler(ESelectHandlerAction::ROLL_OVER, RayChannel, primComp, move));
			EventSelected.Broadcast(true, primComp);

		}
	
		
		
	}
}

void ARayHandler::updateSelected(FContactResult _result)
{
	if (getContainsInList(listHitResult, _result.toComp))
	{
		int8 foundIndex = -1;
			for (int8 i = 0; i < listHitResult.Num(); i++)
			{
				FContactResult r = listHitResult[i];
				if (r.toComp == _result.toComp)
				{
					foundIndex = i;
				}
			}
		
			if (foundIndex > -1)
				listHitResult[foundIndex] = _result;


	}
}

void ARayHandler::removeSelected(FContactResult _result)
{
	if (getContainsInList(listHitResult, _result.toComp))
	{
		//listHitResult.Remove(_result);
		
			UOmComp* omComp = Cast<UOmComp>(_result.toComp);
			if (omComp)
			{
				AOmActor* owner = Cast<AOmActor>(omComp->GetOwner());
				AMoveCont* move = Cast<AMoveCont>(RootComponent->GetAttachParent()->GetOwner());
				//if (owner) 	owner->BpEventRun("cmd:(RollOut),comp:(" + omComp->GetName() + ")");
				if (owner) 	 owner->OnSelect(FSelectHandler(ESelectHandlerAction::ROLL_OUT, RayChannel, omComp,move));
				EventSelected.Broadcast(false, omComp);
			}
			
			int8 removeIndex = -1;
			for (int8 i = 0; i < listHitResult.Num(); i++)
			{
				FContactResult r = listHitResult[i];
				if (r.toComp == _result.toComp)
				{
					removeIndex = i;
				}
			}

			if (removeIndex > -1) listHitResult.RemoveAt(removeIndex);
	}
}

bool ARayHandler::checkComp(UPrimitiveComponent* _primComp)
{
	bool r = true;
	if (GetOwner() == _primComp->GetOwner()) r = false;
	if (_primComp->GetOwner()->GetRootComponent()->GetAttachParent())
	{
		if (_primComp->GetOwner()->GetRootComponent()->GetAttachParent()->GetOwner() == GetRootComponent()->GetAttachParent()->GetOwner())
		{

			r = false;
		}

	}
		
	UOmComp* omComp = Cast<UOmComp>(_primComp);
	AItem* owner = Cast<AItem>(_primComp->GetOuter());
	if (omComp)
		if (!omComp->GetEnable()) r = false;
		
		
	if(owner)
		if (!owner->GetEnable()) r = false;


	return r;
}