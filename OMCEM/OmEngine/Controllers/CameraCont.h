#pragma once

#include "Camera/CameraComponent.h"
#include "OmEngine/Utils/OmStructs.h"
#include "CameraCont.generated.h"


class UOmComp;

UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UCameraCont : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	static  UCameraCont* INS;
	UCameraCont();
	void Init();
	UPROPERTY() float zoomRatio;
	UPROPERTY() FRotator lookAt;
	UPROPERTY() bool bZoomIn;
	UPROPERTY(EditAnywhere) FVector NotVrCamRelLoc;
	UPROPERTY(EditAnywhere) FRotator NotVrCamRot;
	UPROPERTY(EditAnywhere) float CamSpeedDefault = 0.3f;
	UPROPERTY(EditAnywhere) float CamSpeedMin = 0.1f; //alt key
	UPROPERTY(EditAnywhere) float CamSpeedMax = 1.0f; //shift key

	FVector GetLookAtPoint(float _length);
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;
	void ToogleFocusTool();
	void SetMainCam(FString _mainCamId);
	void SetCamLoc(FVector _loc);
	void SetCamLocRel(FVector _loc);
	void ResetCamLocRel();
	void SetCamRot(FRotator _rot);
	void ResetCamPosition();
	void SetCamRotRel(FRotator _rot);
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventRun(const FString &cmd);


private:
	UPROPERTY() FTransform defCamRelTrans;
	UPROPERTY()float defZoomRatio;
	UPROPERTY() FTransform mainCamTargetTransform;
	UPROPERTY() FVector targetLocation;
	UPROPERTY() FVector inputRotationOffset;
	UPROPERTY() TArray<FContactResult> listLookAt;
	UPROPERTY() float mainCamAnimTime;
	UPROPERTY() FTransform defCamTrans;
	void update();

	bool isToogleFocusTool;
	
	
};
