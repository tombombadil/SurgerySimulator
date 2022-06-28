#include "XRayRender.h"
#include "OMCEM.h"
#include <Components/SceneCaptureComponent2D.h>
#include "../Components/OmComp.h"
#include <TimerManager.h>
#include <Components/BoxComponent.h>
#include "MainCont.h"
#include "../Ui/Widget/OmWidgetComp.h"
#include "../../SPINE/Controllers/SPINECont.h"



AXRayRender* AXRayRender::INS = NULL;

AXRayRender::AXRayRender()
{

}

void AXRayRender::Init()
{
	INS = this;
	listXrayElements.Empty(0);
	/*for (USceneComponent* sceneComp : UResource::INS->GetComponentsByTag("XRAY"))
	{
		UPrimitiveComponent* primComp = Cast<UPrimitiveComponent>(sceneComp);
		if (primComp)
		{
			listXrayElements.Add(FXrayElement(primComp));
		}


	}*/

	for (AItem* item : UResource::INS->GetItems())
	{
		TArray<UPrimitiveComponent*> foundComps = item->FindComponentsByMatId("XRAY"); //item icinde xray idsine sahip tum componentler
		
		for (UPrimitiveComponent* pcomp : foundComps)
		{
			if (pcomp)
			{
				listXrayElements.Add(pcomp);
				
			}

		}
		

	}

	
	TArray<UActorComponent*> allComps;
	GetComponents(allComps, true);
	for (UActorComponent* actorComp : allComps)
	{
		if (actorComp->GetName() == "Right_CaptureComponent2D") captureCompRigt = Cast<USceneCaptureComponent2D>(actorComp);
		if (actorComp->GetName() == "Top_CaptureComponent2D") captureCompTop = Cast<USceneCaptureComponent2D>(actorComp);
		if (actorComp->GetName() == "Area") compXrayArea = Cast<UOmComp>(actorComp);
			
	}

	if (captureCompRigt)
	{
		captureCompRigt->bCaptureEveryFrame = false;
		captureCompRigt->bCaptureOnMovement = false;
	}
	if (captureCompTop)
	{
		captureCompTop->bCaptureEveryFrame = false;
		captureCompTop->bCaptureOnMovement = false;
	}


}


	
void AXRayRender::DetermineHiddenItems()
{
	for (UPrimitiveComponent* comp : listXrayElements)
	{
		AItem* item = Cast<AItem>(comp->GetOwner());
		if (item)
		{
			if (item->IsHidden() && item->IsEnable)
				if (!listHiddenXrayItems.Contains(item))

					listHiddenXrayItems.Add(item);
		}
		
	}
}
void AXRayRender::setElementMat(FXrayElement _elem, UMaterialInterface* _mat)
{
	if (_elem.comp)
	{
		for (int8 i = 0; i < _elem.listDefaultMats.Num(); i++)
		{

			if (_mat)
			{

				_elem.comp->SetMaterial(i, _mat);
			}
			else
			{
				if(!_elem.comp->ComponentHasTag("CAN_TRANSPARENT") || !AMainCont::INS->TransparentMode) //TODO : CAN_TRANSPARENT BU TAG DYNAMIC OLMALI
					_elem.comp->SetMaterial(i, _elem.listDefaultMats[i]);
			}
		
		}
	}
	
}
void AXRayRender::RefreshCaptures()
{
	ClearCaptures();



	applyXrayMats();
	
		for (UPrimitiveComponent* comp  : listXrayElements)
		{
			bool isShow = true;

			if (compXrayArea)
			{
				FVector dist = (comp->GetComponentLocation() - compXrayArea->GetComponentLocation()).GetAbs();
				FVector volume = compXrayArea->GetScaledBoxExtent();
				isShow = (dist.X <= volume.X && dist.Y <= volume.Y && dist.Z <= volume.Z);
			}

			
			
				if(isShow)
			{
				captureCompRigt->ShowOnlyComponents.Add(comp);

				captureCompTop->ShowOnlyComponents.Add(comp);
			}

		}


		captureCompRigt->TextureTarget = ListRenderTargetsRight[0];
		captureCompTop->TextureTarget = ListRenderTargetsTop[0];
		capture();
	
	
		FTimerHandle timerApplyResetMats;
		GetWorldTimerManager().SetTimer(timerApplyResetMats, this, &AXRayRender::applyResetMats, 0.05f, false);
}




