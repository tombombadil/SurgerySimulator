#pragma once

#include <MotionControllerComponent.h>
#include "VrMotionComp.generated.h"






UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UVrMotionComp : public UMotionControllerComponent
{
	GENERATED_BODY()

private:
	UPROPERTY() class ARayHandler* RayHandler;
	UFUNCTION() void onInputKeyHandler(bool _status, FString _keyId);
protected:
	FTransform initTrans;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OMCEM) bool IsEnable;
	UPROPERTY(BlueprintReadOnly, Category = OMCEM) FString Id;


	virtual void Init();

	
	virtual void Render(float DeltaTime);
	void SetVisible(bool isVisible, bool _applyAllChilds);
	//virtual void SetEnable(bool _enable);
	//virtual bool GetEnable();

	
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventRun(const FString &cmd);

};