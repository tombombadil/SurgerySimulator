// Copyright 2015 - 2020 SharpSurgeon 

#pragma once

#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "SpineStructs.h"
#include "SpineConnector.generated.h"





UCLASS(Blueprintable)
class OMCEM_API USpineConnector : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY() FString PATH_JSON_CONNECTOR;
	FSpineData LoadSpine();

	TSharedPtr<FJsonObject> JsonBase;

	UFUNCTION(BlueprintCallable, Category="LAUNCHER") void SaveSpine(FSpineData Data);
};
