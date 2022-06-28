// Copyright 2015 - 2020 SharpSurgeon 
#pragma once
#include "../Utils/SpineStructs.h"
#include "OmEngine/Components/OmComp.h"
#include "HoleInfoComp.generated.h"


UCLASS( Blueprintable)
class OMCEM_API UHoleInfoComp : public UOmComp
{
	GENERATED_BODY()
public:
		//UPROPERTY(EditAnywhere, Category = "OM3") FRotator MinAngles;
		//UPROPERTY(EditAnywhere, Category = "OM3") FRotator MaxAngles;
		//UPROPERTY(EditAnywhere, Category = "OM3") float MinDepth;
		//UPROPERTY(EditAnywhere, Category = "OM3") float MaxDepth;
		UPROPERTY(EditAnywhere, Category = "OM3") FSpineHoleData Data;
	
};
