// Copyright 2015 - 2020 SharpSurgeon 
#include "BleedingComp.h"
#include "OmEngine/Items/Item.h"




void UBleedingComp::BeginPlay()
{
	timeCollisionWithTool = 0;
	IsBleeding = false;
	UOmComp::BeginPlay();
}

void UBleedingComp::StartBleeding()
{
	if (!IsBleeding)
	{
		IsBleeding = true;
		BpEventRun("func:(StartBleeding)");
	}
	
}

void UBleedingComp::StopBleeding()
{
	if (IsBleeding)
	{
		IsBleeding = false;
		isOnColl = false;
		BpEventRun("func:(StopBleeding)");
	}
	
}

void UBleedingComp::Render(float DeltaTime)
{
	UOmComp::Render(DeltaTime);

	
	if (isOnColl) timeCollisionWithTool += DeltaTime;

	if (timeCollisionWithTool >= CollisionTimeToStopBleeding)
		StopBleeding();

}
//to-do ray olarak degistir
//void UBleedingComp::OnCollision(bool _status, FContactResult* _collInfo)
//{
//	UOmComp::OnCollision(_status, _collInfo);
//
//	if (IsBleeding)
//	{
//		if (_collInfo->toItem->Tags.Contains("COUTER"))
//		{
//			isOnColl = _status;
//		}
//	}
//	
//
//}

