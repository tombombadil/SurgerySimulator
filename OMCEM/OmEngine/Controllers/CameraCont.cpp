#include "CameraCont.h"
#include "OMCEM.h"
#include "OmEngine/Items/FieldItem.h"
#include "OmEngine/Controllers/PlayerCont.h"
#include "MainCont.h"
#include "OmEngine/Utils/Config.h"
#include "Observer.h"
#include "OmEngine/Utils/Resource.h"

UCameraCont* UCameraCont::INS = NULL;

UCameraCont::UCameraCont()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCameraCont::Init()
{
	
	if (INS == NULL)INS = this;
	if (AMainCont::INS->IsVRMode)
	{
		GetAttachParent()->SetRelativeLocation(FVector::ZeroVector);
		FRotator r = GetAttachParent()->GetRelativeTransform().Rotator();
		r.Roll = 0;
		r.Pitch = 0;
		GetAttachParent()->SetRelativeRotation(r);
	}
	defCamTrans = GetAttachParent()->GetComponentTransform();
	defCamRelTrans = GetRelativeTransform();
	defZoomRatio = 0.5f;
	zoomRatio = defZoomRatio;
	inputRotationOffset = FVector::ZeroVector;


}


FVector UCameraCont::GetLookAtPoint(float _length)
{
	FVector pos = GetComponentLocation() + (GetForwardVector().GetSafeNormal() * _length);
	return pos;
}









void UCameraCont::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	USceneComponent* parent = GetAttachParent();
	FVector newLoc = parent->GetComponentLocation();
	FRotator newRot = parent->GetComponentRotation();

	float CamSpeedFinal = CamSpeedDefault;
	if (APlayerCont::GetKeyAction("KeyAlt")) CamSpeedFinal = CamSpeedMin;
	if (APlayerCont::GetKeyAction("KeyShift")) CamSpeedFinal = CamSpeedMax;

	if (APlayerCont::GetKeyAxis("CamMoveX") != 0)
	{
	
		newLoc.X += (APlayerCont::GetKeyAxis("CamMoveX") * CamSpeedFinal);
		bLockToHmd = false;
	}

	if (APlayerCont::GetKeyAxis("CamMoveY") != 0)
	{
		newLoc.Y += (APlayerCont::GetKeyAxis("CamMoveY") *CamSpeedFinal);
		bLockToHmd = false;
	}

	if (APlayerCont::GetKeyAxis("CamZoom") != 0)
	{
		newLoc += GetForwardVector() * APlayerCont::GetKeyAxis("CamZoom") * CamSpeedFinal;
		bLockToHmd = false;
	}
	//if (!AMainCont::INS->IsVRMode)
	{
		if (APlayerCont::GetKeyAxis("CamRotX") != 0)
		{
			newRot.Yaw += APlayerCont::GetKeyAxis("CamRotX") * 1;
		}
		if (APlayerCont::GetKeyAxis("CamRotY")!= 0)
		{
			newRot.Pitch += APlayerCont::GetKeyAxis("CamRotY") * 1;
		}
	}

	

	parent->SetWorldLocation(newLoc);
	parent->SetWorldRotation(newRot);
	bLockToHmd = true;



}


void UCameraCont::SetCamLoc(FVector _loc)
{
	SetWorldLocation(_loc);
}

void UCameraCont::SetCamRot(FRotator _rot)
{
	FRotator newRot = FRotator::ZeroRotator;
	newRot.Yaw = _rot.Yaw;
	SetWorldRotation(newRot);
}



void UCameraCont::ResetCamPosition()
{
	USceneComponent* parent = GetAttachParent();
	parent->SetWorldTransform(defCamTrans);
}