#pragma once

#include "OmEngine/Utils/OmValue.h"
#include "OmComp.h"
#include "ContainerComp.generated.h"







UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UContainerComp : public UOmComp
{
	GENERATED_BODY()



public:
	
	/* if list doesnt empty, the items which has IdCat in list can attach to here */
	UPROPERTY(EditAnywhere, Category = "OM3-Attach")  TArray<FString> FilterOnlyCats;
	/* the items which has IdCat in list cant attach to here */
	UPROPERTY(EditAnywhere, Category = "OM3-Attach")  TArray<FString> FilterExceptCats;
	
	bool CanPutItem(FString _idCat);
};