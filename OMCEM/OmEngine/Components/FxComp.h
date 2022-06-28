// Copyright 2015 - 2020 SharpSurgeon 
#pragma once
#include "OmEngine/Components/OmComp.h"
#include "Materials/Material.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleSystem.h"
#include <TimerManager.h>
#include "FxComp.generated.h"

USTRUCT()
struct OMCEM_API FDecalParam
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere)  bool Enable;
		UPROPERTY(EditAnywhere)  UMaterialInterface* Mat;
	UPROPERTY(EditAnywhere)  FVector Size;
	UPROPERTY(EditAnywhere)  float Life;
};

//USTRUCT()
//struct OMCEM_API FSoundParam
//{
//	GENERATED_USTRUCT_BODY()
//		UPROPERTY(EditAnywhere)   bool Enable;
//		UPROPERTY(EditAnywhere)   USoundCue* SoundCue;
//};

UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UFxComp : public UOmComp
{
	GENERATED_BODY()
private:
	void spawnParticle(FVector _loc);
	void startTimer();
	void stopTimer();
	FVector lastSpawnLoc;
	/* timer */
	FTimerDelegate timerDelegate;
	FTimerHandle timer;
	UFUNCTION()	void onTimerHandler();
	void spawnDecal(FVector _loc, float _lifeSpawn);
public:
	void BeginPlay() override;
	void Init() override;
	void Render(float DeltaTime) override;


	//UPROPERTY(EditAnywhere, Category = "[OMCEM] DECAL")  UMaterial* DecalMat;
	UPROPERTY(EditAnywhere, Category = "[OMCEM] DECAL")  FDecalParam Decal;
	//UPROPERTY(EditAnywhere, Category = "[OMCEM] DECAL")  FSoundParam Sound;
	UPROPERTY(EditAnywhere, Category = "[OMCEM] PARTICLE")  UParticleSystem* ParticleClass;
	/* tags of this component collision with other component */
	UPROPERTY(EditAnywhere, Category = "[OMCEM] PARTICLE")  TArray<FString> ParticleCollisionTag;
	
};
