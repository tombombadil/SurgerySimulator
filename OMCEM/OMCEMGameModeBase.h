// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "GameFramework/Actor.h"
#include "OMCEMGameModeBase.generated.h"



UCLASS()
class OMCEM_API AOMCEMGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
		AOMCEMGameModeBase();
	

		virtual void BeginPlay() override;
};
