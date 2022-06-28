// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tween.generated.h"

USTRUCT(BlueprintType)
struct FActorTween
{
	GENERATED_USTRUCT_BODY()

	FString id;
	AActor* actor;
	float speed; //alpha change per seconds
	float alpha;
	float status; /* 0(deactive), 1(started), 2(completed) */
	FTransform from;
	FTransform to;

	
};


UCLASS()
class OMCEM_API UTween : public UObject
{
	GENERATED_BODY()
		
	
public:
	UFUNCTION(BlueprintCallable, Category = OMCEM) FActorTween AddActor(AActor* _actor, FTransform _to, float _speed);
	void Render(float DeltaTime);
	static UTween* INS;
	void Init();
	private:
		TMap<FString, FActorTween> listActorTweens;
	
	
	
		
};
