// Copyright 2015 - 2020 SharpSurgeon 

#pragma once

#include "GameFramework/PlayerController.h"
#include "OmPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OMCEM_API AOmPlayerController : public APlayerController
{
	GENERATED_BODY()



		AOmPlayerController();
	 void AutoManageActiveCameraTarget(AActor* SuggestedTarget) override;
	//AOmPlayerController(const FObjectInitializer& ObjectInitializer) override;
};
