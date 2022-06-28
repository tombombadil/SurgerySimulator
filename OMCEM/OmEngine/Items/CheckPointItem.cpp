// Copyright 2015 - 2020 SharpSurgeon 
#include "CheckPointItem.h"
#include "OMCEM.h"
#include "OmEngine/Components/OmComp.h"
#include "OmEngine/Controllers/CameraCont.h"
#include "../Controllers/PlayerCont.h"


void ACheckPointItem::BeginPlay()
{
	//SetActorHiddenInGame(true);
	Super::BeginPlay();
}
void ACheckPointItem::InitItem(FCheckPoint _data)
{
	if (!IsEnable) return;
	SetActorHiddenInGame(false);
	IsChecked = false;
	IsOnLookAtFromCam = false;
	FString detail = "";
	BpEventRun("func:(SetText),title:(" + _data.Title + "),detail:(" + detail + ")");
	UOmComp* compMainVol = GetCompByName("MainVolume");
	UOmComp* compTargetVol = GetCompByName("TargetVolume");
}

void ACheckPointItem::Tick(float DeltaSeconds)
{
	if (!IsEnable || (APlayerCont::INS->GetFocus() == EFocus::UI_CONTROLLER))
	{
		SetActorHiddenInGame(true);
		return;
	}
	else
	{
		if (!IsChecked) SetActorHiddenInGame(false);
	}

	if (IsOnLookAtFromCam)
	{
		Open();
	}
	else
	{
		Close();
	}


	if (UCameraCont::INS && IsDynamicLookAtCam)
	{
		FVector dir = UCameraCont::INS->GetComponentLocation() - GetActorLocation();
		FRotator Rot = FRotationMatrix::MakeFromX(dir).Rotator();
		FRotator newRot = GetActorRotation();
		newRot.Yaw = Rot.Yaw + 180;
		SetActorRotation(newRot);
	}

	//if (APlayerCont::INS->BtnTrigger)
	//{
	//	if (IsOnLookAtFromCam)
	//	{
	//		if (!IsChecked)
	//		{
	//			IsChecked = true;
	//			BpEventRun("cmd:(Completed)");
	//		}
	//		
	//	}
	//}

	IsOnLookAtFromCam = false;


	AItem::Tick(DeltaSeconds);
}

void ACheckPointItem::Open()
{
	BpEventRun("func:(OpenAnim)");
}

void ACheckPointItem::Close()
{
	BpEventRun("func:(CloseAnim)");
}



void ACheckPointItem::OnSelect(FSelectHandler _handler)
{
	if (!IsEnable || (APlayerCont::INS->GetFocus() == EFocus::UI_CONTROLLER)) return;
	if (IsChecked) return;

	IsOnLookAtFromCam = true;

	Super::OnSelect(_handler);
}
