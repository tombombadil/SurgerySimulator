// Copyright 2015 - 2020 SharpSurgeon 

#pragma once

#include "OmEngine/Items/Item.h"
#include "CheckPointItem.h"
#include "Patient.generated.h"


UCLASS()
class OMCEM_API APatient : public AItem
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	UPROPERTY() TArray<ACheckPointItem*> ListCheckPoints;
private:
	
};
