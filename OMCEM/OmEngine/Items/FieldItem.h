#pragma once

#include "OmEngine/Items/Item.h"
#include "FieldItem.generated.h"

UENUM()
enum class ETakeType : uint8
{
	NONE,
	TAKE_ZERO_LOC  UMETA(DisplayName = "TAKE_ZERO_LOC"),
	TAKE_RANDOM_LOC_IN_VOL  UMETA(DisplayName = "TAKE_RANDOM_LOC_IN_VOL"),
	
};



UCLASS()
class OMCEM_API AFieldItem : public AItem
{
	GENERATED_BODY()
	
public:
	/* tool acisini ayarlar ve o aciyi sabitler. values : rel:(),world:()   */
	UPROPERTY(EditAnywhere, Category = OMCEM) bool IsLockCam;
	UPROPERTY(EditAnywhere, Category = OMCEM)  TArray<FString> ListTakes;
	UPROPERTY(EditAnywhere, Category = OMCEM)  TArray<FString> ListPuts;
	UPROPERTY(EditAnywhere, Category = OMCEM) ETakeType TakeType;
	UPROPERTY() int8 CountOfPlacementComps;
	AFieldItem();
	virtual void InitAfter() override;
	AItem* TakePart(int8 _index=0);
	void PutPart(AItem* _item);
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventResetField();
private:
	TArray<AItem*> listTakes;
	TArray<AItem*> listPuts;
	TArray<FTransform> listPutTrans;
};



UCLASS()
class OMCEM_API AHoleField : public AFieldItem
{
	GENERATED_BODY()
public:
	void SetDepth(float _depth);
	float GetDepth();
	UFUNCTION(BlueprintCallable, Category = "OM3") AToolItem* GetScrew();
protected:
	float depth;
};