void AXRayRender::applyXrayMats()
{
	applyMats(true);
}

void AXRayRender::applyResetMats()
{
	
		applyMats(false);
}

void AXRayRender::applyMats(bool _status)
{
	
	//for (FXrayElement elem : listXrayElements)
	for (UPrimitiveComponent* comp : listXrayElements)
	{
		AItem* item = Cast<AItem>(comp->GetOwner());
		if (item)
		{
			if (_status)
			{
				item->SetComponentMat("XRAY", comp->GetName());
				if (listHiddenXrayItems.Contains(item))
					item->SetActorHiddenInGame(false);
			}
				
			else
			{
				item->ResetCompMat(comp->GetName());
				if (listHiddenXrayItems.Contains(item))
					item->SetActorHiddenInGame(true);
			}
				

		}
		
		/*if (item)
		{

			UMaterialInterface* mat = 0;

			if (_status)
			{
				if (item->ListXrayMats.Contains(elem.comp->GetName()))
				{
					mat = Cast<UMaterialInterface>(item->ListXrayMats[elem.comp->GetName()]);
				}

				if (mat)
				{
					setElementMat(elem, mat);
				}
				else
				{
				}
			}

			else
			{
				setElementMat(elem, 0);
			}

		}*/
		
		






	}
}


void AXRayRender::capture()
{
	captureCompRigt->UpdateContent();
	captureCompRigt->CaptureScene();
	captureCompTop->UpdateContent();
	captureCompTop->CaptureScene();

	UWidgetComponent* widgetCarmMonitorR = Cast<UWidgetComponent>(ASPINECont::INS->UiCont->GetComponent("WCarmRight"));
	widgetCarmMonitorR->RequestRedraw();
	UWidgetComponent* widgetCarmMonitorL = Cast<UWidgetComponent>(ASPINECont::INS->UiCont->GetComponent("WCarmLeft"));
	widgetCarmMonitorL->RequestRedraw();
}

void AXRayRender::ClearCaptures()
{




	TArray<AActor*> forClear;
	forClear.Add(this);
	{
		captureCompRigt->ShowOnlyActors = forClear;
		captureCompRigt->ShowOnlyComponents.Empty(0);
		captureCompRigt->TextureTarget = ListRenderTargetsRight[0];

		captureCompTop->ShowOnlyActors = forClear;
		captureCompTop->ShowOnlyComponents.Empty(0);
		captureCompTop->TextureTarget = ListRenderTargetsTop[0];


		capture();

	}

}





///*____________________ XRAY ELEMENT ____________________ */
//void UXrayElement::Init(USceneComponent* _comp)
//{
//	comp = _comp;
//	UStaticMeshComponent* staticMeshComp = Cast<UStaticMeshComponent>(comp);
//	isStaticMesh = (bool)(staticMeshComp != nullptr);
//	if (isStaticMesh)
//	{
//		defaultMat = staticMeshComp->GetMaterial(0);
//	}
//
//}
//
//void UXrayElement::SetEnable(bool _status)
//{
//	if (isStaticMesh)
//	{
//		UStaticMeshComponent* staticMeshComp = Cast<UStaticMeshComponent>(comp);
//		UMaterialInterface* mat = 0;
//
//		if (staticMeshComp)
//		{
//			if(staticMeshComp->ComponentHasTag("XRAY"))
//				mat = (UMaterialInterface*)UResource::INS->MAT_TOOLXRAY;
//		}
//
//		
//		if (_status)
//		{
//			if (mat) 	staticMeshComp->SetMaterial(0, mat);
//		}
//		else {
//			if (mat) staticMeshComp->SetMaterial(0, defaultMat);
//		}
//	}
//	else {
//		comp->SetVisibility(_status);
//	}
//}
