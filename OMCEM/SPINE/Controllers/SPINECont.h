
#pragma once

#include "OmEngine/Controllers/MainCont.h"
#include "../Items/SpineHoleField.h"
#include "../Utils/SpineStructs.h"
#include "../Utils/SpineConnector.h"
#include "SPINECont.generated.h"





UCLASS()
class OMCEM_API ASPINECont : public AMainCont
{
	GENERATED_BODY()

public:
	static ASPINECont* INS;
	ASPINECont();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OMCEM|Config-Classes") TSubclassOf<ASpineHoleField> ClassSpineHole;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OMCEM|Config-Classes") TMap<FString, UMaterialInterface*> ListMaterials;
	UPROPERTY( BlueprintReadWrite, Category = "OM3") FSpineData SpineData;
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) UServer* server;
	UFUNCTION(BlueprintCallable, Category = "OM3") TArray< FSpineHoleData> GetHoleFields();
	UFUNCTION(BlueprintCallable, Category = "OM3") void SetTransparentMode(bool _status);
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventSetVrMenus(AVrMenu*  MenuRight , AVrMenu* MenuLeft );
protected:
	void Start() override;
	void initHoles(FSpineData _data);
	//ASpineHoleField* spawnHole(FSpineHoleData _data);
	FSpineHoleData spawnHole(FSpineHoleData _data, int8 _indexOfHole);
	 void delayedBeginPlay() override;
	UPROPERTY() TArray< FSpineHoleData> listHoleDatas;
	UPROPERTY() USpineConnector* spineConnector;
	
	//virtual void createControllers() override;
};
