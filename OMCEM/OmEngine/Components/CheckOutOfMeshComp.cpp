// Copyright 2015 - 2020 SharpSurgeon 
#include "CheckOutOfMeshComp.h"
#include "OMCEM.h"
#include "OmEngine/Utils/OmUtils.h"
#include "OmEngine/Items/Item.h"
#include "OmEngine/Items/ToolItem.h"
#include "OmEngine/Controllers/MainCont.h"




void UCheckOutOfMeshComp::BeginPlay()
{
	Super::BeginPlay();
}

void UCheckOutOfMeshComp::Init()
{
	Super::Init();
	//SetEnable(false);
	
}

void UCheckOutOfMeshComp::Render(float DeltaTime)
{
	Super::Render(DeltaTime);
	

}



bool UCheckOutOfMeshComp::GetInnerResult( TArray<FString> ListMeshComps, float RayLength)
{
	bool resultFinal = true;
	TArray<FVector> listDirs;
	listDirs.Add(GetUpVector()* -1);
	listDirs.Add(GetRightVector());
	listDirs.Add(GetRightVector() *-1);
	listDirs.Add(GetForwardVector() );
	listDirs.Add(GetForwardVector() *-1);
	for (FVector dir : listDirs)
	{
		bool status = getRay(dir, RayLength, ListMeshComps);
			if (!status)
				resultFinal = false;
	}

	return resultFinal;
}

bool UCheckOutOfMeshComp::getRay(FVector _direction, float _len, TArray<FString> _listMeshComps)
{
	bool status = false;
	FVector from = GetComponentLocation();
	FVector to = from + (_direction * _len);
	UOmUtils::DrawLine(from, to, FColor::Orange, false);
	TArray<FHitResult> listRays = 	UOmUtils::RayCast(true, EChannels::TRACE_RAY_RIGIDBODY, from, to, 1, FColor::Black);
	for (FHitResult ray : listRays)
	{
		FString idHitComp = UResource::INS->GenerateCompId(ray.GetComponent());

		if (_listMeshComps.Contains(idHitComp))
		{
			status = true;
			UOmUtils::DrawLine(from, to, FColor::Green, false);
		}
			
			
	}
	return status;


}

