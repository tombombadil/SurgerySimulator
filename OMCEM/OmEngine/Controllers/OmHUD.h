// Copyright 2015 - 2020 SharpSurgeon 

#pragma once

#include "GameFramework/HUD.h"
#include "../Utils/OmValue.h"
#include "OmHUD.generated.h"

/**
 * 
 */
UCLASS()
class OMCEM_API AOmHUD : public AHUD
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventRun(const FString &Params);
	void ShowMessage(FString _msg, bool _append = true);
	void HideMessage();
private:
	TArray<FString> listMessages;
};
