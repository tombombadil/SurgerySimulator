#pragma once

#include "OmEngine/Items/Item.h"
#include "OmEngine/Utils/OmStructs.h"
#include "OmEngine/Items/RayHandler.h"
#include <MotionControllerComponent.h>
#include "MoveCont.generated.h"

class UJointForce;

UCLASS(ClassGroup = (OMCEM), meta = (BlueprintSpawnableComponent))
class OMCEM_API AMoveCont : public AItem
{
	GENERATED_BODY()
public:
	AMoveCont();
	UPROPERTY(BlueprintReadOnly, Category = OMCEM) class UInputs* Input;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OMCEM) EInputSource InputSource;
	UPROPERTY(BlueprintReadOnly, Category = OMCEM) class AToolItem* CurrTool; //first
	UPROPERTY(BlueprintReadOnly, Category = OMCEM) class  AToolItem* CurrPart; //second
	UPROPERTY(BlueprintReadOnly, Category = "OMCEM") float TimerFromTurnOn; //butona basili  gecen sure. yani alet xx ms kadardir çalisiyor.
	UPROPERTY(BlueprintReadOnly, Category = "OMCEM") ARayHandler* RaySelectTool;
	UPROPERTY(BlueprintReadOnly, Category = "OMCEM") ARayHandler* RayHit;
	UPROPERTY(BlueprintReadOnly, Category = "OMCEM") ARayHandler* RayCollProcess;
	UPROPERTY(BlueprintReadOnly, Category = "OMCEM") ARayHandler* RaySelectWidget;
	//UPROPERTY(BlueprintReadOnly, Category = "OMCEM") UMotionControllerComponent* MotionComp;
	UPROPERTY(BlueprintReadOnly, Category = "OMCEM") class UOmComp* RootComp;
	UPROPERTY(BlueprintReadOnly, Category = "OMCEM") bool IsOnProcess;
	//UFUNCTION(BlueprintCallable, Category = OMCEM) virtual void SetVibrate(bool _status, float _timeout, float _amplitude = 0.2, float _freq = 200);
	void SetEnable(bool _enable) ;

	void SetEnableSelectTool(bool _enable);
	void SetEnableForceFeedback(bool _enable);
	void SetEnableProcessTrigger(bool _enable);
	void SetEnableSelectWidget(bool _enable);


	/* Process Input Functions for ProcessComp : processComp input icin bu funclari kullaniyor */
	virtual FVector GetProcessInputLoc();
	virtual FRotator GetProcessInputRot();
	virtual void UpdateProcessInputOrigin();

	virtual float GetAxisYaw();
	UFUNCTION() virtual void OnInputHandler(FInputInfo _info) ;
	virtual void Init() override;
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable, Category = OMCEM)  void SetProcess(bool _status, UProcessComp* _processComp, FOmValue _params);

	UFUNCTION(BlueprintCallable, Category = "OM3") virtual void TakeTool(class AToolItem* _tool, bool _releaseCurr);
	UFUNCTION(BlueprintCallable, Category = "OM3") virtual void PutTool(AToolItem* _tool, EPutToolRule _putRule, FVector _releaseLoc=FVector::ZeroVector);


	//PairTrans GetMoveTrans();
	//void SetMoveTrans(PairTrans _pairTrans);
	void UpdateParams(FOmValue _value, bool sendToObserver);
	void SendParams(FOmValue _value);
	//void UpdateMetrics(FOmValue _value);
	void OnHapticKeyHandler(EEventType _type, FString _keyId);
	void Destroy();
	virtual	void SetOperationHand (bool _isOperation);
	UProcessComp* GetCurrProcessComp();
	 void Run(FOmValue cmd);
	 void updateTransform();
protected:

	UPROPERTY() FTransform processInputOrigin;
	UPROPERTY() bool enableSelectTool;
	UPROPERTY() bool enableForceFeedback;
	UPROPERTY() bool enableProcessTrigger;
	UPROPERTY() bool enableSelectWidget ;
	//UPROPERTY() 	FOmValue paramsMetrics;

	FString operationDir;;
	FVector direction;
	FRotator baseRot;
	UPROPERTY() bool isOperationHand;
	UPROPERTY() UProcessComp* currProcessComp;
	UPROPERTY() UOmComp* CompToolContainer;
	UPROPERTY() UJointForce* jointForce;

	FVector locProcessBefore;
	bool isRealignThresholdUp;
	bool isRealignThresholDown;
	FVector boundsBox;
	bool isInOpArea;
	FTransform initialTrans;
	virtual void setEnableToolSelectDisplays(bool _enable, class AToolItem* _selected, bool _isErr=false) ;
	void sendProcessRayResult();
	TArray<FContactResult> getSelectToolResult();
	UPROPERTY()	USceneComponent* HandMesh;
	FVector getHitRayResult(float _length);
	FVector getHitRayResult(FVector _angle, float _length);
	void takeAndPutTool();
	void setBaseRot(FRotator _rotator);
	bool isEnableVibrateEff;
	float vibrateEffTimeout;
	float vibrateEffTimer;
	FVector hitForce;
	void setCurrentPart(AToolItem* _newCurrPart);
	void updateToolContainerRelative(AToolItem* _tool);

private:
	UPROPERTY() FTransform compToolContainerDefaultTrans;
};
