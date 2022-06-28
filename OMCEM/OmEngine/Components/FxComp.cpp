// Copyright 2015 - 2020 SharpSurgeon 
#include "FxComp.h"
#include "OMCEM.h"
#include "OmEngine/Utils/OmUtils.h"
#include "OmEngine/Items/Item.h"
#include "OmEngine/Items/ToolItem.h"
#include "OmEngine/Controllers/MainCont.h"




void UFxComp::BeginPlay()
{
	Super::BeginPlay();
	lastSpawnLoc = FVector::ZeroVector;
}

void UFxComp::Init()
{
	Super::Init();
	SetEnable(false);
	
}

void UFxComp::Render(float DeltaTime)
{
	Super::Render(DeltaTime);
	
	if (!GetEnable()) return;
	FVector from = GetComponentLocation();
	FVector to = from + (GetUpVector() *  -2);
	TArray<FHitResult> listRays = 	UOmUtils::RayCast(true, EChannels::TRACE_RAY_RIGIDBODY, from, to, 1, FColor::Black);
	for (FHitResult ray : listRays)
	{
		for (FString tag : ParticleCollisionTag)
		{
			//bool hasTag = ray.GetActor() ? ray.GetActor()->ActorHasTag(*tag) : false;
			bool hasTag = ray.GetComponent() ? ray.GetComponent()->ComponentHasTag(*tag) : false;
			if (hasTag)
			{
				spawnParticle(ray.Location);
				if(Decal.Enable) spawnDecal(ray.Location, 4);
			}

		}
	}

	
}



void UFxComp::spawnDecal(FVector _loc, float _lifeSpawn)
{
	if (Decal.Mat)
	{
		FActorSpawnParameters SpawnInfo;
		ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(_loc, FRotator(90, 0, 0), SpawnInfo);
		if (decal)
		{
			decal->SetDecalMaterial(Decal.Mat);
			decal->SetLifeSpan(_lifeSpawn);
			decal->GetDecal()->DecalSize = Decal.Size;
		}

	}


}
void UFxComp::spawnParticle(FVector _loc)
{
	if (FVector::Distance(lastSpawnLoc, _loc) > 1)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleClass, _loc, FRotator::ZeroRotator, true);
		lastSpawnLoc = _loc;
		startTimer();
	}
	
}


void UFxComp::startTimer()
{

	//Calling MyUsefulFunction after 3 seconds without looping
	float delay = 3;
	AMainCont::INS->GetWorldTimerManager().SetTimer(timer, timerDelegate, delay, false);
}

void UFxComp::stopTimer()
{
	if (timer.IsValid())
		AMainCont::INS->GetWorld()->GetTimerManager().ClearTimer(timer);
}

void UFxComp::onTimerHandler()
{
	

}
