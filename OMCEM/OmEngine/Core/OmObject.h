// Copyright 2015 - 2020 SharpSurgeon 

#pragma once

#include "OmObject.generated.h"

/**
 * 
 */
UCLASS()
class OMCEM_API UOmObject : public UObject
{
	GENERATED_BODY()

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3") FString Id;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3") bool IsEnable = true;
		//UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventRun(const FString& cmd);
		UFUNCTION(BlueprintImplementableEvent, Category = "OM3") void BpEventStart(); //hit
	
};
