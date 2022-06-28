#pragma once

#include "Runtime/HeadMountedDisplay/Public/MotionControllerComponent.h"
#include "OmEngine/Utils/OmUtils.h"
#include "OmEngine/Inputs/Inputs.h"
#include "OmEngine/Components/VrMotionComp.h"
#include "MoveCont.h"
#include "OmEngine/Components/OmComp.h"
#include "Player/VrMoveCont.h"
#include "PlayerCont.generated.h"


DECLARE_DELEGATE_TwoParams(FInputActionBindEvent, EInputEvent, FString);

/* ___________________ #FRayHit ___________________ */
//USTRUCT(BlueprintType)
//struct OMCEM_API FRayHit
//{
//	GENERATED_USTRUCT_BODY()
//		UPROPERTY(BlueprintReadWrite)	class UOmComp* FromComp;
//	UPROPERTY(BlueprintReadWrite)	class AItem* ToItem;
//	UPROPERTY(BlueprintReadWrite)	class UOmComp* ToComp;
//	UPROPERTY(BlueprintReadWrite)	float Ratio;
//
//	FRayHit() {}
//
//	/* for collision */
//	FRayHit(UPrimitiveComponent* _fromComp, UPrimitiveComponent* _toComp)
//	{
// 		FromComp = Cast<UOmComp>(_fromComp);;
//		ToComp = Cast<UOmComp>(_toComp);
//		ToItem = Cast<AItem>(_toComp->GetOwner());
//	}
//
//	 AMoveCont* GetMove()
//	{
//		return Cast<AMoveCont>(FromComp->GetOwner());
//	}
//
//	FString ToString()
//	{
//		FString s = "";
//		if (FromComp) s.Append("FromComp:(" + Cast<UPrimitiveComponent>(FromComp)->GetName() + ")");
//		if (ToItem) s.Append("ToItem:(" + Cast<AActor>(ToItem)->GetName() + ")");
//		if (ToComp) s.Append("ToComp:(" + Cast<UPrimitiveComponent>(ToComp)->GetName() + ")");
//		 s.Append("Ratio:(" + FString::SanitizeFloat(Ratio) + ")");
//		return s;
//	}
//};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEventInput, bool, _status, FString, _key);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventRayHit, FRayHit, _ray);

//UENUM(BlueprintType)
//enum class EInputAction : uint8
//{
//	MOUSE_LEFT_CLICK,
//	KEY_TAB
//};
//
UENUM(BlueprintType)
enum class EFocus : uint8
{
	GAME,
	PROCESS_COMPENENT,
	UI_CONTROLLER,
};


USTRUCT(BlueprintType)
struct OMCEM_API FTransparentMeshInfo
{
	GENERATED_USTRUCT_BODY()
		//UPROPERTY()	TArray<UMaterailInter
		UPROPERTY(EditAnywhere)	FString MeshId;
		UPROPERTY(EditAnywhere)	UMaterialInterface* TransparentMat;



};

USTRUCT(BlueprintType)
struct OMCEM_API FTransparentMesh
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY()	TArray<UMaterialInterface*> DefaultMats;
		UPROPERTY()	UMaterialInterface* TransparentMat;
	UPROPERTY()	class UMeshComponent* MeshComp;
	

	
};






class UInputListener;


UCLASS()
class OMCEM_API APlayerCont : public APawn
{
	GENERATED_BODY()
public:
	static float GetKeyAxis(FString _keyName);
	static bool GetKeyAction(FString _keyName);
	static FVector GetKeyAxisVector(FString _axisName);
	FEventInput EventKeyAndMouse;
	//FEventRayHit EventRay;
	UPROPERTY(BlueprintReadWrite) UInputListener* InputListener;
	UPROPERTY(BlueprintReadWrite) class UCameraCont* Cam;
	UPROPERTY(BlueprintReadWrite) AMoveCont * MoveR;
	UPROPERTY(BlueprintReadWrite) AMoveCont * MoveL;
	UPROPERTY(EditAnywhere, Category = "OM3") TSubclassOf<AVrMoveCont> ClassVrLeft;
	UPROPERTY(EditAnywhere, Category = "OM3") TSubclassOf<AVrMoveCont> ClassVrRight;
	UPROPERTY(EditAnywhere, Category = "OM3|TransparentMeshes") TArray<FTransparentMeshInfo> TransparentMeshes;
	UPROPERTY(EditAnywhere, Category = "OM3|TransparentMeshes") TArray<FString> TransparentModeHideObjects;

	static APlayerCont* INS;
	virtual void Init();

	void AfterInit();

	virtual void Start();
	
