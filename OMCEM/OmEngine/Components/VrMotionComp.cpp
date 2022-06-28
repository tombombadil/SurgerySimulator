#include "VrMotionComp.h"
#include "OMCEM.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "../Controllers/PlayerCont.h"
#include "../Items/OmActor.h"
#include "../Controllers/MainCont.h"








void UVrMotionComp::Init()
{
	TArray<USceneComponent*> listChilds;
	GetChildrenComponents(true, listChilds);
	for (USceneComponent* c : listChilds)
	{
		UChildActorComponent* rayHandlerComp = Cast<UChildActorComponent>(c);
		if (rayHandlerComp) 
		{
			RayHandler = Cast<ARayHandler>(rayHandlerComp->GetChildActor());
			
		}
	}
	if(!AMainCont::INS->IsVRMode) SetVisible(false, true);
	else SetVisible(IsEnable, true);
	
	

	if (RayHandler)
	{
		//RayHandler->RayHandlerActor->EventSelected.AddDynamic(this, &UVrMotionComp::onRaySelected);
	}
	APlayerCont::INS->EventKeyAndMouse.AddDynamic(this, &UVrMotionComp::onInputKeyHandler);

}


	
void UVrMotionComp::onInputKeyHandler(bool _status, FString _keyId)
{
	if (!RayHandler) return;
	if (_keyId == "BtnTrigger")
	{
		
		for (UOmComp* comp : RayHandler->GetHitComps())
		{
			//if (_status)
			//{
				//AOmActor* item = Cast<AOmActor>(comp->GetOwner());
				//item->OnSelect(FSelectHandler(ESelectHandlerAction::CLICK, RayHandler->RayChannel, comp, 0));
			//}

			/*	if (APlayerCont::INS->BtnTrigger)
				{
					if (omComp->GetName() == "BtnExitHit")
					{
						if (GetWorld()->WorldType == EWorldType::Game)
							FGenericPlatformMisc::RequestExit(true);

					}
					if (omComp->GetName() == "BtnRestartHit")
					{

						UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
					}
				}*/
		}
		
	}

}

void UVrMotionComp::Render(float DeltaTime)
{
	if (!IsEnable) return;
	
	
	//if(RayHandler) RayHandler->ShowLine(APlayerCont::INS->KeyShowVrLaser);
	//if (APlayerCont::INS->KeyShowVrLaser) return;
	
	


	
}



void UVrMotionComp::SetVisible(bool isVisible, bool _applyAllChilds)
{
	SetVisibility(isVisible);
	if (_applyAllChilds)
	{
		TArray<USceneComponent*> listAllChilds;
		GetChildrenComponents(true, listAllChilds);
		for (USceneComponent* c : listAllChilds)
		{
			c->SetVisibility(isVisible);
		}
	}


}