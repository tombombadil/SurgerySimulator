// Copyright 2015 - 2020 SharpSurgeon 
#pragma once

#include "OmEngine/Components/OmComp.h"
#include "Materials/Material.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleSystem.h"
#include "CheckOutOfMeshComp.generated.h"



UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UCheckOutOfMeshComp : public UOmComp
{
	GENERATED_BODY()
public:
	void BeginPlay() override;
	void Init() override;
	void Render(float DeltaTime) override;

	/* GetResult() retur(0) comp out of mesh, return(1) comp in comp */
	UFUNCTION(BlueprintCallable, Category="OM3") bool GetInnerResult(TArray<FString> ListMeshComps, float RayLength=5);
private:
	bool getRay(FVector _direction, float _len, TArray<FString> _listMeshComps);
};