	UFUNCTION() void onInputEventHandler(FInputInfo _info);
	UFUNCTION(BlueprintCallable, Category = "OM3") void TakeTool(FString _move, AToolItem* _tool, bool _releaseCurrent=true);
	void TakeTool(FString _move, FString _id);
	UFUNCTION(BlueprintCallable, Category = OMCEM) AToolItem* PutTool(FString _move, EPutToolRule _putRule, FVector _releaseLoc = FVector::ZeroVector);
	UFUNCTION(BlueprintCallable, Category = OMCEM) void SetProcess(bool _status, FString _move, AToolItem* _tool, UProcessComp* _processComp, FOmValue _params);
	UInputComponent* playerInputComponent;
	virtual void Update(float DeltaSeconds);
	void EndPlay(const EEndPlayReason::Type EndPlayReason);
	//void AddForce(EForceType _type, FVector _direction, float magnitude);
	//void AddForce(EForceType _type, FVector _force) { if (InputManager) InputManager->AddForce(_type, _force); };
	UFUNCTION(BlueprintCallable, Category = OMCEM) TArray<UOmComp*> GetComps();
	EFocus GetFocus();
	void SetFocus(EFocus _focusId);
	void BackToPrevFocus();
	void ToogleTransparentMode();
	void resetValues();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* _InputComp);
	//UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventOnAction(EInputAction _action);
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventInput(FInputInfo InputInfo);
	UFUNCTION(BlueprintCallable, Category = OMCEM)AMoveCont* GetMoveCont(FString _source);
	AOmActor* GetChildActor(FString _compName);

	//float MouseWheelAxis;
	//bool IsMouseLeft;

	//float AxisCamMoveX = 0;
	//float AxisCamMoveY = 0;
	//float AxisCamZoom = 0;
	//float AxisCamRotX = 0;
	//float AxisCamRotY = 0;
	//bool KeyMouseLeft;
	//bool KeyMouseRight;
	//bool KeyZ;
	//bool KeyX;
	//bool KeyC;
	//bool KeyW;
	//bool KeyCtrl;
	//bool KeyShift;
	//bool KeyAlt;
	//bool KeySpace;
	//bool KeyShowRVrLaser;
	//bool KeyShowLVrLaser;
	//float KeyAxisWS;
	//float KeyAxisAD;
	//float KeyAxisUpDown;
	//float BtnTrigger;
	APlayerCont();
	//bool bRightClick = false;

	//void InputKeyAxisWS(float AxisValue);
	//void InputKeyAxisAD(float AxisValue);
	//void InputKeyAxisLeftRight(float AxisValue);
	//void InputKeyAxisUpDown(float AxisValue);
	//void InputAxisCamMoveX(float _axis) { AxisCamMoveX = _axis; }
	//void InputAxisCamMoveY(float _axis) { AxisCamMoveY = _axis; }
	//void InputAxisCamZoom(float _axis) { AxisCamZoom = _axis; }
	//void InputAxisCamRotX(float _axis) { AxisCamRotX = _axis; }
	//void InputAxisCamRotY(float _axis) { AxisCamRotY = _axis; }
	//void InputAxis(float _axis) {  }
	//void InputKeyDownCtrl();
	//void InputKeyFocus();
	//void InputKeyUpCtrl();
	//void InputKeyDownShift();
	//void InputKeyUpShift();
	//void InputKeyDownAlt();
	//void InputKeyDownSpace();
	//void InputKeyUpSpace();
	//void InputKeyUpAlt();
	//void InputKeyDownTab();
	//void InputKeyVrLeftTrigger();
	//void InputKeyVrLeftTriggerDown();
	//void InputKeyVrRightTrigger();
	//void InputKeyVrRightTriggerDown();
	//void InputKeyVrRightGrip();
	//void InputKeyVrRightGripDown();
	//void InputTestSendAllMetricsUp();
	//void InputCloseTheMessageScreen();
	//void InputKeyShowRVrLaserUp();
	//void InputKeyShowRVrLaserDown();
	//void InputKeyShowLVrLaserUp();
	//void InputKeyShowLVrLaserDown();
	//void InputKeyResetCam();
	//void InputKeyTransparentMode();
	//void InputKeyNum0();
	//void InputKeyNum1();
	//void InputKeyNum2();
	//void InputKeyNum3();
	//void InputKeyNum4();
	//void InputKeyNum5();
	//void InputKeyNum6();
	//void InputKeyNum7();
	//void InputKeyNum8();
	//void InputKeyNum9();
	
private:
	class AMoveCont* registerMoveCont(AMoveCont* _moveCont);
	EFocus focusId;
	EFocus lastFocusId;
	TArray<FContactResult> listMotionRayResult;
	void updateMotionCont();
	UPROPERTY() TArray<class AMoveCont*> listMoveConts;
	AVrMoveCont* spawnVrMove(TSubclassOf<AVrMoveCont> _class);
	UPROPERTY()	USceneComponent* compCamRoot;
protected:
	//void onInputKeyHandler(FString btnId, bool status);


	USceneComponent* GetComponent(FString _insName);
	
	USceneComponent* GetComp(FString _compName);

private://mouse
	//void InputMouseLeftDown();
	//void InputMouseLeftUp();
	//void InputMouseRightDown();
	//void InputMouseRightUp();
	bool transparentMode;
	void initializeTransparentMeshes();
	TArray<FTransparentMesh> listTransparentMeshes;
	UFUNCTION() void onInputReceive(bool _status, FString _key);
	
};













UCLASS()
class OMCEM_API UInputListener : public UObject
{
	GENERATED_BODY()

public:
	void EnableMouseAndKeyboardListener(class UInputComponent* _InputComp);
	void Update(float _deltaTime);

	UInputListener();
	FEventInput EventAction; //press & release
	void OnInputAxisListener(float _axis);  //call from PlayerController
	void OnInputActionListener(EInputEvent _type, FString _keyName);  //call from PlayerController
	void addActionMappings(FString _type);
	 //float GetKeyAxis(FString _keyName);
	 bool GetKeyAction(FString _keyName);
	 FVector GetVector(FString _keyName);
protected:
	UPROPERTY() TMap<FString, float> listFloats;
	UPROPERTY() TMap<FString, FVector> listVectors;
	void updateFloat(FString _key, float _value);
	void updateVector(FString _key, float _value);

private:
	UPROPERTY() UInputComponent* inputComponent;
	UPROPERTY() TArray<FString> listAxisNames;
	UPROPERTY() TArray<FString> listActionNames;





};
