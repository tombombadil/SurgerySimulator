// Fill out your copyright notice in the Description page of Project Settings.


#include "VrMoveCont.h"
#include "../PlayerCont.h"
#include "../../Components/ProcessComp.h"
#include "../MainCont.h"
#include "../../../OMCEM.h"

void AVrMoveCont::Init()
{
	Super::Init();
	if (HandMesh)  HandMesh->SetVisibility(false);
	VrMenu = Cast<AVrMenu>(GetChildActor("VrMenu"));


	MotionComp =  Cast< UMotionControllerComponent>(GetComponentByClass(UMotionControllerComponent::StaticClass()));
	HandRigComp = Cast<USkeletalMeshComponent>(GetComponent("HandRig"));;
	HandRigComp->SetVisibility(false, true);
	OculusMeshComp = Cast<UStaticMeshComponent>(GetComponent("OculusControllerMesh"));;
	if(OculusMeshComp) 	OculusMeshComp->SetVisibility(false, true);
	
	//TArray<USceneComponent*> listAllChilds;
	//GetComponents(listAllChilds, true);
	//for (USceneComponent* c : listAllChilds)
	//{

	//	if (c->GetName() == "MotionC")
	//		compCamRoot = c;

	//}

	

	if (AMainCont::INS->IsVRMode)
	{
		if (AMainCont::INS->InputMode == 3)
		{
			if (HandRigComp) HandRigComp->SetVisibility(true,true);

		
		}
		else
		{
			if (OculusMeshComp) OculusMeshComp->SetVisibility(true,true);
		}
			
	}


	
}


void AVrMoveCont::Start()
{
	Super::Start();
	APlayerCont::INS->InputListener->EventAction.AddDynamic(this, &AVrMoveCont::onInputReceive);

}


void AVrMoveCont::TakeTool(class AToolItem* _tool, bool _releaseCurr)
{
	Super::TakeTool(_tool, _releaseCurr);

	if (_tool)
	{
		BpEventHandAnim("TakeTool", _tool->VrHandAnimStatus);
	}
}

void AVrMoveCont::PutTool(AToolItem* _tool, EPutToolRule _putRule, FVector _releaseLoc /*= FVector::ZeroVector*/)
{
	Super::PutTool(_tool, _putRule, _releaseLoc);


	if (_tool)
	{
		BpEventHandAnim("PutTool",0);
	}
}



void AVrMoveCont::onInputReceive(bool _status, FString _key)
{

	if (_key == "VrRightTrigger")
	{
		if (_status)
		{
			

			if (currProcessComp)
			{
				UpdateProcessInputOrigin();
				
			}
				
		}
	}




}


/* key presets from UInputs */
void AVrMoveCont::OnInputHandler(FInputInfo _info)
{

	Super::OnInputHandler(_info);

	FString _keyPreset = _info.KeyPreset;
	bool _status = (_info.Value != 0);

	

	//if (_keyPreset == "KeyProcess" && Input->IsVr())
	//{
	//	if (_status)
	//	{
	//		VrDeltaRotStart = GetActorTransform().GetRotation().Rotator();
	//	}
	//}



}



void AVrMoveCont::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);



	if (InputSource == EInputSource::VR_RIGHT)
	{
		bool isShowLaser = APlayerCont::GetKeyAction("KeyShowRVrLaser");
		//RaySelectWidget->ShowLine(isShowLaser, true);
	}
	else if (InputSource == EInputSource::VR_LEFT)
	{
		bool isShowLaser = APlayerCont::GetKeyAction("KeyShowLVrLaser");
		//RaySelectWidget->ShowLine(isShowLaser, true);
	}


	if (enableSelectWidget && !CurrTool)
	{
		if (RaySelectWidget)
		{
			if (RaySelectWidget->GetHitComps().Num() > 0)
			{
				BpEventHandAnim("SelectWidget", 1);
			}
			else
			{
				BpEventHandAnim("SelectWidget", 0);
			}
		}

	}
		

}







/* haptic'deki kalemin yaw donme acisi */
float  AVrMoveCont::GetAxisYaw()
{
	float r = 0;
	r = MotionComp->GetRelativeTransform().GetRotation().Rotator().Roll * -1 * 2;

	return r;
}

void AVrMoveCont::setEnableToolSelectDisplays(bool _enable, class AToolItem* _selected, bool _isErr /*= false*/)
{
	Super::setEnableToolSelectDisplays(_enable, _selected, _isErr);

	if (!CurrTool)
	{
		if (_enable)
		{
			if (_selected)
			{
				BpEventHandAnim("SelectTool", 1);
			}

		}
		else
		{

			BpEventHandAnim("SelectTool", 0);
		}
	}
	
}

FVector AVrMoveCont::GetProcessInputLoc()
{
	FVector loc = MotionComp->GetComponentLocation() - processInputOrigin.GetLocation();
	loc = loc * 0.7f;
	return loc;
}

FRotator AVrMoveCont::GetProcessInputRot()
{
	FRotator toolContainerRot = CompToolContainer->GetComponentRotation();

	toolContainerRot.Pitch = CompToolContainer->GetComponentRotation().Roll * -1;
	toolContainerRot.Roll = CompToolContainer->GetComponentRotation().Pitch;





	FRotator rot = toolContainerRot - processInputOrigin.GetRotation().Rotator();
	return rot;
}


void AVrMoveCont::UpdateProcessInputOrigin()
{
	processInputOrigin = MotionComp->GetComponentTransform();
}

void AVrMoveCont::SetOperationHand(bool _isOperation)
{
	Super::SetOperationHand(_isOperation);
	//Input->SetEnable(true);

	
}
