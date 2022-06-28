#pragma once

#include "OmEngine/Utils/OmValue.h"
#include "OmEngine/Utils/OmStructs.h"
#include <vector>
#include <Components/BoxComponent.h>
#include "OmComp.generated.h"

struct FContactResult;
struct FCollisionList;
struct FOmValue;
struct FOmValues;





UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UOmComp : public UBoxComponent
{
	GENERATED_BODY()



public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OMCEM) bool IsEnable = true;
	UPROPERTY() UPrimitiveComponent* Collision;
	UPROPERTY() int8 Index;
	UPROPERTY(BlueprintReadOnly, Category = OMCEM) FString Id;
	UPROPERTY() FString FullName;
	UPROPERTY(BlueprintReadOnly, Category = OMCEM)  UOmComp* OwnerComp = 0;
	UPROPERTY(BlueprintReadOnly, Category = OMCEM)  class AItem* OwnerItem = 0;

	FContactResult* CollisionEnter; //flag
	FContactResult* CollisionExit; //flag
	FCollisionList* CollList;

	UOmComp();
	FString Create(int8 _index, FString _id);

	virtual void Init();
	virtual void Render(float DeltaTime);
	void SetStatus(EEventType _status);
	EEventType GetStatus();
	//virtual void OnCollision(bool _status, FContactResult* _collInfo);
	void SetLoc(FVector _loc);
	void SetRot(FRotator _rot);
	virtual void SetTrans(FTransform _omTrans);
	void SetRelTrans(FTransform _trans);
	FTransform GetRelTrans();
	virtual FTransform GetTrans();
	UFUNCTION(BlueprintCallable, Category = "OM3") virtual void SetEnable(bool _enable);
	UFUNCTION(BlueprintCallable, Category = "OM3") virtual bool GetEnable();
	void SetCollisionPreset(FString _presetName);
	void ResetCollisionPreset();
	FVector GetTipPoint(bool _isWorld=true);

	void SaveTrans(FString _id);
	FTransform GetSavedTrans(FString _id);
	FTransform RestoreTrans(FString _id);
	FVector RestoreLoc(FString _id);
	UFUNCTION(BlueprintCallable, Category = "OM3") void UpdateParams(FString _strParams, bool sendToObserver=false);
	void UpdateParams(FOmValue _value, bool sendToObserver=false);
	void SendParams(FOmValue _value);
	UFUNCTION(BlueprintCallable, Category = "OM3") FString GetParam(FString _key);
	UFUNCTION(BlueprintCallable, Category = OMCEM)void SetInitTrans(FTransform _trans);
	UFUNCTION(BlueprintCallable, Category = OMCEM)FTransform GetInitTrans();
	/*GetRandomPositionInBounds : get random worl position in bounds of box extends */
	UFUNCTION(BlueprintCallable, Category = OMCEM) FVector GetRandomPositionInBounds();
	UFUNCTION(Category = OMCEM) virtual void Run(FOmValue cmd);

	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventStart();
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventStartOperation();
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventRun(const FString &cmd);
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventStatus(EEventType _status);
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventOnColl(class AItem* FromItem, UOmComp* FromComp, FVector _location); //overlap
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventOnCollRemove(class AItem* FromItem, UOmComp* FromComp);
	void PreDestroy();

private:
	UPROPERTY() bool isHit = false;
	//UPROPERTY() bool isEnable = true;
	UPROPERTY()	FOmValue params;
	UPROPERTY() FString collProfileName;
	//UFUNCTION() void onOverlapBegin( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION() void onOverlapEnd( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	EEventType status;
	TMap<FString, FTransform> listSavedTrans;

protected:
	FTransform initTrans;
};