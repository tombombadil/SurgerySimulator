#pragma once

#include "OmEngine/Components/OmComp.h"
#include "OmEngine/Items/ToolItem.h"
#include "OmEngine/Utils/OmUtils.h"
#include "OmEngine/Inputs/Inputs.h"
#include "ProcessComp.generated.h"




UENUM()
enum class EProgressType : uint8
{
	FOLLOW_PATH  UMETA(DisplayName = "FOLLOW THE PATH FROM START TO END"),
	FREE  UMETA(DisplayName = "FREE MOVEMENT IN LIMITS"),
	HAPTIC_STICK_ANGLE  UMETA(DisplayName = "HAPTIC_STICK_ANGLE")
	

};



/*____________________ CLASS<ProcessComp> ____________________ */

UCLASS(ClassGroup = (OMCEM), meta = (BlueprintSpawnableComponent))
class OMCEM_API UProcessComp : public UOmComp
{
	GENERATED_BODY()



public:
	UPROPERTY(EditAnywhere, Category = "OM3 - PROCESS")  bool IsShowWidgetBar = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCESS", Meta = (MakeEditWidget = true)) bool ApllyLimits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCESS", Meta = (MakeEditWidget = true)) FTransform Limits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCESS", Meta = (MakeEditWidget = true)) FTransform InputScales;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCESS", Meta = (MakeEditWidget = true)) float HapticStickAngleScale;
	FTransform GetPoint(int8 _index);

	float Ratio;

	/* Process Start Transform */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCESS - START / END", Meta = (MakeEditWidget = true)) FTransform StartPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCESS - START / END", Meta = (MakeEditWidget = true)) FVector RelativeStartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCESS - START / END", Meta = (MakeEditWidget = true)) FVector RelativeStartRotation;
	/* Process End Transform */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCESS - START / END", Meta = (MakeEditWidget = true)) FTransform EndPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCESS - START / END", Meta = (MakeEditWidget = true)) FVector RelativeEndLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCESS - START / END", Meta = (MakeEditWidget = true)) FVector RelativeEndRotation;
	/* progress size valuse in the tool widget  */
	UPROPERTY(EditAnywhere, Category = "OM3 - PROCESS") FString DisplayProgressValue;
	/* ui bar style */
	UPROPERTY(EditAnywhere, Category = "OM3 - PROCESS") FString UiBarStyle = "Horizontal";
	/* extra add position of ui tool widget root location */
	UPROPERTY(EditAnywhere, Category = "OM3 - PROCESS")  FVector ToolWidgetPosOffset; 
	/* process force stifness */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCESS")  float JointForceStifness = 1.5;
	/* process force cube area length */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCESS")  float JointForceArea = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCEess - END FORCE")  float EndForceThreshold = 0.9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3 - PROCEess - END FORCE")  float EndForceStifness = 1.5;
	UPROPERTY()  AMoveCont* MoveCont;
	/* IsReActiveTool  : whether tool will active after the process. the tool fixed to component when if IsReActiveTool == false */
	UPROPERTY()  bool IsReActiveTool;
	UFUNCTION(BlueprintCallable, Category = OMCEM) AToolItem* GetCurrTool();
	virtual int GetProcessStep() { return 0; }
	UProcessComp();
	virtual void Init() override;
	UFUNCTION(BlueprintCallable, Category = OMCEM) virtual void SetProcessEnable(bool _status, AMoveCont* _movecont, AToolItem* _toolItem, FOmValue _params);
	void UpdateTransform();
	UFUNCTION(BlueprintCallable, Category = OMCEM) float GetRatio() { return Ratio; };
	virtual void SetRatio(float _progress);
	FVector GetForcefeedback();
	UFUNCTION(BlueprintCallable, Category = OMCEM)  float GetProgress();
	
	virtual FVector GetStartPoint();
	virtual PairTrans GetProcessTrans();
	 FTransform GetTransWorld() ;
	 FTransform GetTransRelative();
	 FRotator GetProcessRot(bool _isDelta);
	virtual void OnKeyPress(FString _keyId);
	UFUNCTION(BlueprintCallable, Category = OMCEM) virtual FVector GetProcessLoc(bool _isDelta);
	float ForceFeedbackRatio;
	virtual FOmValue GetObserverParams();
	virtual void UpdateParamsFromObserver(FOmValue _value, bool callPawnEnded);
	//FTransform startingTrans;
protected:
	UPROPERTY() EProgressType progressType;

	void updateToolMenu(FTransform _processTrans);

	FTransform lastProcessTrans;
	FTransform lastProcessTransRel;
	void updateStartingTrans();
	bool isValidStartingTrans;
	FTransform savedLastProgressRelTrans;
	



	bool isEnable;
	UPROPERTY() AToolItem* currTool;
	int8 isRunProcess;
	FOmValue observerParams;
	float progressValuePerClick;
	float stickAngle;
	//-3:(hic bir aci ayarlanmadi), -2(x acisi ok),  -1:(y acisi ok),  0:( fakat z progresinde start ve end noktasindan hic birine yakin degil), 1:(startpoint'den gecildi), 2:(endpoint'den gecildi), 3:(tekrar startpointe donuldu) */
	UPROPERTY()  int8 progressStatus;
	UPROPERTY()  class AHoleField* hole;
	void onClickTimer();

	


};




/*____________________ CLASS<MakeHole> ____________________ */
UCLASS(ClassGroup = (OMCEM), meta = (BlueprintSpawnableComponent))
class OMCEM_API UMakeHoleComp : public UProcessComp
{
	GENERATED_BODY()


public:


	void Init() override;
	void OnKeyPress(FString _keyId) override;
	void SetProcessEnable(bool _status, AMoveCont* _movecont, AToolItem* _toolItem, FOmValue _params) override;
protected:
	void setStep(int8 _step);
	void UpdateParamsFromObserver(FOmValue _params, bool callPawnEnded) override;
	int GetProcessStep() override;
	FRotator defaultLimits;
	int8 step;
};




