// Copyright 2015 - 2020 SharpSurgeon 
#pragma once

#include "OmEngine/Items/Item.h"
#include "CheckPointItem.generated.h"


USTRUCT()
struct OMCEM_API FCheckPoint
{
	GENERATED_USTRUCT_BODY()

		int32 Id;
	FString Title;


};


UCLASS()
class OMCEM_API ACheckPointItem : public AItem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere) int OrderIndex = 0;
	UPROPERTY(EditAnywhere) bool IsDynamicLookAtCam;
	UPROPERTY(BlueprintReadOnly) bool IsChecked;
	UPROPERTY() bool IsOnLookAtFromCam;
	virtual void BeginPlay() override;
	void InitItem(FCheckPoint _title);
	 void Tick(float DeltaSeconds) override;
	void Open();
	
	void Close();
	void OnSelect( FSelectHandler _handler) override;
};
